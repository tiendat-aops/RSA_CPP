from setuptools import setup, Extension
from pybind11.setup_helpers import Pybind11Extension, build_ext

ext_modules = [
    Pybind11Extension(
        "rsa_uet",  # Tên thư viện
        ["wrapper.cpp"],  # File wrapper
    ),
]

setup(
    name="rsa_uet",
    version="0.1",
    author="datnt",
    description="Python binding for RSA method in BigNumber",
    ext_modules=ext_modules,
    cmdclass={"build_ext": build_ext},
    zip_safe=False,
)
