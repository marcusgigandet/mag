import os
import subprocess
import sys
from datetime import datetime
from pathlib import Path
from typing import Final

sys.path.insert(0, str(Path(__file__).resolve().parent))

from docs_config import (
    BUILD_RELATIVE_PATH,
    DEFAULT_LANGUAGE,
    DOCS_DIR,
    DOCS_RELATIVE_PATH,
    DOCS_SOURCE_RELATIVE_PATH,
    DOXYGEN_ENABLED,
    DOXYGEN_EXCLUDE_PATTERNS,
    DOXYGEN_EXECUTABLE,
    DOXYGEN_FILE_PATTERNS,
    DOXYGEN_INPUT_PATHS,
    DOXYGEN_PROJECT_NAME,
    DOXYGEN_RECURSIVE,
    DOXYGEN_STRIP_FROM_PATH,
    ENABLE_GITHUB_ISSUES,
    ENABLE_SITEMAP,
    EXCLUDE_PATTERNS,
    HTML_STATIC_PATHS,
    HTML_THEME,
    ISSUES_GITHUB_PATH,
    LATEST_REF,
    LOCALE_RELATIVE_PATH,
    PROJECT_AUTHOR,
    PROJECT_NAME,
    REPO_ROOT,
    REMOTE_HTML_BASE_URL,
    REPOSITORY_URL,
    SITE_LANGUAGES,
    SPHINX_EXTENSIONS,
    CONFIG_YAML_PATH,
    VersionDetails,
    discover_languages,
    load_versions,
    run_git_command,
)

LOCAL_BUILD: Final[bool] = os.getenv("DOCS_LOCAL_BUILD", "0") == "1"
HTML_BASE_URL: Final[str] = "" if LOCAL_BUILD else REMOTE_HTML_BASE_URL
SITE_BASE_URL: Final[str] = HTML_BASE_URL.rstrip("/")


def quote_doxygen_value(value: str) -> str:
    """
    Quote a value for a generated Doxygen configuration file.

    :param value: Raw value that should be quoted for Doxygen.
    :return: Doxygen-safe quoted value.
    """
    escaped_value = value.replace("\\", "\\\\").replace('"', '\\"')
    return f'"{escaped_value}"'


def render_doxygen_list(values: list[str]) -> str:
    """
    Render a list of values for a generated Doxygen configuration file.

    :param values: Values that should be emitted on one Doxygen config line.
    :return: Space-delimited Doxygen config values.
    """
    return " ".join(quote_doxygen_value(value) for value in values)


def generate_doxygen_xml() -> Path:
    """
    Generate Doxygen XML output for the project sources used by Breathe.

    :return: Path to the generated Doxygen XML directory.
    """
    doxygen_output_dir = (DOCS_DIR / BUILD_RELATIVE_PATH / "doxygen").resolve()
    xml_output_dir = doxygen_output_dir / "xml"
    doxygen_output_dir.mkdir(parents=True, exist_ok=True)

    input_paths = [(REPO_ROOT / input_path).resolve().as_posix() for input_path in DOXYGEN_INPUT_PATHS]
    strip_from_path = (REPO_ROOT / DOXYGEN_STRIP_FROM_PATH).resolve().as_posix()
    config_lines = [
        f"PROJECT_NAME = {quote_doxygen_value(DOXYGEN_PROJECT_NAME)}",
        f"OUTPUT_DIRECTORY = {quote_doxygen_value(doxygen_output_dir.as_posix())}",
        "GENERATE_HTML = NO",
        "GENERATE_LATEX = NO",
        "GENERATE_XML = YES",
        "XML_OUTPUT = xml",
        "QUIET = YES",
        "WARN_IF_UNDOCUMENTED = NO",
        "WARN_IF_DOC_ERROR = YES",
        "EXTRACT_ALL = NO",
        "EXTRACT_PRIVATE = NO",
        "BUILTIN_STL_SUPPORT = YES",
        "RECURSIVE = YES" if DOXYGEN_RECURSIVE else "RECURSIVE = NO",
        f"INPUT = {render_doxygen_list(input_paths)}",
        f"FILE_PATTERNS = {' '.join(DOXYGEN_FILE_PATTERNS)}",
        f"STRIP_FROM_PATH = {quote_doxygen_value(strip_from_path)}",
        "EXTENSION_MAPPING = cppm=C++",
    ]
    if DOXYGEN_EXCLUDE_PATTERNS:
        config_lines.append(
            f"EXCLUDE_PATTERNS = {' '.join(DOXYGEN_EXCLUDE_PATTERNS)}"
        )

    doxygen_config_path = doxygen_output_dir / "Doxyfile"
    doxygen_config_path.write_text("\n".join(config_lines) + "\n", encoding="utf-8")
    subprocess.run(
        [DOXYGEN_EXECUTABLE, str(doxygen_config_path)],
        check=True,
        cwd=REPO_ROOT,
    )
    return xml_output_dir


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


def build_version_label(version_name: str, details: VersionDetails | None = None) -> str:
    """
    Build the display label for a docs version entry.

    :param version_name: Version name such as ``"latest"`` or ``"0.1.0"``.
    :param details: Optional metadata for the version.
    :return: Display label for the version switcher.
    """
    if version_name == "latest":
        return "latest"
    if details is None:
        return version_name

    title = details.get("title", version_name)
    status = details.get("status", "")
    return f"{title} ({status})" if status else title


def normalize_vcs_ref(ref: str) -> str:
    """
    Normalize a Git ref for GitHub edit URLs.

    :param ref: Git ref such as ``origin/main`` or ``v0.1.0``.
    :return: Web-safe ref name.
    """
    return ref.removeprefix("origin/")


def resolve_local_edit_ref() -> str:
    """
    Resolve the most useful ref for local-preview edit links.

    :return: Current branch when available, otherwise the configured latest ref.
    """
    current_branch = run_git_command(
        ["git", "symbolic-ref", "--quiet", "--short", "HEAD"], REPO_ROOT
    )
    if current_branch:
        return current_branch

    return LATEST_REF or "main"

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
        "default_language": DEFAULT_LANGUAGE,
        "local_build": True,
        "languages": [
            (language_name, build_local_language_link(current_language, language_name))
            for language_name in languages
        ],
        "current_version_label": "local",
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

    versions = load_versions(CONFIG_YAML_PATH)
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

    version_links.append(
        (build_version_label("latest"), build_version_url("latest", current_language))
    )

    for version_name in versions:
        version_language = resolve_version_language(
            version_name, current_language, versions
        )
        version_links.append(
            (
                build_version_label(version_name, versions.get(version_name)),
                build_version_url(version_name, version_language),
            )
        )

    return {
        "current_language": current_language,
        "default_language": DEFAULT_LANGUAGE,
        "local_build": False,
        "languages": languages,
        "current_version_label": build_version_label(
            current_version, versions.get(current_version)
        ),
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
locale_dirs = [f"{LOCALE_RELATIVE_PATH.as_posix()}/"]
gettext_compact = False
gettext_uuid = True
language = os.getenv("current_language", DEFAULT_LANGUAGE)

# -- General configuration ---------------------------------------------------
templates_path = ["_templates"]
extensions = SPHINX_EXTENSIONS.copy()
issues_github_path = ISSUES_GITHUB_PATH
if DOXYGEN_ENABLED and "breathe" not in extensions:
    extensions.append("breathe")
if ENABLE_GITHUB_ISSUES and issues_github_path and "sphinx_issues" not in extensions:
    extensions.append("sphinx_issues")
if ENABLE_SITEMAP and not LOCAL_BUILD and HTML_BASE_URL and "sphinx_sitemap" not in extensions:
    extensions.append("sphinx_sitemap")
exclude_patterns = EXCLUDE_PATTERNS.copy()
breathe_projects: dict[str, str] = {}
if DOXYGEN_ENABLED:
    breathe_projects = {DOXYGEN_PROJECT_NAME: str(generate_doxygen_xml())}
    breathe_default_project = DOXYGEN_PROJECT_NAME
    breathe_domain_by_extension = {
        "h": "cpp",
        "hpp": "cpp",
        "cpp": "cpp",
        "cppm": "cpp",
    }

# -- Options for HTML output -------------------------------------------------
html_theme = HTML_THEME
html_static_path = [path_name.as_posix() for path_name in HTML_STATIC_PATHS]
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
