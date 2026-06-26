import os
import subprocess
from pathlib import Path
from typing import Final, TypedDict

import yaml


class VersionDetails(TypedDict, total=False):
    tag: str
    languages: list[str]


DOCS_ROOT: Final[Path] = Path(__file__).resolve().parent.parent
VERSIONS_PATH: Final[Path] = DOCS_ROOT / "versions.yaml"
PROJECT_NAME: Final[str] = "MAG - Mathématiques et Graphiques"
PROJECT_AUTHOR: Final[str] = "Marcus Gigandet"
REPOSITORY_URL: Final[str] = "https://github.com/marcusgigandet/mag"
REMOTE_HTML_BASE_URL: Final[str] = "https://marcusgigandet.github.io/mag/"
LOCAL_BUILD: Final[bool] = os.getenv("DOCS_LOCAL_BUILD", "0") == "1"
HTML_BASE_URL: Final[str] = "" if LOCAL_BUILD else REMOTE_HTML_BASE_URL
SITE_BASE_URL: Final[str] = HTML_BASE_URL.rstrip("/")


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


def load_versions(path: Path) -> dict[str, VersionDetails]:
    """
    Load version metadata from ``versions.yaml``.

    :param path: Path to the versions YAML file.
    :return: A mapping of version names to validated version metadata.
    """
    with path.open("r", encoding="utf-8") as yaml_file:
        raw_versions = yaml.safe_load(yaml_file) or {}

    if not isinstance(raw_versions, dict):
        raise TypeError("versions.yaml must contain a mapping of version names to metadata.")

    versions: dict[str, VersionDetails] = {}
    for version_name, details in raw_versions.items():
        if not isinstance(version_name, str):
            raise TypeError("versions.yaml version keys must be strings.")
        if not isinstance(details, dict):
            raise TypeError(f"Version '{version_name}' metadata must be a mapping.")

        languages = details.get("languages", [])
        if not isinstance(languages, list) or not all(isinstance(language_name, str) for language_name in languages):
            raise TypeError(f"Version '{version_name}' languages must be a list of strings.")

        tag = details.get("tag", version_name)
        if not isinstance(tag, str):
            raise TypeError(f"Version '{version_name}' tag must be a string.")

        versions[version_name] = {
            "tag": tag,
            "languages": languages,
        }

    return versions


def build_local_html_context(current_language: str) -> dict[str, object]:
    """
    Build version/language navigation for a local-only docs preview.

    :param current_language: The language code for the current build, such as ``"en"`` or ``"fr"``.
    :return: A dictionary containing only local relative links for the current preview build.
    """
    root_link = "." if current_language == "en" else ".."
    french_link = "fr" if current_language == "en" else "."

    return {
        "current_language": current_language,
        "languages": [
            ("en", root_link),
            ("fr", french_link),
        ],
        "current_version": "local",
        "repository_url": REPOSITORY_URL,
        "versions": [("local", root_link)],
    }


def build_html_context(current_version: str, current_language: str, local_build: bool) -> dict[str, object]:
    """
    Build the version/language switcher context used by the docs template.

    :param current_version: The docs version currently being built, such as ``"latest"`` or ``"0.1.0"``.
    :param current_language: The language code for the current build, such as ``"en"`` or ``"fr"``.
    :param local_build: Whether the current build is a local preview rather than a GitHub Pages build.
    :return: A dictionary containing the current selection plus version and language navigation entries.
    """
    if local_build:
        return build_local_html_context(current_language)

    versions = load_versions(VERSIONS_PATH)
    languages: list[tuple[str, str]] = []
    version_links: list[tuple[str, str]] = []

    if current_version == "latest":
        languages.extend(
            [
                ("en", SITE_BASE_URL),
                ("fr", f"{SITE_BASE_URL}/fr"),
            ]
        )
    else:
        for language_name in versions.get(current_version, {}).get("languages", []):
            languages.append((language_name, f"{SITE_BASE_URL}/{current_version}/{language_name}"))

    if current_language == "en":
        version_links.append(("latest", SITE_BASE_URL))
    elif current_language == "fr":
        version_links.append(("latest", f"{SITE_BASE_URL}/fr"))

    for version_name in versions:
        version_links.append((version_name, f"{SITE_BASE_URL}/{version_name}/{current_language}"))

    return {
        "current_language": current_language,
        "languages": languages,
        "current_version": current_version,
        "repository_url": REPOSITORY_URL,
        "versions": version_links,
    }


# -- Project information -----------------------------------------------------
project = PROJECT_NAME
author = PROJECT_AUTHOR
copyright = f"2026, {PROJECT_AUTHOR}"

release = get_git_tag()
version = ".".join(release.split(".")[:2])

# -- Internationalization ----------------------------------------------------
locale_dirs = ["locale/"]
gettext_compact = False
gettext_uuid = True
language = os.getenv("current_language", "en")

# -- General configuration ---------------------------------------------------
templates_path = ["_templates"]
extensions = [
    "sphinx.ext.githubpages",
    "sphinx.ext.intersphinx",
    "sphinx_design",
    "sphinx_issues",
    "sphinx_last_updated_by_git",
    "sphinx_multiversion",
]
if not LOCAL_BUILD:
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

# -- Options for sphinx_issues -----------------------------------------------
issues_github_path = "marcusgigandet/mag"

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
