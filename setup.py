from setuptools import setup, Extension
from pybind11.setup_helpers import Pybind11Extension

ext_modules = [
    Pybind11Extension(
        "multiset_avl",
        ["multiset_avl.cpp"],
        cxx_std=14,
    ),
]

setup(
    name="multiset_avl",
    ext_modules=ext_modules,
    install_requires=["pybind11>=2.6.0"],
    python_requires=">=3.6",
)