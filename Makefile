# Documentation site build. Site layout: the newest git tag is the
# site root (the default documentation), older tags under
# out/pages/<tag>/, the working-tree ("dev") build under
# out/pages/dev/, one <lang>/ subdirectory per non-English language.
# See README.md for the checklist to adapt the template to a project.

VENV    := .venv
PIP     := $(VENV)/bin/pip
SPHINX  := $(VENV)/bin/sphinx-build
LIVE    := $(VENV)/bin/sphinx-autobuild
INTL    := $(VENV)/bin/sphinx-intl
DOXYGEN := doxygen

# Site languages (LANG_NAMES pairs positionally with LANGUAGES).
LANGUAGES  := en fr
LANG_NAMES := English,Français

# Released docs versions: one git tag per version, each containing
# docs/, Doxyfile, and the sources Doxygen consumes. MAG release tags
# carry no "v" prefix, and only tags that ship the Markdown docs tree
# (docs/index.md) are buildable; older tags are skipped.
VERSIONS ?= $(shell for t in $$(git tag --list | sort -V); do git cat-file -e $$t:docs/index.md 2>/dev/null && echo $$t; done)

# The newest tag builds into the site root and carries the "latest"
# badge. Without tags, "dev" serves at the site root.
LATEST_VERSION := $(if $(VERSIONS),$(lastword $(VERSIONS)),dev)
DEV_OUTDIR := out/pages$(if $(filter dev,$(LATEST_VERSION)),,/dev)

# The version of the current checkout: the git tag HEAD is on, "dev"
# otherwise (HEAD not exactly at a tag). Uncommitted changes are
# included — this is the local state, not the released tag.
LOCAL_VERSION := $(shell git describe --tags --exact-match HEAD 2>/dev/null || echo dev)

empty :=
space := $(empty) $(empty)
comma := ,
LANGUAGES_CSV := $(subst $(space),$(comma),$(LANGUAGES))
VERSIONS_CSV  := $(subst $(space),$(comma),$(VERSIONS))
ALL_VERSIONS  := dev$(if $(VERSIONS_CSV),$(comma)$(VERSIONS_CSV))
# All version ids as a grep alternation (file-list exclusions below).
VERSIONS_ALT  := $(subst $(comma),|,$(ALL_VERSIONS))

LANG_FLAGS := -A docs_languages=$(LANGUAGES_CSV) \
              -A docs_language_names=$(LANG_NAMES)
DEV_VERSION_FLAGS := -A docs_versions=dev
ALL_VERSION_FLAGS := -A docs_versions=$(ALL_VERSIONS) \
                    -A docs_latest_version=$(LATEST_VERSION) \
                    -A docs_root_version=$(LATEST_VERSION)

.PHONY: deps doxygen gettext update live local linkcheck versions pages clean

deps: $(SPHINX)

$(SPHINX):
	@python3 -m venv $(VENV)
	@$(PIP) install -r requirements-docs.txt

doxygen:
	@mkdir -p out
	@$(DOXYGEN) Doxyfile

gettext: deps doxygen
	@$(SPHINX) -b gettext docs out/gettext

# Refresh .po files under docs/locales/ and compile .mo catalogs.
# The .mo files are git-ignored build artifacts: every HTML build
# compiles them itself (live, versions, pages), so fresh checkouts
# and CI never miss translations.
update: gettext
	@$(INTL) update -p out/gettext -d docs/locales $(addprefix -l ,$(filter-out en,$(LANGUAGES)))
	@cd docs && ../$(INTL) build

# Exits non-zero on broken external links; CI runs this weekly.
linkcheck: deps doxygen
	@$(SPHINX) -b linkcheck docs out/linkcheck

live: deps doxygen
	@cd docs && ../$(INTL) build
	$(LIVE) --port 8000 --watch README.md --watch src -D release=dev $(LANG_FLAGS) $(DEV_VERSION_FLAGS) -A docs_version=dev docs out/pages-local

# Build the current checkout's site (LOCAL_VERSION) into
# out/pages-local/, as that version deploys: single version, so no
# version panel. docs_latest_version is the repository's real newest
# tag (an older checkout does not claim to be latest); docs_root_version
# is the local version itself, since the local build serves it at its
# root. No server — open the directory or serve it yourself. The
# multi-version chrome only exists in the `make pages` output.
local: deps doxygen
	@cd docs && ../$(INTL) build
	@$(SPHINX) -b html -D release=$(LOCAL_VERSION) $(LANG_FLAGS) \
	   -A docs_versions=$(LOCAL_VERSION) -A docs_latest_version=$(LATEST_VERSION) \
	   -A docs_root_version=$(LOCAL_VERSION) -A docs_version=$(LOCAL_VERSION) \
	   docs out/pages-local
	@for lang in $(filter-out en,$(LANGUAGES)); do \
		$(SPHINX) -b html -D language=$$lang -D release=$(LOCAL_VERSION) $(LANG_FLAGS) \
		   -A docs_versions=$(LOCAL_VERSION) -A docs_latest_version=$(LATEST_VERSION) \
		   -A docs_root_version=$(LOCAL_VERSION) -A docs_version=$(LOCAL_VERSION) \
		   docs out/pages-local/$$lang; \
	done

# One site per released version, built from the tag's own content
# (including its Doxygen XML) via a temporary git worktree, with the
# site chrome (Doxyfile, conf.py, 404.md, _templates/, custom.css,
# switcher JS/CSS) overlaid from the working tree so every version
# renders the current chrome. The newest tag builds into the site root.
versions: deps
	@mkdir -p out/pages
	@for tag in $(VERSIONS); do \
		worktree=out/versions/$$tag; \
		git worktree add --detach $$worktree $$tag || continue; \
		outdir=out/pages/$$tag; \
		if [ "$$tag" = "$(LATEST_VERSION)" ]; then outdir=out/pages; fi; \
		cp -R docs/_templates/. $$worktree/docs/_templates/; \
		cp docs/conf.py docs/404.md $$worktree/docs/; \
		cp Doxyfile $$worktree/Doxyfile; \
		cp docs/_static/language-switcher.js docs/_static/language-switcher.css \
		   docs/_static/custom.css $$worktree/docs/_static/; \
		mkdir -p $$worktree/out; \
		( cd $$worktree/docs && ../../../../$(INTL) build ); \
		( cd $$worktree && $(DOXYGEN) Doxyfile ); \
		$(SPHINX) -b html -D release=$$tag $(LANG_FLAGS) $(ALL_VERSION_FLAGS) -A docs_version=$$tag $$worktree/docs $$outdir; \
		for lang in $(filter-out en,$(LANGUAGES)); do \
			$(SPHINX) -b html -D language=$$lang -D release=$$tag $(LANG_FLAGS) $(ALL_VERSION_FLAGS) -A docs_version=$$tag $$worktree/docs $$outdir/$$lang; \
		done; \
		git worktree remove --force $$worktree; \
	done

pages: deps doxygen versions
	@cd docs && ../$(INTL) build
	@$(SPHINX) -b html -D release=dev $(LANG_FLAGS) $(ALL_VERSION_FLAGS) -A docs_version=dev docs $(DEV_OUTDIR)
	@for lang in $(filter-out en,$(LANGUAGES)); do \
		$(SPHINX) -b html -D language=$$lang -D release=dev $(LANG_FLAGS) $(ALL_VERSION_FLAGS) -A docs_version=dev docs $(DEV_OUTDIR)/$$lang; \
	done
# Drop the sites of versions that no longer exist (deleted or renamed
# tags), so no orphaned version gets deployed.
	@for site in out/pages/*; do \
		[ -f "$$site/index.html" ] || continue; \
		ver=$${site#out/pages/}; \
		case ",$(ALL_VERSIONS)," in *",$$ver,"*) continue;; esac; \
		case " $(LANGUAGES) " in *" $$ver "*) continue;; esac; \
		echo "Removing stale version directory: $$site"; \
		rm -rf "$$site"; \
	done
# Page manifest per version (docs-pages.js): the version switcher
# checks that a target page exists in the target version before
# navigating (docs/_static/language-switcher.js).
	@for site in out/pages out/pages/*; do \
		[ -f "$$site/index.html" ] || continue; \
		ver=$${site#out/pages}; ver=$${ver#/}; \
		case " $(LANGUAGES) " in *" $$ver "*) continue;; esac; \
		ver=$${ver:-$(LATEST_VERSION)}; \
		{ echo 'window.docsPages = window.docsPages || {};'; \
		  printf 'window.docsPages["%s"] = [' "$$ver"; \
		  find "$$site" -name '*.html' \
		    | grep -vE "^$$site/($(VERSIONS_ALT))/" \
		    | sed -e "s|^$$site/|  \"|" -e 's|$$|",|'; \
		  echo '];'; \
		} > "$$site/docs-pages.js"; \
	done
# Sitemap and robots.txt for the site root: the indexable pages only
# (the site root's version, all languages); the other versions are
# robots-noindex'd (see the html-page-context hook in docs/conf.py).
	@base=`$(VENV)/bin/python -c "import sys; sys.path.insert(0, 'docs'); import conf; print(conf.html_baseurl.rstrip('/'))"`; \
	{ echo '<?xml version="1.0" encoding="UTF-8"?>'; \
	  echo '<urlset xmlns="http://www.sitemaps.org/schemas/sitemap/0.9">'; \
	  find out/pages -name '*.html' \
	    | grep -vE "^out/pages/($(VERSIONS_ALT))/" \
	    | grep -vE "/(404|search)\.html$$" \
	    | sort \
	    | sed -e "s|^out/pages/|  <url><loc>$$base/|" -e 's|$$|</loc></url>|'; \
	  echo '</urlset>'; \
	} > out/pages/sitemap.xml; \
	printf 'User-agent: *\nSitemap: %s/sitemap.xml\n' "$$base" > out/pages/robots.txt
	@for tag in $(VERSIONS); do rm -f out/pages/$$tag/sitemap.xml out/pages/$$tag/robots.txt; done

clean:
	@rm -rf out docs/api
	@rm -rf doxygen-xml docs/doxygen-xml docs/pages docs/pages-local docs/gettext docs/linkcheck docs/.versions
	@git worktree prune
