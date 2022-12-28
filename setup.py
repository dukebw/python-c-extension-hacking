"""Install Python C extension by calling CMake from setuptools"""
from pathlib import Path

import cmake_build_extension
from setuptools import Extension, find_packages, setup

setup(
    name="fib",
    version="0.0.0",
    packages=find_packages(),
    license="Apache 2.0",
    classifiers=[
        "Development Status :: 3 - Alpha",
        "License :: OSI Approved :: Apache Software License",
        "Natural Language :: English",
        "Programming Language :: Python :: 3 :: Only",
        "Programming Language :: Python :: Implementation :: CPython",
    ],
    ext_modules=[
        cmake_build_extension.CMakeExtension(
            name="Fibonacci",
            install_prefix="fib",
            source_dir=str(Path(__file__).parent.absolute()),
            cmake_configure_options=[
                "-DBUILD_SHARED_LIBS=YES",
                "-DCMAKE_EXPORT_COMPILE_COMMANDS=YES",
            ],
        ),
    ],
    cmdclass={"build_ext": cmake_build_extension.BuildExtension},
)
