# distutils: language=C++

from distutils.core import setup
from Cython.Build import cythonize

setup(ext_modules = cythonize("sentiment_analysis.pyx", language="C++"))
