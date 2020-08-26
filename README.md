# Compute Benchmarks
A benchmark suite developed and maintained by NEO driver development team in order to provide a stable environment which allows checking and reproducing performance for various parts of the driver. The benchmark suite is divided into multiple executables, each of which tests a different aspect of the driver. Each executable contained in ComputeBechmarks consists of multiple test cases with a set of predefined configurations (parameters like buffer sizes, workgroup sizes, etc.) along with a possibility to run them with any desired parameters. They support both OpenCL and LevelZero APIs. Current set of exectuables:
1. ulls_benchmark - benchmarks measuring performance impact of Ultra Low Latency Submission implemented in NEO Driver.
2. pci_benchmark - benchmarks for bandwidth of different memory transfer operations using various APIs

## Binary Releases
Ulls Benchmark is released to gfx-assets.igk.intel.com. [Linux](https://gfx-assets.igk.intel.com/artifactory/webapp/#/artifacts/browse/tree/General/gfx-ocl-abn-assets-igk/linux/ulls_benchmark) [Windows](https://gfx-assets.igk.intel.com/artifactory/webapp/#/artifacts/browse/tree/General/gfx-ocl-abn-assets-igk/windows/ulls_benchmark)

Pci Benchmark is released to gfx-assets.igk.intel.com. [Linux](https://gfx-assets.igk.intel.com/artifactory/webapp/#/artifacts/browse/tree/General/gfx-ocl-abn-assets-igk/linux/pci_benchmark) [Windows](https://gfx-assets.igk.intel.com/artifactory/webapp/#/artifacts/browse/tree/General/gfx-ocl-abn-assets-igk/windows/pci_benchmark)

## Building
Internally ComputeBenchmarks uses googletest, which is referenced as a git submodule and has to be cloned separately. Cloning command is automatically performed by CMake, but Intel employees may have to properly set up proxy. If building hangs on cloning googletest, use [build.sh](build.sh) script, which sets up proxy or run the following commands:
```
git config --global http.proxy http://proxy-chain.intel.com:911
git config --global https.proxy http://proxy-chain.intel.com:912
export http_proxy="http://proxy-chain.intel.com:911"
export https_proxy="htts://proxy-chain.intel.com:912"
```

By default ComputeBenchmarks enables both OpenCL and LevelZero benchmarks. They can be disabled with commandline arguments to CMake. Example OpenCL-only build invocation:
```
    cmake .. -DTEST_L0=OFF -DTEST_OCL=ON
```

ComputeBenchmarks will try to find SDKs for the APIs used. In case of inability to find those, it will use libraries contained in [third_party/opencl-sdk](third_party/opencl-sdk) and [third_party/level-zero-sdk](third_party/level-zero-sdk) directories. The libraries where compiled on Ubuntu 18.04 LTS with gcc 7.4.0 compiler. Using a different setup may result in build failures due to ABI incompatibility, so it's safest to have the SDK installed in your system.

## Running
For specific information about how to run the benchmarks, please run the binary with "--help" parameter.

## Contributing
Please insert new benchmarks you add to TestMap contained in source/<benchmark_name>/benchmark_info.cpp. This will enable the benchmark in single-test mode.
Please use ClangFormat to properly format the code. Visual Studio extension can be downloaded [here](https://marketplace.visualstudio.com/items?itemName=LLVMExtensions.ClangFormat).
