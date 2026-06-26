import argparse
import os
import shutil
import subprocess
import tempfile
from contextlib import ExitStack, contextmanager
from dataclasses import dataclass
from pathlib import Path
from typing import Final, Iterator

from docs_config import (
    DEFAULT_LANGUAGE,
    DOCS_DIR,
    DOCS_RELATIVE_PATH,
    LATEST_REF,
    REPO_ROOT,
    SITE_LANGUAGES,
    VERSIONS_YAML_PATH,
    VersionDetails,
    discover_languages,
    load_versions,
)


@dataclass(frozen=True)
class BuildTarget:
    version: str
    language: str
    destination: Path
    docs_dir: Path


@dataclass(frozen=True)
class BuildPlan:
    output_dir: Path
    targets: list[BuildTarget]


PAGES_DIR: Final[Path] = DOCS_DIR / "pages"
LOCAL_PAGES_DIR: Final[Path] = DOCS_DIR / "pages-local"
VENV_PYTHON: Final[Path] = DOCS_DIR / ".venv" / "bin" / "python"


def parse_args() -> argparse.Namespace:
    """
    Parse command-line arguments for the docs build script.

    :return: Parsed command-line arguments.
    """
    parser = argparse.ArgumentParser(
        description="Build versioned Sphinx documentation."
    )
    parser.add_argument(
        "--local",
        action="store_true",
        help="Build a local preview from the current checkout with only local relative links.",
    )
    return parser.parse_args()


def git_ref_exists(ref: str) -> bool:
    """
    Check whether a Git ref exists in the repository.

    :param ref: Git ref name to check.
    :return: ``True`` when the ref exists, otherwise ``False``.
    """
    return (
        subprocess.run(
            ["git", "rev-parse", "--verify", ref],
            cwd=REPO_ROOT,
            stdout=subprocess.DEVNULL,
            stderr=subprocess.DEVNULL,
            check=False,
        ).returncode
        == 0
    )


def resolve_latest_ref() -> str:
    """
    Resolve the Git ref that should back the deployable ``latest`` docs.

    :return: ``origin/main`` when available, otherwise ``main``.
    """
    configured_ref = LATEST_REF
    if configured_ref:
        if git_ref_exists(configured_ref):
            return configured_ref
        raise RuntimeError(f"Configured latest ref '{configured_ref}' does not exist.")

    for command in (
        ["git", "symbolic-ref", "--quiet", "--short", "refs/remotes/origin/HEAD"],
        ["git", "symbolic-ref", "--quiet", "--short", "HEAD"],
    ):
        resolved_ref = subprocess.run(
            command,
            cwd=REPO_ROOT,
            text=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.DEVNULL,
            check=False,
        ).stdout.strip()
        if resolved_ref and git_ref_exists(resolved_ref):
            return resolved_ref

    for fallback_ref in ("origin/main", "main", "origin/master", "master"):
        if git_ref_exists(fallback_ref):
            return fallback_ref

    raise RuntimeError(
        "Unable to resolve the latest docs ref automatically. Set site.latest_ref or DOCS_LATEST_REF explicitly."
    )


@contextmanager
def temporary_worktree(ref: str, temp_dir: Path) -> Iterator[Path]:
    """
    Create a temporary Git worktree for a ref and clean it up afterward.

    :param ref: Git ref to check out in the worktree.
    :param temp_dir: Temporary directory that should contain the worktree.
    :return: The filesystem path to the temporary worktree.
    """
    worktree_dir = temp_dir / ref.replace("/", "-")
    subprocess.run(
        ["git", "worktree", "add", "--detach", str(worktree_dir), ref],
        cwd=REPO_ROOT,
        check=True,
    )
    try:
        yield worktree_dir
    finally:
        subprocess.run(
            ["git", "worktree", "remove", "--force", str(worktree_dir)],
            cwd=REPO_ROOT,
            check=True,
        )


def build_local_plan() -> BuildPlan:
    """
    Build the target list for a local preview.

    :return: The local preview build plan.
    """
    languages = discover_languages(
        DOCS_DIR,
        default_language=DEFAULT_LANGUAGE,
        configured_languages=SITE_LANGUAGES,
    )
    return BuildPlan(
        output_dir=LOCAL_PAGES_DIR,
        targets=[
            BuildTarget(
                version="local",
                language=language_name,
                destination=(
                    LOCAL_PAGES_DIR
                    if language_name == DEFAULT_LANGUAGE
                    else LOCAL_PAGES_DIR / language_name
                ),
                docs_dir=DOCS_DIR,
            )
            for language_name in languages
        ],
    )


def build_remote_plan(
    versions: dict[str, VersionDetails],
    latest_docs_dir: Path,
    worktree_dirs: dict[str, Path],
) -> BuildPlan:
    """
    Build the target list for a deployable multi-version site.

    :param versions: Mapping of configured docs versions to their metadata.
    :param latest_docs_dir: Docs directory that should be used for the deployable ``latest`` docs.
    :param worktree_dirs: Mapping of Git refs to checked-out worktree directories.
    :return: The deployable docs build plan.
    """
    latest_languages = discover_languages(
        latest_docs_dir,
        default_language=DEFAULT_LANGUAGE,
        configured_languages=SITE_LANGUAGES,
    )
    targets = [
        BuildTarget(
            version="latest",
            language=language_name,
            destination=(
                PAGES_DIR
                if language_name == DEFAULT_LANGUAGE
                else PAGES_DIR / language_name
            ),
            docs_dir=latest_docs_dir,
        )
        for language_name in latest_languages
    ]

    for version_name, details in versions.items():
        ref = details.get("tag", version_name)
        docs_dir = worktree_dirs[ref] / DOCS_RELATIVE_PATH
        for language_name in details.get("languages", []):
            targets.append(
                BuildTarget(
                    version=version_name,
                    language=language_name,
                    destination=PAGES_DIR / version_name / language_name,
                    docs_dir=docs_dir,
                )
            )

    return BuildPlan(output_dir=PAGES_DIR, targets=targets)


def has_docs_source(docs_dir: Path) -> bool:
    """
    Check whether a docs directory contains the Sphinx source tree needed for HTML builds.

    :param docs_dir: Docs directory expected to contain the Sphinx source tree.
    :return: ``True`` when the docs source tree exists, otherwise ``False``.
    """
    return (docs_dir / "source" / "index.rst").exists()


def ensure_docs_source_exists(ref: str, docs_dir: Path) -> None:
    """
    Ensure a checked-out ref contains the docs source tree needed for HTML builds.

    :param ref: Git ref being validated.
    :param docs_dir: Docs directory expected to contain the Sphinx source tree.
    :return: ``None``.
    """
    if not has_docs_source(docs_dir):
        docs_source_path = DOCS_RELATIVE_PATH / "source" / "index.rst"
        raise RuntimeError(
            f"Git ref '{ref}' does not contain '{docs_source_path}', so it cannot back the deployable docs build."
        )


def resolve_latest_docs_dir(latest_ref: str, worktree_dirs: dict[str, Path]) -> Path:
    """
    Resolve the docs directory that should back the deployable ``latest`` docs.

    :param latest_ref: Preferred Git ref for the deployable ``latest`` docs.
    :param worktree_dirs: Mapping of Git refs to checked-out worktree directories.
    :return: Docs directory to use for the ``latest`` build.
    """
    latest_docs_dir = worktree_dirs[latest_ref] / DOCS_RELATIVE_PATH
    if has_docs_source(latest_docs_dir):
        return latest_docs_dir

    if has_docs_source(DOCS_DIR):
        docs_source_path = DOCS_RELATIVE_PATH / "source" / "index.rst"
        print(
            f"Falling back to the current checkout for latest because '{latest_ref}' does not contain '{docs_source_path}' yet."
        )
        return DOCS_DIR

    docs_source_path = DOCS_RELATIVE_PATH / "source" / "index.rst"
    raise RuntimeError(
        f"Neither '{latest_ref}' nor the current checkout contains '{docs_source_path}', so latest cannot be built."
    )


def filter_buildable_versions(
    versions: dict[str, VersionDetails],
    worktree_dirs: dict[str, Path],
) -> dict[str, VersionDetails]:
    """
    Filter out historical versions whose refs do not contain a buildable docs tree.

    :param versions: Mapping of configured docs versions to their metadata.
    :param worktree_dirs: Mapping of Git refs to checked-out worktree directories.
    :return: Only the version entries whose refs contain buildable docs sources.
    """
    buildable_versions: dict[str, VersionDetails] = {}

    for version_name, details in versions.items():
        ref = details.get("tag", version_name)
        if has_docs_source(worktree_dirs[ref] / DOCS_RELATIVE_PATH):
            buildable_versions[version_name] = details
            continue

        docs_source_path = DOCS_RELATIVE_PATH / "source" / "index.rst"
        print(
            f"Skipping docs version '{version_name}' from ref '{ref}' because it has no '{docs_source_path}'."
        )

    return buildable_versions


def build_doc(target: BuildTarget, local_build: bool) -> None:
    """
    Build a single docs target.

    :param target: The version/language/docs destination combination to build.
    :param local_build: Whether this build should use local-only navigation and metadata.
    :return: ``None``.
    """
    build_env = os.environ.copy()
    build_env["current_version"] = target.version
    build_env["current_language"] = target.language
    build_env["DOCS_LOCAL_BUILD"] = "1" if local_build else "0"

    source_dir = (target.docs_dir / "source").resolve()
    build_dir = (target.docs_dir / "_build" / "html").resolve()

    subprocess.run(
        [
            str(VENV_PYTHON),
            "-m",
            "sphinx",
            "-b",
            "html",
            str(source_dir),
            str(build_dir),
            "-q",
            "-D",
            f"language={target.language}",
        ],
        check=True,
        cwd=target.docs_dir.resolve(),
        env=build_env,
    )


def move_dir_contents(src_dir: Path, dst_dir: Path) -> None:
    """
    Replace a directory's contents with another directory's contents.

    :param src_dir: Source directory whose contents should be moved.
    :param dst_dir: Destination directory that should receive the moved contents.
    :return: ``None``.
    """
    dst_dir.mkdir(parents=True, exist_ok=True)

    for item in src_dir.iterdir():
        target = dst_dir / item.name
        if target.exists():
            if target.is_dir():
                shutil.rmtree(target)
            else:
                target.unlink()
        shutil.move(str(item), str(target))


def main() -> None:
    """
    Build every configured docs version and language into the pages output tree.

    :return: ``None``.
    """
    args = parse_args()
    versions = load_versions(VERSIONS_YAML_PATH)

    if args.local:
        plan = build_local_plan()
        shutil.rmtree(plan.output_dir, ignore_errors=True)
        for target in plan.targets:
            build_doc(target, local_build=True)
            move_dir_contents(target.docs_dir / "_build" / "html", target.destination)
        return

    with tempfile.TemporaryDirectory() as temp_dir_name, ExitStack() as stack:
        temp_dir = Path(temp_dir_name)
        latest_ref = resolve_latest_ref()
        refs = {latest_ref}
        refs.update(
            details.get("tag", version_name)
            for version_name, details in versions.items()
        )

        worktree_dirs: dict[str, Path] = {}
        for ref in sorted(refs):
            worktree_dirs[ref] = stack.enter_context(temporary_worktree(ref, temp_dir))
        latest_docs_dir = resolve_latest_docs_dir(latest_ref, worktree_dirs)

        buildable_versions = filter_buildable_versions(versions, worktree_dirs)
        plan = build_remote_plan(buildable_versions, latest_docs_dir, worktree_dirs)
        shutil.rmtree(plan.output_dir, ignore_errors=True)
        for target in plan.targets:
            build_doc(target, local_build=False)
            move_dir_contents(target.docs_dir / "_build" / "html", target.destination)


if __name__ == "__main__":
    main()
