from distutils.core import *
setup(
    name = "BBearPythonModule",                                                 # Packed file name
    version = "1.0",
    ext_modules = [Extension("BBPythonModule", ["BBPythonModule.c"])]           # module name and its code
)