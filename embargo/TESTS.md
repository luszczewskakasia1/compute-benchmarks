# ulls_benchmark_embargo
Ulls Benchmark is a set of tests aimed at measuring Ultra Low Latency Submission (ULLS) performance impact.
| Test name | Description | Params | L0 | OCL |
|-----------|-------------|--------|----|-----|
EmptyKernelEmbargo|enqueues empty kernel and measures time to launch it and wait for it on CPU, thus measuring walker spawn time.|<ul><li>--wgc Workgroup count</li><li>--wgs Workgroup size (aka local work size)</li></ul>|:heavy_check_mark:|:heavy_check_mark:|



