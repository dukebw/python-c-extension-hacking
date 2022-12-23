from setuptools import setup, find_packages, Extension


setup(
    name="fib",
    version="0.1.0",
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
        Extension(
            # the qualified name of the extension module to build
            "fib.fib",
            # the files to compile into our module relative to ``setup.py``
            ["src/fib.c"],
        ),
    ],
)
