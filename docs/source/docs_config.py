import os
import subprocess
from pathlib import Path
from typing import Final, Iterable, TypedDict
from urllib.parse import urlparse

import yaml


class VersionDetails(TypedDict, total=False):
    tag: str
    languages: list[str]


SOURCE_DIR: Final[Path] = Path(__file__).resolve().parent
DOCS_DIR: Final[Path] = SOURCE_DIR.parent
DEFAULT_LANGUAGE: Final[str] = os.getenv("DOCS_DEFAULT_LANGUAGE", "en")


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
VERSIONS_YAML_PATH: Final[Path] = DOCS_DIR / "versions.yaml"


def load_versions(path: Path) -> dict[str, VersionDetails]:
    """
    Load version metadata from ``versions.yaml``.

    :param path: Path to the versions YAML file.
    :return: A mapping of version names to validated version metadata.
    """
    with path.open("r", encoding="utf-8") as yaml_file:
        raw_versions = yaml.safe_load(yaml_file) or {}

    if not isinstance(raw_versions, dict):
        raise TypeError(
            "versions.yaml must contain a mapping of version names to metadata."
        )

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

        versions[version_name] = {
            "tag": tag,
            "languages": languages,
        }

    return versions


def normalize_languages(
    languages: Iterable[str], default_language: str = DEFAULT_LANGUAGE
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
    default_language: str = DEFAULT_LANGUAGE,
) -> list[str]:
    """
    Discover the languages supported by a docs tree.

    :param docs_dir: Documentation root that contains the Sphinx source tree.
    :param versions: Optional versions metadata whose language lists should also be included.
    :param default_language: Language code that should be treated as the root language.
    :return: Normalized language codes supported by the docs tree.
    """
    languages = {default_language}
    locale_dir = docs_dir / "source" / "locale"
    if locale_dir.exists():
        for candidate in locale_dir.iterdir():
            if candidate.is_dir() and (candidate / "LC_MESSAGES").exists():
                languages.add(candidate.name)

    if versions is not None:
        for details in versions.values():
            languages.update(details.get("languages", []))

    return normalize_languages(languages, default_language=default_language)


def resolve_repository_url(repo_root: Path) -> str:
    """
    Resolve a repository URL suitable for docs metadata.

    :param repo_root: Repository root directory.
    :return: Repository URL, or an empty string when it cannot be determined.
    """
    configured_url = os.getenv("DOCS_REPOSITORY_URL")
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
        return f"https://{parsed_url.hostname}{parsed_url.path.removesuffix('.git')}".rstrip(
            "/"
        )

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


def resolve_remote_html_base_url(repository_url: str) -> str:
    """
    Resolve the deployable HTML base URL for the docs site.

    :param repository_url: Repository URL associated with the docs site.
    :return: HTML base URL, or an empty string when it cannot be derived.
    """
    configured_url = os.getenv("DOCS_HTML_BASE_URL")
    if configured_url:
        return configured_url.rstrip("/") + "/"

    github_repo_path = resolve_github_repo_path(repository_url)
    if not github_repo_path:
        return ""

    owner, repo = github_repo_path.split("/", maxsplit=1)
    return f"https://{owner}.github.io/{repo}/"
