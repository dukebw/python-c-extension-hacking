# python-c-extension-hacking

## CMake Setup

```shell
mkdir build
cd build
cmake .. -G Ninja                                 \
  -DBUILD_SHARED_LIBS=YES                         \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=YES             \
  -DPython3_INCLUDE_DIR=/path/to/python3/include
```


## Build Python Wheel

```shell
python -m build --wheel                                                               \
    "--config-setting=--build-option=build_ext"                                       \
    "--config-setting=--build-option=-DPython3_INCLUDE_DIR=/path/to/python3/include"
```
