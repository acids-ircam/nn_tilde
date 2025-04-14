import os

import setuptools

VERSION = os.environ["NN_TILDE_VERSION"]

with open("README.md", "r") as readme:
    readme = readme.read()

with open("requirements.txt", "r") as requirements:
    requirements = requirements.read()

setuptools.setup(
    name="nn_tilde",
    version=VERSION,
    author="Antoine CAILLON & Axel CHEMLA--ROMEU-SANTOS",
    author_email="chemla@ircam.fr",
    description="Set of tools to create nn_tilde compatible models.",
    long_description=readme,
    long_description_content_type="text/markdown",
    packages=['nn_tilde'],
    package_dir={'nn_tilde': 'python_tools'},
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: MIT License",
        "Operating System :: OS Independent",
    ],
    install_requires=requirements.split("\n"),
    python_requires='>=3.7',
)
