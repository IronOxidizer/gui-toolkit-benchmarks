# gui-toolkit-benchmarks
A suite of benchmarks for measuring the performance of various system GUI toolkits

This project was created with the hopes of creating competition between various GUI toolkit projects in the same way that the [TechEmpower benchmarks](https://github.com/TechEmpower/FrameworkBenchmarks) pushed web server frameworks to strive for more than what's [currently acceptable](https://en.wikipedia.org/wiki/Wirth%27s_law).

Heavily inspired by the following projects:

- https://github.com/TechEmpower/FrameworkBenchmarks
- https://github.com/krausest/js-framework-benchmark
- https://tolszak-dev.blogspot.com/2013/02/simple-qml-vs-efl-comparison.html
- https://www.grapecity.com/blogs/flexgrid-performance-compare-desktop-platforms

All aspects of performance will be measured. This includes (but is not limited to):

- Executable size
- Startup time
- Memory usage
- Rendering speed
- Input latency/reponsiveness
- CPU usage
- GPU usage
- Disk/cache usage
- Resource scalability (multiple app instances, large grids)

Metrics will have different variants to cover all important cases such as cold and warm results (first run and after some caching/JIT), standalone and shared resource usage (executable and libs/vm/interpreter), and multiple platforms and architectures.
