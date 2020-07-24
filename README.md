# Ulls Benchmark
A set of benchmarks created to help measuring performance impact of Ultra Low Latency Submission implemented in NEO Driver. It consists of multiple test cases with a set of predefined configurations (parameters like buffer sizes, workgroup sizes, etc.) along with a possibility to run them with any desired parameters. It supports both OpenCL and LevelZero APIs.

## Binary Releases
Ulls Benchmark is currently not released in binary form.

## Building
Internally UllsBenchmark uses googletest, which is referenced as a git submodule and has to be cloned separately. Cloning command is performed by CMake, but Intel employees may have to properly set up proxy. If building hangs on cloning googletest, run
```
git config --global http.proxy http://proxy-chain.intel.com:911
git config --global https.proxy http://proxy-chain.intel.com:912
export http_proxy="http://proxy-chain.intel.com:911"
export https_proxy="htts://proxy-chain.intel.com:912"
```

By default UllsBenchmark enables both OpenCL and LevelZero benchmarks. They can be disabled with commandline arguments to CMake. Example OpenCL-only build invocation:
```
    cmake .. -DTEST_L0=OFF -DTEST_OCL=ON
```

UllsBenchmark will try to find SDKs for the APIs used. In case of inability to find those, it will use libraries contained in [third_party/opencl-sdk](third_party/opencl-sdk) and [third_party/level-zero-sdk](third_party/level-zero-sdk) directories.

## Running
For specific information about how to run the benchmarks, please run the binary with "--help" parameter.

## Contributing
1. Please add new benchmarks in source/tests directory
2. Please insert new benchmarks you add to TestMap contained in source/tests/test_map.cpp. This will enable the benchmark in single-test mode.
3. Please use ClangFormat properly format the code. Visual Studio extension can be downloaded [here](https://marketplace.visualstudio.com/items?itemName=LLVMExtensions.ClangFormat).