import os
import sys

sys.path.insert(0, os.path.abspath('../modules/'))

# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

project = ''
copyright = '2024, Nils Hoffmann'
author = 'Nils Hoffmann'
release = ''

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

extensions = [
    'sphinx.ext.autodoc',
    'sphinx.ext.napoleon',  # For Google/NumPy style docstrings
]

templates_path = ['_templates']
exclude_patterns = []



# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_theme = 'classic'
html_static_path = ['_static']
html_use_index = False
html_show_sourcelink = False
master_doc = 'index'
html_title = "ORB-Python Documentation"
# Disable module index in LaTeX output
latex_domain_indices = False

# Optionally, disable all indices, including the module index
html_domain_indices = False
latex_use_index = False

autodoc_default_options = {
    'members':         True,
    'member-order':    'bysource',
    'special-members': '__init__',
}