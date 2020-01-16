import numpy

from distutils.core import setup, Extension
from Cython.Build import cythonize

extensions = [
    Extension(
        "speedup",
        ["render/speedup.pyx"],
        include_dirs=[numpy.get_include()],
    )
]
setup(
    ext_modules=cythonize(
        extensions,
        annotate=True,
        compiler_directives={"linetrace": True, "binding": True},
    )
)
