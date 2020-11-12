# Contributing to ComputeBenchmarks
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
