import os
import shutil
import subprocess
import tempfile

import yaml

from pathlib import Path
from typing import Final

DOCS_DIR: Final[Path] = Path(__file__).resolve().parent.parent
PAGES_DIR: Final[Path] = DOCS_DIR / "pages"
CONF_PY_PATH: Final[Path] = DOCS_DIR / "source" / "conf.py"
VERSIONS_YAML_PATH: Final[Path] = DOCS_DIR / "source" / "versions.yaml"
ORIGINAL_REF = subprocess.check_output(
    ["git", "rev-parse", "--abbrev-ref", "HEAD"],
    cwd=DOCS_DIR,
    text=True,
).strip()

if ORIGINAL_REF == "HEAD":
    ORIGINAL_REF = subprocess.check_output(
        ["git", "rev-parse", "HEAD"],
        cwd=DOCS_DIR,
        text=True,
    ).strip()


def restore_docs_files(conf_backup, versions_backup) -> None:
    shutil.copy2(conf_backup, CONF_PY_PATH)
    shutil.copy2(versions_backup, VERSIONS_YAML_PATH)


def build_doc(version, language, tag, conf_backup, versions_backup) -> None:
    os.environ["current_version"] = version
    os.environ["current_language"] = language
    # subprocess.run(["git", "checkout", tag], cwd=DOCS_DIR, check=True)
    restore_docs_files(conf_backup, versions_backup)
    os.environ["SPHINXOPTS"] = f"-D language={language}"
    subprocess.run(["make", "html"], cwd=DOCS_DIR, check=True)


def move_dir(src_dir: Path, dst_dir: Path) -> None:
    dst_dir.mkdir(parents=True, exist_ok=True)
    for item in src_dir.iterdir():
        target = dst_dir / item.name
        if target.exists():
            if target.is_dir():
                shutil.rmtree(target)
            else:
                target.unlink()
        shutil.move(str(item), str(target))

try:
    with tempfile.TemporaryDirectory() as temp_dir:
        temp_dir = Path(temp_dir)
        conf_backup = temp_dir / "conf.py"
        versions_backup = temp_dir / "versions.yaml"
        shutil.copy2(CONF_PY_PATH, conf_backup)
        shutil.copy2(VERSIONS_YAML_PATH, versions_backup)

        # Build English docs
        build_doc("latest", "en", "add-docs-page", conf_backup, versions_backup)
        move_dir(DOCS_DIR / "_build/html", PAGES_DIR)

        # Build French docs
        build_doc("latest", "fr", "add-docs-page", conf_backup, versions_backup)
        move_dir(DOCS_DIR / "_build/html", PAGES_DIR / "fr")

        with open(VERSIONS_YAML_PATH, "r", encoding="utf-8") as yaml_file:
            docs = yaml.safe_load(yaml_file)

        for version, details in docs.items():
            tag = details.get("tag", version)
            for language in details.get("languages", []):
                build_doc(version, language, tag, conf_backup, versions_backup)
                move_dir(DOCS_DIR / "_build/html", PAGES_DIR / version / language)
finally:
    subprocess.run(
        ["git", "checkout", ORIGINAL_REF],
        cwd=DOCS_DIR,
        check=True
    )
