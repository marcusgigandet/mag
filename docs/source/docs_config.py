import os
import subprocess
from dataclasses import dataclass
from functools import lru_cache
from pathlib import Path
from typing import Final, Iterable, TypedDict
from urllib.parse import urlparse

import yaml


class VersionDetails(TypedDict, total=False):
    tag: str
    title: str
    status: str
    languages: list[str]


class DoxygenDetails(TypedDict, total=False):
    enabled: bool
    executable: str
    project_name: str
    input_paths: list[str]
    file_patterns: list[str]
    exclude_patterns: list[str]
    strip_from_path: str
    recursive: bool


class SiteDetails(TypedDict, total=False):
    default_language: str
    languages: list[str]
    project_name: str
    project_author: str
    latest_ref: str
    repository_url: str
    html_base_url: str
    issues_github_path: str
    docs_source_dir: str
    build_dir: str
    pages_dir: str
    local_pages_dir: str
    locale_dir: str
    html_theme: str
    html_static_path: list[str]
    extensions: list[str]
    exclude_patterns: list[str]
    enable_sitemap: bool
    enable_github_issues: bool
    doxygen: DoxygenDetails


@dataclass(frozen=True)
class DocsConfig:
    default_language: str
    languages: list[str]
    project_name: str
    project_author: str
    latest_ref: str
    repository_url: str
    html_base_url: str
    issues_github_path: str
    docs_source_dir: Path
    build_dir: Path
    pages_dir: Path
    local_pages_dir: Path
    locale_dir: Path
    html_theme: str
    html_static_path: list[Path]
    extensions: list[str]
    exclude_patterns: list[str]
    enable_sitemap: bool
    enable_github_issues: bool
    doxygen_enabled: bool
    doxygen_executable: str
    doxygen_project_name: str
    doxygen_input_paths: list[Path]
    doxygen_file_patterns: list[str]
    doxygen_exclude_patterns: list[str]
    doxygen_strip_from_path: Path
    doxygen_recursive: bool
    versions: dict[str, VersionDetails]


SOURCE_DIR: Final[Path] = Path(__file__).resolve().parent
DOCS_DIR: Final[Path] = SOURCE_DIR.parent


def run_git_command(args: list[str], cwd: Path) -> str | None:
    """
    Run a Git command and return stripped stdout when it succeeds.

    :param args: Git command arguments.
    :param cwd: Repository directory used as the command working directory.
    :return: Stripped stdout on success, otherwise ``None``.
    """
    try:
        output = subprocess.check_output(
            args, cwd=cwd, text=True, stderr=subprocess.DEVNULL
        ).strip()
    except (FileNotFoundError, subprocess.CalledProcessError):
        return None

    return output or None


def resolve_repo_root(start_dir: Path) -> Path:
    """
    Resolve the repository root containing the docs tree.

    :param start_dir: Directory from which repository discovery should begin.
    :return: The resolved repository root.
    """
    git_root = run_git_command(["git", "rev-parse", "--show-toplevel"], cwd=start_dir)
    if git_root is not None:
        return Path(git_root).resolve()

    for candidate in (start_dir, *start_dir.parents):
        if (candidate / ".git").exists():
            return candidate

    raise RuntimeError(f"Unable to resolve the Git repository root from '{start_dir}'.")


REPO_ROOT: Final[Path] = resolve_repo_root(DOCS_DIR)
DOCS_RELATIVE_PATH: Final[Path] = DOCS_DIR.relative_to(REPO_ROOT)
CONFIG_YAML_PATH: Final[Path] = DOCS_DIR / "config.yaml"


def load_raw_docs_yaml(path: Path) -> dict[str, object]:
    """
    Load the raw docs YAML payload.

    :param path: Path to the docs YAML file.
    :return: Parsed YAML mapping.
    """
    with path.open("r", encoding="utf-8") as yaml_file:
        raw_config = yaml.safe_load(yaml_file) or {}

    if not isinstance(raw_config, dict):
        raise TypeError("versions.yaml must contain a mapping.")

    return raw_config


def split_docs_yaml(raw_config: dict[str, object]) -> tuple[SiteDetails, dict[str, object]]:
    """
    Split docs YAML into site and version sections.

    :param raw_config: Parsed YAML mapping.
    :return: Site metadata and raw version metadata.
    """
    raw_site = raw_config.get("site")
    raw_versions = raw_config.get("versions")

    if raw_site is None and raw_versions is None:
        return {}, raw_config

    if raw_site is None:
        raw_site = {}
    if not isinstance(raw_site, dict):
        raise TypeError("versions.yaml 'site' section must be a mapping.")

    if raw_versions is None:
        raw_versions = {}
    if not isinstance(raw_versions, dict):
        raise TypeError("versions.yaml 'versions' section must be a mapping.")

    return raw_site, raw_versions


def validate_string_list(values: object, field_name: str) -> list[str]:
    """
    Validate that a config field is a list of strings.

    :param values: Raw config value.
    :param field_name: Fully-qualified config field name.
    :return: Validated list of strings.
    """
    if not isinstance(values, list) or not all(isinstance(value, str) for value in values):
        raise TypeError(f"{field_name} must be a list of strings.")

    return values


def validate_bool(value: object, field_name: str) -> bool:
    """
    Validate that a config field is a boolean.

    :param value: Raw config value.
    :param field_name: Fully-qualified config field name.
    :return: Validated boolean value.
    """
    if not isinstance(value, bool):
        raise TypeError(f"{field_name} must be a boolean.")

    return value


def validate_string(value: object, field_name: str) -> str:
    """
    Validate that a config field is a string.

    :param value: Raw config value.
    :param field_name: Fully-qualified config field name.
    :return: Validated string value.
    """
    if not isinstance(value, str):
        raise TypeError(f"{field_name} must be a string.")

    return value


def validate_relative_path(path_value: object, field_name: str) -> str:
    """
    Validate that a config field is a relative path string.

    :param path_value: Raw config value.
    :param field_name: Fully-qualified config field name.
    :return: Validated relative path string.
    """
    if not isinstance(path_value, str):
        raise TypeError(f"{field_name} must be a string.")
    if Path(path_value).is_absolute():
        raise TypeError(f"{field_name} must be a relative path.")

    return path_value


def validate_relative_path_list(values: object, field_name: str) -> list[str]:
    """
    Validate that a config field is a list of relative path strings.

    :param values: Raw config value.
    :param field_name: Fully-qualified config field name.
    :return: Validated list of relative path strings.
    """
    path_values = validate_string_list(values, field_name)
    return [
        validate_relative_path(path_value, f"{field_name}[{index}]")
        for index, path_value in enumerate(path_values)
    ]


def validate_doxygen_details(raw_doxygen: object) -> DoxygenDetails:
    """
    Validate site-level Doxygen configuration.

    :param raw_doxygen: Raw Doxygen metadata loaded from YAML.
    :return: Validated Doxygen metadata.
    """
    if not isinstance(raw_doxygen, dict):
        raise TypeError("versions.yaml site.doxygen must be a mapping.")

    validated: DoxygenDetails = {}

    if raw_doxygen.get("enabled") is not None:
        validated["enabled"] = validate_bool(
            raw_doxygen["enabled"], "versions.yaml site.doxygen.enabled"
        )
    if raw_doxygen.get("recursive") is not None:
        validated["recursive"] = validate_bool(
            raw_doxygen["recursive"], "versions.yaml site.doxygen.recursive"
        )

    for field_name in ("executable", "project_name"):
        field_value = raw_doxygen.get(field_name)
        if field_value is None:
            continue
        validated[field_name] = validate_string(
            field_value, f"versions.yaml site.doxygen.{field_name}"
        )

    if raw_doxygen.get("input_paths") is not None:
        validated["input_paths"] = validate_relative_path_list(
            raw_doxygen["input_paths"], "versions.yaml site.doxygen.input_paths"
        )

    for field_name in ("file_patterns", "exclude_patterns"):
        field_value = raw_doxygen.get(field_name)
        if field_value is None:
            continue
        validated[field_name] = validate_string_list(
            field_value, f"versions.yaml site.doxygen.{field_name}"
        )

    if raw_doxygen.get("strip_from_path") is not None:
        validated["strip_from_path"] = validate_relative_path(
            raw_doxygen["strip_from_path"],
            "versions.yaml site.doxygen.strip_from_path",
        )

    return validated


def validate_site_details(raw_site: SiteDetails) -> SiteDetails:
    """
    Validate site-level docs configuration.

    :param raw_site: Raw site metadata loaded from YAML.
    :return: Validated site metadata.
    """
    validated: SiteDetails = {}

    string_fields = (
        "default_language",
        "project_name",
        "project_author",
        "latest_ref",
        "repository_url",
        "html_base_url",
        "issues_github_path",
        "html_theme",
    )
    for field_name in string_fields:
        field_value = raw_site.get(field_name)
        if field_value is None:
            continue
        if not isinstance(field_value, str):
            raise TypeError(f"versions.yaml site.{field_name} must be a string.")
        validated[field_name] = field_value

    if raw_site.get("languages") is not None:
        validated["languages"] = validate_string_list(
            raw_site["languages"], "versions.yaml site.languages"
        )

    for field_name in (
        "docs_source_dir",
        "build_dir",
        "pages_dir",
        "local_pages_dir",
        "locale_dir",
    ):
        field_value = raw_site.get(field_name)
        if field_value is None:
            continue
        validated[field_name] = validate_relative_path(
            field_value, f"versions.yaml site.{field_name}"
        )

    for field_name in ("html_static_path", "extensions", "exclude_patterns"):
        field_value = raw_site.get(field_name)
        if field_value is None:
            continue
        validated[field_name] = validate_string_list(
            field_value, f"versions.yaml site.{field_name}"
        )

    for field_name in ("enable_sitemap", "enable_github_issues"):
        field_value = raw_site.get(field_name)
        if field_value is None:
            continue
        validated[field_name] = validate_bool(
            field_value, f"versions.yaml site.{field_name}"
        )

    if raw_site.get("doxygen") is not None:
        validated["doxygen"] = validate_doxygen_details(raw_site["doxygen"])

    return validated


def load_versions(path: Path) -> dict[str, VersionDetails]:
    """
    Load version metadata from ``versions.yaml``.

    :param path: Path to the versions YAML file.
    :return: A mapping of version names to validated version metadata.
    """
    _, raw_versions = split_docs_yaml(load_raw_docs_yaml(path))

    versions: dict[str, VersionDetails] = {}
    for version_name, details in raw_versions.items():
        if not isinstance(version_name, str):
            raise TypeError("versions.yaml version keys must be strings.")
        if not isinstance(details, dict):
            raise TypeError(f"Version '{version_name}' metadata must be a mapping.")

        languages = details.get("languages", [])
        if not isinstance(languages, list) or not all(
            isinstance(language_name, str) for language_name in languages
        ):
            raise TypeError(
                f"Version '{version_name}' languages must be a list of strings."
            )

        tag = details.get("tag", version_name)
        if not isinstance(tag, str):
            raise TypeError(f"Version '{version_name}' tag must be a string.")

        title = details.get("title", version_name)
        if not isinstance(title, str):
            raise TypeError(f"Version '{version_name}' title must be a string.")

        status = details.get("status", "")
        if not isinstance(status, str):
            raise TypeError(f"Version '{version_name}' status must be a string.")

        versions[version_name] = {
            "tag": tag,
            "title": title,
            "status": status,
            "languages": languages,
        }

    return versions


def normalize_languages(
    languages: Iterable[str], default_language: str
) -> list[str]:
    """
    Normalize a language list with the default language first.

    :param languages: Iterable of language codes.
    :param default_language: Language code that should be treated as the root language.
    :return: A deduplicated language list ordered for navigation and build output.
    """
    other_languages = sorted(
        {
            language_name
            for language_name in languages
            if language_name != default_language
        }
    )
    return [default_language, *other_languages]


def discover_languages(
    docs_dir: Path,
    versions: dict[str, VersionDetails] | None = None,
    default_language: str | None = None,
    configured_languages: list[str] | None = None,
    docs_source_dir: Path | None = None,
    locale_dir: Path | None = None,
) -> list[str]:
    """
    Discover the languages supported by a docs tree.

    :param docs_dir: Documentation root that contains the Sphinx source tree.
    :param versions: Optional versions metadata whose language lists should also be included.
    :param default_language: Language code that should be treated as the root language.
    :param configured_languages: Explicit language list from configuration.
    :param docs_source_dir: Source directory relative to ``docs_dir``.
    :param locale_dir: Locale directory relative to the docs source directory.
    :return: Normalized language codes supported by the docs tree.
    """
    if default_language is None:
        default_language = DEFAULT_LANGUAGE
    if docs_source_dir is None:
        docs_source_dir = DOCS_SOURCE_RELATIVE_PATH
    if locale_dir is None:
        locale_dir = LOCALE_RELATIVE_PATH

    if configured_languages:
        return normalize_languages(
            configured_languages, default_language=default_language
        )

    languages = {default_language}
    locale_root = docs_dir / docs_source_dir / locale_dir
    if locale_root.exists():
        for candidate in locale_root.iterdir():
            if candidate.is_dir() and (candidate / "LC_MESSAGES").exists():
                languages.add(candidate.name)

    if versions is not None:
        for details in versions.values():
            languages.update(details.get("languages", []))

    return normalize_languages(languages, default_language=default_language)


def resolve_repository_url(repo_root: Path, configured_url: str = "") -> str:
    """
    Resolve a repository URL suitable for docs metadata.

    :param repo_root: Repository root directory.
    :param configured_url: Explicit repository URL from configuration.
    :return: Repository URL, or an empty string when it cannot be determined.
    """
    env_url = os.getenv("DOCS_REPOSITORY_URL")
    if env_url:
        return env_url.rstrip("/")
    if configured_url:
        return configured_url.rstrip("/")

    remote_url = run_git_command(["git", "remote", "get-url", "origin"], cwd=repo_root)
    if remote_url is None:
        return ""

    if remote_url.startswith("git@"):
        host_and_path = remote_url.split("@", maxsplit=1)[1]
        host, repo_path = host_and_path.split(":", maxsplit=1)
        return f"https://{host}/{repo_path.removesuffix('.git')}"

    parsed_url = urlparse(remote_url)
    if parsed_url.hostname and parsed_url.path:
        return (
            f"https://{parsed_url.hostname}{parsed_url.path.removesuffix('.git')}"
        ).rstrip("/")

    return remote_url.rstrip("/")


def resolve_github_repo_path(repository_url: str) -> str:
    """
    Resolve the ``owner/repo`` path for GitHub repositories.

    :param repository_url: Repository URL to inspect.
    :return: GitHub repository path, or an empty string when the URL is not a GitHub repository.
    """
    if not repository_url:
        return ""

    parsed_url = urlparse(repository_url)
    if parsed_url.hostname != "github.com":
        return ""

    path_parts = [part for part in parsed_url.path.split("/") if part]
    if len(path_parts) < 2:
        return ""

    owner, repo = path_parts[:2]
    return f"{owner}/{repo.removesuffix('.git')}"


def resolve_remote_html_base_url(
    repository_url: str, configured_url: str = ""
) -> str:
    """
    Resolve the deployable HTML base URL for the docs site.

    :param repository_url: Repository URL associated with the docs site.
    :param configured_url: Explicit HTML base URL from configuration.
    :return: HTML base URL, or an empty string when it cannot be derived.
    """
    env_url = os.getenv("DOCS_HTML_BASE_URL")
    if env_url:
        return env_url.rstrip("/") + "/"
    if configured_url:
        return configured_url.rstrip("/") + "/"

    github_repo_path = resolve_github_repo_path(repository_url)
    if not github_repo_path:
        return ""

    owner, repo = github_repo_path.split("/", maxsplit=1)
    return f"https://{owner}.github.io/{repo}/"


def resolve_issues_github_path(
    repository_url: str, configured_path: str = ""
) -> str:
    """
    Resolve the GitHub repository path used by ``sphinx_issues``.

    :param repository_url: Repository URL associated with the docs site.
    :param configured_path: Explicit GitHub repository path from configuration.
    :return: ``owner/repo`` path, or an empty string when it cannot be determined.
    """
    env_path = os.getenv("DOCS_ISSUES_GITHUB_PATH")
    if env_path:
        return env_path
    if configured_path:
        return configured_path

    return resolve_github_repo_path(repository_url)


def as_relative_path(configured_path: str, default_path: str) -> Path:
    """
    Resolve a docs config path value into a relative ``Path``.

    :param configured_path: Configured path value.
    :param default_path: Default path value.
    :return: Relative path.
    """
    return Path(configured_path or default_path)


@lru_cache(maxsize=None)
def load_docs_config(path: Path, repo_root: Path) -> DocsConfig:
    """
    Load and resolve the full docs configuration from YAML plus runtime defaults.

    :param path: Path to the docs YAML file.
    :param repo_root: Repository root used for derived defaults.
    :return: Resolved docs configuration.
    """
    raw_site, _ = split_docs_yaml(load_raw_docs_yaml(path))
    site = validate_site_details(raw_site)
    versions = load_versions(path)

    default_language = os.getenv(
        "DOCS_DEFAULT_LANGUAGE", site.get("default_language", "en")
    )
    configured_languages = site.get("languages", [])
    languages = (
        normalize_languages(configured_languages, default_language=default_language)
        if configured_languages
        else []
    )
    repository_url = resolve_repository_url(
        repo_root, configured_url=site.get("repository_url", "")
    )
    html_base_url = resolve_remote_html_base_url(
        repository_url, configured_url=site.get("html_base_url", "")
    )
    issues_github_path = resolve_issues_github_path(
        repository_url, configured_path=site.get("issues_github_path", "")
    )
    docs_source_dir = as_relative_path(
        site.get("docs_source_dir", ""), SOURCE_DIR.name
    )
    build_dir = as_relative_path(site.get("build_dir", ""), "_build")
    pages_dir = as_relative_path(site.get("pages_dir", ""), "pages")
    local_pages_dir = as_relative_path(
        site.get("local_pages_dir", ""), "pages-local"
    )
    locale_dir = as_relative_path(site.get("locale_dir", ""), "locale")
    doxygen = site.get("doxygen", {})

    html_static_path = [
        Path(path_name)
        for path_name in site.get("html_static_path", ["_static"])
    ]
    extensions = site.get(
        "extensions",
        [
            "sphinx.ext.githubpages",
            "sphinx.ext.intersphinx",
            "breathe",
            "sphinx_copybutton",
            "sphinx_design",
            "sphinx_last_updated_by_git",
            "sphinx_multiversion",
        ],
    )
    exclude_patterns = site.get(
        "exclude_patterns", [build_dir.as_posix(), "Thumbs.db", ".DS_Store"]
    )
    resolved_project_name = os.getenv(
        "DOCS_PROJECT_NAME", site.get("project_name", repo_root.name)
    )

    return DocsConfig(
        default_language=default_language,
        languages=languages,
        project_name=resolved_project_name,
        project_author=os.getenv(
            "DOCS_PROJECT_AUTHOR",
            site.get(
                "project_author",
                run_git_command(["git", "config", "user.name"], cwd=repo_root) or "",
            ),
        ),
        latest_ref=os.getenv("DOCS_LATEST_REF", site.get("latest_ref", "")).strip(),
        repository_url=repository_url,
        html_base_url=html_base_url,
        issues_github_path=issues_github_path,
        docs_source_dir=docs_source_dir,
        build_dir=build_dir,
        pages_dir=pages_dir,
        local_pages_dir=local_pages_dir,
        locale_dir=locale_dir,
        html_theme=site.get("html_theme", "sphinx_rtd_theme"),
        html_static_path=html_static_path,
        extensions=extensions,
        exclude_patterns=exclude_patterns,
        enable_sitemap=site.get("enable_sitemap", True),
        enable_github_issues=site.get("enable_github_issues", True),
        doxygen_enabled=doxygen.get("enabled", True),
        doxygen_executable=doxygen.get("executable", "doxygen"),
        doxygen_project_name=doxygen.get("project_name", resolved_project_name),
        doxygen_input_paths=[
            Path(path_name) for path_name in doxygen.get("input_paths", ["src"])
        ],
        doxygen_file_patterns=doxygen.get(
            "file_patterns", ["*.h", "*.hpp", "*.cpp", "*.cppm"]
        ),
        doxygen_exclude_patterns=doxygen.get("exclude_patterns", []),
        doxygen_strip_from_path=as_relative_path(
            doxygen.get("strip_from_path", ""), "."
        ),
        doxygen_recursive=doxygen.get("recursive", True),
        versions=versions,
    )


DOCS_CONFIG: Final[DocsConfig] = load_docs_config(CONFIG_YAML_PATH, REPO_ROOT)
DEFAULT_LANGUAGE: Final[str] = DOCS_CONFIG.default_language
SITE_LANGUAGES: Final[list[str]] = DOCS_CONFIG.languages
PROJECT_NAME: Final[str] = DOCS_CONFIG.project_name
PROJECT_AUTHOR: Final[str] = DOCS_CONFIG.project_author
LATEST_REF: Final[str] = DOCS_CONFIG.latest_ref
REPOSITORY_URL: Final[str] = DOCS_CONFIG.repository_url
REMOTE_HTML_BASE_URL: Final[str] = DOCS_CONFIG.html_base_url
ISSUES_GITHUB_PATH: Final[str] = DOCS_CONFIG.issues_github_path
DOCS_SOURCE_RELATIVE_PATH: Final[Path] = DOCS_CONFIG.docs_source_dir
BUILD_RELATIVE_PATH: Final[Path] = DOCS_CONFIG.build_dir
BUILD_HTML_RELATIVE_PATH: Final[Path] = BUILD_RELATIVE_PATH / "html"
PAGES_RELATIVE_PATH: Final[Path] = DOCS_CONFIG.pages_dir
LOCAL_PAGES_RELATIVE_PATH: Final[Path] = DOCS_CONFIG.local_pages_dir
LOCALE_RELATIVE_PATH: Final[Path] = DOCS_CONFIG.locale_dir
HTML_THEME: Final[str] = DOCS_CONFIG.html_theme
HTML_STATIC_PATHS: Final[list[Path]] = DOCS_CONFIG.html_static_path
SPHINX_EXTENSIONS: Final[list[str]] = DOCS_CONFIG.extensions
EXCLUDE_PATTERNS: Final[list[str]] = DOCS_CONFIG.exclude_patterns
ENABLE_SITEMAP: Final[bool] = DOCS_CONFIG.enable_sitemap
ENABLE_GITHUB_ISSUES: Final[bool] = DOCS_CONFIG.enable_github_issues
DOXYGEN_ENABLED: Final[bool] = DOCS_CONFIG.doxygen_enabled
DOXYGEN_EXECUTABLE: Final[str] = DOCS_CONFIG.doxygen_executable
DOXYGEN_PROJECT_NAME: Final[str] = DOCS_CONFIG.doxygen_project_name
DOXYGEN_INPUT_PATHS: Final[list[Path]] = DOCS_CONFIG.doxygen_input_paths
DOXYGEN_FILE_PATTERNS: Final[list[str]] = DOCS_CONFIG.doxygen_file_patterns
DOXYGEN_EXCLUDE_PATTERNS: Final[list[str]] = DOCS_CONFIG.doxygen_exclude_patterns
DOXYGEN_STRIP_FROM_PATH: Final[Path] = DOCS_CONFIG.doxygen_strip_from_path
DOXYGEN_RECURSIVE: Final[bool] = DOCS_CONFIG.doxygen_recursive
