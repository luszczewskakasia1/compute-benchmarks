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
atomic_benchmark | Atomic operations in kernels |
miscellaneous_benchmark | Different simple compute scenarios |

## How to run benchmarks
For specific information about how to run the benchmarks, please run the binary with "--help" parameter.

## FAQ
Answers to frequently asked questions and common problems can be found in [FAQ.md](FAQ.md).

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

### Contributing
Information on how to contribute to ComputeBenchmarks can be found in [CONTRIBUTING.md](CONTRIBUTING.md)
