import os
from pathlib import Path

import yaml
import subprocess


def get_git_tag() -> str:
    """
    Return the most recent Git tag.

    :return Git tag
    """
    try:
        return subprocess.check_output(
            ["git", "describe", "--tags", "--abbrev=0"],
            text=True,
            stderr=subprocess.DEVNULL,
        ).strip()
    except Exception:
        return "0.0.0"


# -- Project information -----------------------------------------------------
project = "MAG - Mathématiques et Graphiques"
author = "Marcus Gigandet"
copyright = f"2026, {author}"

release = get_git_tag()
version = ".".join(release.split(".")[:2])

# -- Internationalization ----------------------------------------------------
locale_dirs = ["locale/"]
gettext_compact = False
gettext_uuid = True

# -- Project information ----------------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

project = 'MAG - Mathématiques et Graphiques'
copyright = '2026, Marcus Gigandet'
author = 'Marcus Gigandet'

# -- General configuration ----------------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

templates_path = ['_templates']
extensions = [
    # Spinx extensions
    'sphinx.ext.githubpages',
    "sphinx.ext.intersphinx",

    # External extensions
    'sphinx_design',
    'sphinx_issues',
    'sphinx_last_updated_by_git',
    'sphinx_multiversion',
    'sphinx_sitemap',
]
exclude_patterns = [
    '_build',
    'Thumbs.db',
    '.DS_Store',
]

# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_theme = 'sphinx_rtd_theme'
html_static_path = ['_static']

# -- Options for sphinx_last_updated_by_git ----------------
# https://pypi.org/project/sphinx-last-updated-by-git/0.3.8/

git_untracked_check_dependencies = True
git_untracked_show_sourcelink = True
git_last_updated_metatags = True

# -- Options for sphinx_issues extension -------------------------------
# https://github.com/sloria/sphinx-issues#installation-and-configuration

issues_github_path = "marcusgigandet/mag"

# -- Options for todo ---------------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/extensions/todo.html#configuration

todo_include_todos = True
todo_emit_warnings = False
todo_link_only = False

# -- Config for sphinx_sitemap ------------------------------------------------
# https://sphinx-sitemap.readthedocs.io/en/latest/

html_baseurl = "https://marcusgigandet.github.io/mag/"
site_baseurl = html_baseurl.rstrip("/")

# Logic for building multiple version tags and languages
build_all_docs = True
if build_all_docs:
    current_language = os.getenv("current_language", "en")
    current_version = os.getenv("current_version", "latest")
    language = current_language

    html_context = {
        'current_language': current_language,
        'languages': [],
        'current_version': current_version,
        'versions': [],
    }

    if current_version == 'latest':
        html_context['languages'].append(['en', site_baseurl])
        html_context['languages'].append(['fr', f"{site_baseurl}/fr"])

    if current_language == 'en':
        html_context['versions'].append(['latest', site_baseurl])
    if current_language == 'fr':
        html_context['versions'].append(['latest', f"{site_baseurl}/fr"])

    versions_path = Path(__file__).resolve().parent / "versions.yaml"
    with versions_path.open("r", encoding="utf-8") as yaml_file:
        docs = yaml.safe_load(yaml_file)

    if current_version != 'latest':
        for language in docs[current_version].get('languages', []):
            html_context['languages'].append([language, f"{site_baseurl}/{current_version}/{language}"])

    for version, details in docs.items():
        html_context['versions'].append([version, f"{site_baseurl}/{version}/{current_language}"])
