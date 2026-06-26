# Makefile for Sphinx documentation

# Mute commands unless VERBOSE is set
ifeq ($(VERBOSE),)
	VS=@
else
	VS=
endif

# Makefile variables
SPHINX_OPTS    ?= -q
SPHINX_BUILD   ?= sphinx-build
BUILD_DIR      ?= _build
SOURCE_DIR     ?= source
VENV_DIR = .venv
PYTHON   = python3
BUILD_LOG = build.log
ERROR_LOG = error.log
BUILD_PAGES_SCRIPT = $(SOURCE_DIR)/build_docs.py

# Makefile targets
help:
	$(SPHINX_BUILD) -M help "$(SOURCE_DIR)" "$(BUILD_DIR)" $(SPHINX_OPTS)

gettext:
	@echo "Generating translation templates (.pot)..."
	$(VS)$(SPHINX_BUILD) -M gettext "$(SOURCE_DIR)" "$(BUILD_DIR)" $(SPHINX_OPTS)
	@echo "Translation templates generated."

i18n-update:
	@echo "Updating French translation catalogs (.po)..."
	$(VS)sphinx-intl update -p "$(BUILD_DIR)"/gettext -l fr
	@echo "French translation catalogs updated."

i18n-build: html
	@echo "Localized documentation built."

translate: gettext i18n-update
	@echo "Translation workflow complete."

clean:
	@echo "Cleaning documentation build dir..."
	$(VS)rm -rf "$(BUILD_DIR)" > $(BUILD_LOG) 2> $(ERROR_LOG) || true
	@echo "Cleaned up the build dir."

html:
	@echo "Running sphinx html generation..."
	$(VS)$(VENV_DIR)/bin/python -m sphinx -b html "$(SOURCE_DIR)" "$(BUILD_DIR)/html" $(SPHINX_OPTS) > $(BUILD_LOG) 2>> $(ERROR_LOG)
	@echo "html built."

pages:
	@echo "Building all versions and languages..."
	$(VS)$(VENV_DIR)/bin/python "$(BUILD_PAGES_SCRIPT)"
	@echo "Versioned pages built."

local-pages:
	@echo "Building local docs preview..."
	$(VS)$(VENV_DIR)/bin/python "$(BUILD_PAGES_SCRIPT)" --local
	@echo "Local docs preview built."

venv:
	@echo "Setting up virtual environment..."
	$(VS)test -d $(VENV_DIR) || $(PYTHON) -m venv $(VENV_DIR)
	@echo "Installing dependencies..."
	$(VS)$(VENV_DIR)/bin/pip install --upgrade pip > $(BUILD_LOG) 2>> $(ERROR_LOG)
	$(VS)$(VENV_DIR)/bin/pip install -r $(CURDIR)/requirements.txt > $(BUILD_LOG) 2>> $(ERROR_LOG)
	@echo "Finished setting up the virtual environment."

all: clean venv translate pages

all-local: clean venv translate local-pages

.PHONY: all all-local venv clean html gettext i18n-update i18n-build translate pages local-pages
