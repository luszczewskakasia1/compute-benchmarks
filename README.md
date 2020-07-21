# Ulls Benchmark
A set of benchmarks created to help measuring performance impact of Ultra Low Latency Submission implemented in NEO Driver. It consists of multiple test cases with a set of predefined configurations (parameters like buffer sizes, workgroup sizes, etc.) along with a possibility to run them with any desired parameters. It supports both OpenCL and LevelZero APIs.

## Binary Releases
Ulls Benchmark is currently not released in binary form.

## Building
To generate build files (VS solution, makefiles, etc.) run a build script suited for your OS (build.bat for Windows, build.sh for Linux). Beside running CMake it will also run the git command, which will download project's dependencies from external repositories.

UllsBenchmark requires both OpenCL and LevelZero to be built. If those libraries are properly installed in your system, CMake should find them and build without problems. In case of failure there's also a fallback path, put the required SDKs in root directory like so:
```
+-- build
+-- level-zero-sdk
|  +-- include
|    +-- level_zero
|      +-- ze_api.h
|      +-- ze_barrier.h
|      +-- ze_callbacks.h
|      ...
|  +-- lib
|    +-- x64
|      +-- ze_loader.lib
+-- opencl-sdk
|  +-- include
|    +-- CL
|      +-- cl.h
|      +-- cl_icd.h
|      +-- cl_platform.h
|      ...
|  +-- lib
|    +-- x64
|      +-- OpenCL.lib
+-- source
+-- third_party
+-- CMakeLists.txt
```

## Running
For specific information about how to run the benchmarks, please run the binary with "--help" parameter.

## Contributing
1. Please add new benchmarks in source/tests directory
2. Please insert new benchmarks you add to TestMap contained in source/tests/test_map.cpp. This will enable the benchmark in single-test mode.
3. Please use ClangFormat properly format the code. Visual Studio extension can be downloaded [here](https://marketplace.visualstudio.com/items?itemName=LLVMExtensions.ClangFormat).