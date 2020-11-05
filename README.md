# Compute Benchmarks
A benchmark suite developed and maintained by NEO driver development team in order to provide a stable environment which allows checking and reproducing performance for various parts of the driver. The benchmark suite is divided into multiple executables, each of which tests a different aspect of the driver. Each executable contained in ComputeBechmarks consists of multiple test cases with a set of predefined configurations (parameters like buffer sizes, workgroup sizes, etc.) along with a possibility to run them with any desired parameters. They support both OpenCL and LevelZero APIs. Current set of exectuables:

|Executable |Measured aspect |
|-----------|----------------|
ulls_benchmark |performance impact of Ultra Low Latency Submission implemented in NEO Driver |
memory_benchmark |bandwidth of different memory transfer operations using various APIs |
multi_tile_benchmark | bandwidth of different memory transfer operations using various APIs on a multi-tile device |
api_overhead_benchmark | duration of various API calls |
gpu_cmds_benchmark | duration of various commands on the GPU |
overlap_benchmark | GPU operations overlapping |

## How to run benchmarks
For specific information about how to run the benchmarks, please run the binary with "--help" parameter.

## Binary releases

Binary releases are available on Artifactory for both [Windows](https://gfx-assets.igk.intel.com/artifactory/webapp/#/artifacts/browse/tree/General/gfx-ocl-abn-assets-igk/windows/compute_benchmarks) and [Linux](https://gfx-assets.igk.intel.com/artifactory/webapp/#/artifacts/browse/tree/General/gfx-ocl-abn-assets-igk/linux/compute_benchmarks). Keep in mind, the releases may not be regular and it's best to build the newest version of the benchmarks yourself. You can find out how recent the binary release is from its name. Naming is as follows:
```
YYYYMMDD_II_HHHHHHH

Y - year of publishing
M - month of publishing
D - day of publishing
I - index in given day
H - git commit hash ID
```

## Building

### Setup proxy
Internally ComputeBenchmarks uses googletest, which is referenced as a git submodule and has to be cloned separately. Cloning command is automatically performed by CMake, but Intel employees may have to properly set up proxy. If building hangs on cloning googletest, use [build.sh](build.sh) script, which sets up proxy or run the following commands:
```
git config --global http.proxy http://proxy-chain.intel.com:911
git config --global https.proxy http://proxy-chain.intel.com:912
export http_proxy="http://proxy-chain.intel.com:911"
export https_proxy="htts://proxy-chain.intel.com:912"
```

### Run CMake and compile
Build the benchmark as any other CMake project. Example command sequence:
```
git clone https://gitlab.devtools.intel.com/mdziuban/ullsbenchmark.git
cd ullsbenchmark
mkdir build
cd build
cmake ..
cmake --build . --config Release
```
The last line can be substituted with platform-specific command, for example:
```
make -j`nproc`
```
or:
```
make memory_benchmark_ocl -j`nproc`
```

### Single API binaires
Each benchmark suite can be built as a single-api binary or as a an all-api binary.
- Single-api binaries are named like `ulls_benchmark_ocl` and do not load libraries from not used APIs.
- All-api binaries are named like `ulls_benchamrk` and contain every API supported by the benchmark, which makes them more convenient, but at a risk of introducing some overhead caused by loading unnecessary library.

By default all benchmarks are compiled in all configurations they support. This can be changed with CMake arguments. For example, to disable all-api binaries, run:
```
cmake .. -DBUILD_ALL_API_BINARIES=OFF -DBUILD_SINGLE_API_BINARIES=ON
```

### SDK
ComputeBenchmarks will try to find SDKs for the APIs used. In case of inability to find those, it will use libraries contained in [third_party/opencl-sdk](third_party/opencl-sdk) and [third_party/level-zero-sdk](third_party/level-zero-sdk) directories. The libraries where compiled on Ubuntu 18.04 LTS with gcc 7.4.0 compiler. Using a different setup may result in build failures due to ABI incompatibility, so it's safest to have the SDK installed in your system.

## Contributing
Please use ClangFormat to properly format the code. Visual Studio extension can be downloaded [here](https://marketplace.visualstudio.com/items?itemName=LLVMExtensions.ClangFormat).

Submit your changes via pull request. Maintainers are allowed to push to the master branch directly.

Please do not create wrappers for compute API calls if not absolutely necessary.

If you want to significantly change the underlying test framework, please contact maciej.dziuban@intel.com first. There are many changes planned and it's best not to interfere with each other.

### Adding new benchmarks
A good way to add new benchmarks is to mimic the existing ones and tweak them to your needs. General flow of adding a brand new test is:
1. Select binary, that suits you benchmark, for example `memory_benchmark`
2. Select name for you benchmark, for example `TwoWayTransfer`.
3. Add definition file of your benchmark as source/`memory_benchmark`/definitions/`TwoWayTransfer`.h. This file specifiec generic info about your test - its name, description and parameters
4. Add test registration file as source/`memory_benchmark`/gtest/`TwoWayTransfer`.cpp. This file registers your test, so the framework knowns about it and it can be run.
5. Add implementation file as source/`memory_benchmark`/implementations/ocl/`TwoWayTransfer`_ocl.cpp. This file is contains the actual implementation of your test. Substittue *ocl* with *l0* for LevelZero implementation. Each test *can* be implemented in more than one API.

