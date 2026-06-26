import os
import subprocess
import sys
from datetime import datetime
from pathlib import Path
from typing import Final

sys.path.insert(0, str(Path(__file__).resolve().parent))

from docs_config import (
    DEFAULT_LANGUAGE,
    DOCS_DIR,
    ISSUES_GITHUB_PATH,
    PROJECT_AUTHOR,
    PROJECT_NAME,
    REMOTE_HTML_BASE_URL,
    REPOSITORY_URL,
    SITE_LANGUAGES,
    VERSIONS_YAML_PATH,
    VersionDetails,
    discover_languages,
    load_versions,
)

LOCAL_BUILD: Final[bool] = os.getenv("DOCS_LOCAL_BUILD", "0") == "1"
HTML_BASE_URL: Final[str] = "" if LOCAL_BUILD else REMOTE_HTML_BASE_URL
SITE_BASE_URL: Final[str] = HTML_BASE_URL.rstrip("/")


def build_local_language_link(current_language: str, target_language: str) -> str:
    """
    Build the relative local-preview link from one language tree to another.

    :param current_language: Language currently being rendered.
    :param target_language: Language to link to.
    :return: Relative link rooted at the target language tree.
    """
    if target_language == current_language:
        return "."
    if target_language == DEFAULT_LANGUAGE:
        return ".." if current_language != DEFAULT_LANGUAGE else "."
    if current_language == DEFAULT_LANGUAGE:
        return target_language
    return f"../{target_language}"


def build_version_url(version_name: str, language_name: str) -> str:
    """
    Build the published URL for a version/language pair.

    :param version_name: Version segment such as ``"latest"`` or ``"0.1.0"``.
    :param language_name: Language segment for that version.
    :return: URL base for the selected version/language pair.
    """
    if version_name == "latest":
        if language_name == DEFAULT_LANGUAGE:
            return SITE_BASE_URL or "/"
        return (
            f"{SITE_BASE_URL}/{language_name}" if SITE_BASE_URL else f"/{language_name}"
        )

    return (
        f"{SITE_BASE_URL}/{version_name}/{language_name}"
        if SITE_BASE_URL
        else f"/{version_name}/{language_name}"
    )


def resolve_version_language(
    version_name: str,
    preferred_language: str,
    versions: dict[str, VersionDetails],
) -> str:
    """
    Choose the best language link for a version switcher entry.

    :param version_name: Version whose published languages should be inspected.
    :param preferred_language: Language currently being viewed.
    :param versions: Version metadata loaded from ``versions.yaml``.
    :return: Language code that exists for the requested version.
    """
    available_languages = versions.get(version_name, {}).get("languages", [])
    if not isinstance(available_languages, list) or not available_languages:
        return preferred_language
    if preferred_language in available_languages:
        return preferred_language
    if DEFAULT_LANGUAGE in available_languages:
        return DEFAULT_LANGUAGE
    return available_languages[0]


def build_local_html_context(current_language: str) -> dict[str, object]:
    """
    Build version/language navigation for a local-only docs preview.

    :param current_language: The language code for the current build, such as ``"en"`` or ``"fr"``.
    :return: A dictionary containing only local relative links for the current preview build.
    """
    languages = discover_languages(
        DOCS_DIR,
        default_language=DEFAULT_LANGUAGE,
        configured_languages=SITE_LANGUAGES,
    )
    return {
        "current_language": current_language,
        "languages": [
            (language_name, build_local_language_link(current_language, language_name))
            for language_name in languages
        ],
        "current_version": "local",
        "repository_url": REPOSITORY_URL,
        "versions": [("local", "." if current_language == DEFAULT_LANGUAGE else "..")],
    }


def build_html_context(
    current_version: str, current_language: str, local_build: bool
) -> dict[str, object]:
    """
    Build the version/language switcher context used by the docs template.

    :param current_version: The docs version currently being built, such as ``"latest"`` or ``"0.1.0"``.
    :param current_language: The language code for the current build, such as ``"en"`` or ``"fr"``.
    :param local_build: Whether the current build is a local preview rather than a GitHub Pages build.
    :return: A dictionary containing the current selection plus version and language navigation entries.
    """
    if local_build:
        return build_local_html_context(current_language)

    versions = load_versions(VERSIONS_YAML_PATH)
    languages: list[tuple[str, str]] = []
    version_links: list[tuple[str, str]] = []

    if current_version == "latest":
        for language_name in discover_languages(
            DOCS_DIR,
            versions=versions,
            default_language=DEFAULT_LANGUAGE,
            configured_languages=SITE_LANGUAGES,
        ):
            languages.append(
                (language_name, build_version_url("latest", language_name))
            )
    else:
        for language_name in versions.get(current_version, {}).get("languages", []):
            languages.append(
                (language_name, build_version_url(current_version, language_name))
            )

    version_links.append(("latest", build_version_url("latest", current_language)))

    for version_name in versions:
        version_language = resolve_version_language(
            version_name, current_language, versions
        )
        version_links.append(
            (version_name, build_version_url(version_name, version_language))
        )

    return {
        "current_language": current_language,
        "languages": languages,
        "current_version": current_version,
        "repository_url": REPOSITORY_URL,
        "versions": version_links,
    }


def get_git_tag() -> str:
    """
    Return the most recent Git tag, or a fallback when none is available.

    :return: The most recent Git tag, or ``"0.0.0"`` when no tag can be resolved.
    """
    try:
        return subprocess.check_output(
            ["git", "describe", "--tags", "--abbrev=0"],
            text=True,
            stderr=subprocess.DEVNULL,
        ).strip()
    except subprocess.CalledProcessError:
        return "0.0.0"


# -- Project information -----------------------------------------------------
project = PROJECT_NAME
author = PROJECT_AUTHOR
copyright = f"{datetime.now().year}, {PROJECT_AUTHOR}"

release = get_git_tag()
version = ".".join(release.split(".")[:2])

# -- Internationalization ----------------------------------------------------
locale_dirs = ["locale/"]
gettext_compact = False
gettext_uuid = True
language = os.getenv("current_language", DEFAULT_LANGUAGE)

# -- General configuration ---------------------------------------------------
templates_path = ["_templates"]
extensions = [
    "sphinx.ext.githubpages",
    "sphinx.ext.intersphinx",
    "sphinx_copybutton",
    "sphinx_design",
    "sphinx_last_updated_by_git",
    "sphinx_multiversion",
]
issues_github_path = ISSUES_GITHUB_PATH
if issues_github_path:
    extensions.append("sphinx_issues")
if not LOCAL_BUILD and HTML_BASE_URL:
    extensions.append("sphinx_sitemap")
exclude_patterns = [
    "_build",
    "Thumbs.db",
    ".DS_Store",
]

# -- Options for HTML output -------------------------------------------------
html_theme = "sphinx_rtd_theme"
html_static_path = ["_static"]
html_baseurl = HTML_BASE_URL
site_baseurl = SITE_BASE_URL

# -- Options for sphinx_last_updated_by_git ----------------------------------
git_untracked_check_dependencies = True
git_untracked_show_sourcelink = True
git_last_updated_metatags = True

# -- Options for todo --------------------------------------------------------
todo_include_todos = True
todo_emit_warnings = False
todo_link_only = False

# -- Versioned docs configuration --------------------------------------------
build_all_docs = True
html_context: dict[str, object] = {}

if build_all_docs:
    current_language = language
    current_version = os.getenv("current_version", "latest")
    html_context = build_html_context(current_version, current_language, LOCAL_BUILD)
