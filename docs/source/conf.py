import yaml
import os
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
language = 'fr'
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

templates_path = ['_templates']

# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_theme = 'sphinx_rtd_theme'
html_static_path = ['_static']

html_theme_options = {
}

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

build_all_docs = True
pages_root = "https://marcusgigandet.github.io/mag/"
if build_all_docs:
    current_language = os.environ.get("current_language", "fr")
    current_version = os.environ.get("current_version", "0.1.0")

    html_context = {
        'current_language' : current_language,
        'languages' : [],
        'current_version' : current_version,
        'versions' : [],
    }

    if current_version == 'latest':
        html_context['languages'].append(['en', pages_root])
        html_context['languages'].append(['fr', pages_root+'/fr'])

    if current_language == 'en':
        html_context['versions'].append(['latest', pages_root])
    if current_language == 'fr':
        html_context['versions'].append(['latest', pages_root+'/fr'])

    with open("versions.yaml", "r") as yaml_file:
        docs = yaml.safe_load(yaml_file)

    if current_version != 'latest':
        for language in docs[current_version].get('languages', []):
            html_context['languages'].append([language, pages_root+'/'+current_version+'/'+language])

    for version, details in docs.items():
        html_context['versions'].append([version, pages_root+'/'+version+'/'+current_language])
