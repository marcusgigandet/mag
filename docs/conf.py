import os
import sys

# Let autodoc import Python modules from the repo root once they exist.
sys.path.insert(0, os.path.abspath(".."))

# --- Project identity (edit these per project) ---
project = "MAG - Mathématiques et Graphiques"
author = "Marcus Gigandet"
copyright = "2026, Marcus Gigandet"
# The Makefile overrides this per build with -D: the released git tag
# for each version site, "dev" for working-tree builds.
release = "dev"

# --- Multilingual support (gettext via sphinx-intl) ---
# Source strings are English; `make update` refreshes .po files under
# docs/locales/. Untranslated strings fall back to English.
language = "en"
locale_dirs = ["locales/"]
gettext_compact = False

# --- Site switcher data ---
# Consumed by docs/_templates/page.html; overridden per build by the
# Makefile via -A. Template strings are not covered by gettext, so the
# per-language text lives here: add one entry per language in
# docs_languages below. Site layout: the newest git tag is the site
# root (the default documentation); "dev" and older tags are served
# from their own <version>/ subdirectories.

# Version notice, shown as a brief toast (docs/_templates/page.html
# renders the card; docs/_static/language-switcher.js shows it and
# auto-hides it) when the version being viewed is not the latest
# release: "dev" on working-tree builds, "old" on releases older than
# the latest, linking the same page in the latest release at the site
# root. The dismissal lasts for the current stay in a version;
# selecting the version again via the switcher re-shows it.
# "{version}" is replaced at render time.
version_notice = {
    "en": {
        "dev": {
            "title": "Development version",
            "message": "Built from the working tree; content may change.",
        },
        "old": {
            "title": "This may be an old version of this documentation",
            "message": "You are reading the documentation for {version}, which may be out of date.",
            "latest": "See the current version",
        },
    },
    "fr": {
        "dev": {
            "title": "Version de développement",
            "message": "Construite depuis l'arborescence de travail ; le contenu peut changer.",
        },
        "old": {
            "title": "Cette documentation peut être une ancienne version",
            "message": "Vous lisez la documentation de la version {version}, qui peut être obsolète.",
            "latest": "Voir la version actuelle",
        },
    },
}

# Shown by the version switcher when the target page does not exist in
# the target version. "{version}" is replaced at runtime.
switcher_notice = {
    "en": {
        "message": "This page does not exist in the {version} version. It may have been added after, or removed before, that release.",
        "home": "Go to the {version} home page",
        "dismiss": "Dismiss",
    },
    "fr": {
        "message": "Cette page n'existe pas dans la version {version}. Elle a peut-être été ajoutée après, ou supprimée avant cette version.",
        "home": "Aller à la page d'accueil de {version}",
        "dismiss": "Fermer",
    },
}

# Version switcher panel: filter box placeholder and the badge marking
# the newest release.
version_labels = {
    "en": {"filter": "Filter versions", "latest": "latest"},
    "fr": {"filter": "Filtrer les versions", "latest": "dernière"},
}

html_context = {
    "docs_version": "dev",
    "docs_versions": "dev",
    # Passed by the Makefile via -A; "dev" here only covers direct
    # sphinx-build / sphinx-autobuild runs.
    "docs_latest_version": "dev",
    "docs_root_version": "dev",
    "docs_languages": "en,fr",
    "docs_language_names": "English,Français",
    "version_notice": version_notice,
    "switcher_notice": switcher_notice,
    "version_labels": version_labels,
    # Optional project icon in the sidebar: a path under docs/, e.g.
    # "_static/icon.svg". Leave None for a text-only brand.
    "sidebar_icon": None,
}

extensions = [
    "myst_parser",
    "sphinxcontrib.mermaid",
    "sphinx.ext.autodoc",
    "sphinx.ext.napoleon",
    "sphinx.ext.viewcode",
    "breathe",
    "exhale",
    "sphinx_last_updated_by_git",
    "sphinx_copybutton",
]

html_last_updated_fmt = "%Y-%m-%d"
git_last_updated_timezone = "UTC"

# C++ API reference: Doxygen XML from `make doxygen` (out/doxygen-xml,
# written by the Doxyfile at the repo root), consumed by Breathe;
# Exhale regenerates docs/api/ from it on every build.
breathe_projects = {"mag": os.path.abspath("../out/doxygen-xml")}
breathe_default_project = "mag"
# Also find definitions in .cpp files (Breathe skips them by default).
breathe_implementation_filename_extensions = []

exhale_args = {
    "containmentFolder": "./api",
    "rootFileName": "libraryroot.rst",
    "rootFileTitle": "C++ Reference",
    "doxygenStripFromPath": "..",
    "createTreeView": True,
}

breathe_domain_by_extension = {
    ".hpp": "cpp",
    ".h": "cpp",
    ".cpp": "cpp",
    ".cppm": "cpp",
}

# Treat ```mermaid fences in Markdown as the mermaid directive.
myst_fence_as_directive = ["mermaid"]

mermaid_version = "12.1.0"

# Branding: drop a logo and favicon into docs/_static/ and uncomment.
# html_logo = "_static/logo.svg"
# html_favicon = "_static/favicon.png"

html_theme = "furo"
# Public URL of the deployed site, used for canonical page URLs.
# Set to https://<user>.github.io/<repo>/.
html_baseurl = "https://marcusgigandet.github.io/mag/"
templates_path = ["_templates"]
html_static_path = ["_static"]
html_css_files = ["language-switcher.css", "custom.css"]
html_js_files = ["language-switcher.js"]

html_theme_options = {
    # Non-empty so the announcement bar hosts the switcher markup;
    # language-switcher.js moves the widgets to the page corner.
    "announcement": "Language switcher",
    # "Edit this page" / "view source" header icons (hidden on the
    # Exhale-generated api/ pages via docs/_templates/components/).
    "source_repository": "https://github.com/marcusgigandet/mag",
    "source_branch": "main",
    "source_directory": "docs",
    "footer_icons": [
        {
            "name": "GitHub",
            # Set to your project's GitHub repository URL.
            "url": "https://github.com/marcusgigandet/mag",
            "html": (
                '<svg xmlns="http://www.w3.org/2000/svg" width="24" height="24"'
                ' viewBox="0 0 16 16" fill="currentColor" role="img"'
                ' aria-hidden="true"><path d="M8 0C3.58 0 0 3.58 0 8c0'
                " 3.54 2.29 6.53 5.47 7.59.4.07.55-.17.55-.38"
                " 0-.19-.01-.82-.01-1.49-2.01.37-2.53-.49-2.69-.94-.09-.23-.48-.94-.82-1.13-.28-.15-.68-.52-.01-.53.63-.01"
                " 1.08.58 1.23.82.72 1.21 1.87.87 2.33.66.07-.52.28-.87.51-1.07-1.78-.2-3.64-.89-3.64-3.95"
                " 0-.87.31-1.59.82-2.15-.08-.2-.36-1.02.08-2.12 0 0 .67-.21 2.2.82.64-.18"
                " 1.32-.27 2-.27s1.36.09 2 .27c1.53-1.04 2.2-.82 2.2-.82.44 1.1.16 1.92.08"
                " 2.12.51.56.82 1.27.82 2.15 0 3.07-1.87 3.75-3.65 3.95.29.25.54.73.54"
                " 1.48 0 1.07-.01 1.93-.01 2.2 0 .21.15.46.55.38A8.01 8.01 0 0 0 16"
                " 8c0-4.42-3.58-8-8-8Z"
                '"/></svg>'
            ),
        },
        # Optional repository-stat badges (stars, forks, issues — the
        # shields.io README badges, rendered at view time). Off by
        # default; uncomment an entry to enable. Set the owner/repo in
        # the URL. The inline style overrides Furo's footer-icon rule
        # (.bottom-of-page .icons img { height: 1em; width: 1em }),
        # which would otherwise squash the 20px-tall badges.
        # {
        #     "name": "GitHub stars",
        #     "url": "https://github.com/marcusgigandet/mag/stargazers",
        #     "html": (
        #         '<img alt="GitHub stars" loading="lazy" '
        #         'style="width: auto; height: 20px" '
        #         'src="https://img.shields.io/github/stars/'
        #         'marcusgigandet/mag.svg?style=flat-square">'
        #     ),
        # },
    ],
}

def _page_meta(app, pagename, templatename, context, doctree):
    # Canonical URL and robots handling: the root version (the latest
    # release) serves at the site root and stays indexable; every other
    # version ("dev", older tags) serves from its own subdirectory and
    # is noindex.
    version = context.get("docs_version", "dev")
    root = context.get("docs_root_version", "dev")
    language = app.config.language or "en"

    if app.config.html_baseurl:
        prefix = "" if version == root else f"{version}/"

        if language != "en":
            prefix += f"{language}/"

        context["pageurl"] = (
            app.config.html_baseurl.rstrip("/")
            + "/"
            + prefix
            + pagename
            + ".html"
        )

    if pagename == "404":
        context["pageurl"] = None
        context["metatags"] = (
            context.get("metatags", "")
            + '<meta name="robots" content="noindex">\n'
        )
    elif version != root and pagename != "search":
        context["metatags"] = (
            context.get("metatags", "")
            + '<meta name="robots" content="noindex">\n'
        )


def setup(app):
    app.connect("html-page-context", _page_meta)
