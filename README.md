# gui-toolkit-benchmarks
A suite of benchmarks for measuring the performance of various system GUI toolkits

This project was created with the hopes of creating competition between various GUI toolkit projects in the same way that the [TechEmpower benchmarks](https://github.com/TechEmpower/FrameworkBenchmarks) pushed web server frameworks to strive for more than what's [currently acceptable](https://en.wikipedia.org/wiki/Wirth%27s_law).

Heavily inspired by the following projects:

- https://github.com/TechEmpower/FrameworkBenchmarks
- https://github.com/krausest/js-framework-benchmark
- https://tolszak-dev.blogspot.com/2013/02/simple-qml-vs-efl-comparison.html
- https://www.grapecity.com/blogs/flexgrid-performance-compare-desktop-platforms
- https://digitalcommons.wpi.edu/cgi/viewcontent.cgi?article=2796&context=mqp-all
- https://eugenkiss.github.io/7guis/more

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

## Testing methodology

### Executable size

The size of the executable with the following variants:

- **base**: The single executable by itself
- **total**: The executable along with its runtime dependencies (not including base platform, OS, window manager, i.e things that were already installed in the base image)
- **stripped total**: Same as total but with the executable and runtime dependencies stripped of extraneous symbols

### Startup time

The time between the application start to the first complete paint **and** input handling is enabled. This is sent to the benchmarker by printing the epoch time in the most precise method to `stderr` when startup is complete. Startup time has the following variants:

- **cold**: The startup time immediately after first download or compile (no caching, first JIT not yet performed)
- **warm**: The startup time after opening and closing the application 8 times

### Rendering speed

This can be tested in two ways.

- How quickly an animation can be rendered, assuming all GUI toolkits display the same frames
- The min/avg/max framerate given an animation function and a fixed time

## Test Platforms

### Linux X

This will use the latest [Debian Sid NetInst](https://www.debian.org/CD/netinst/) as the base image since it's the best representation of a modern install with up-to-date packages that are properly segregated and minimal resource usage. Openbox is a simple, light, and widely used window manager for X that does not automatically tile windows (which would cause a repaint for some toolkits) or include a toolbar/panel which makes it the perfect window manager for the X test platform.
