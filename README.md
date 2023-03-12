# gui-toolkit-benchmarks
A suite of benchmarks for measuring the performance of various system GUI toolkits

This project was created with the hopes of creating competition between various GUI toolkit projects in the same way that the [TechEmpower](https://github.com/TechEmpower/FrameworkBenchmarks) and [krausest](https://github.com/krausest/js-framework-benchmark) benchmarks [raised the bar](https://en.wikipedia.org/wiki/Wirth%27s_law).

Heavily inspired by the following projects:

- https://github.com/TechEmpower/FrameworkBenchmarks
- https://github.com/krausest/js-framework-benchmark
- https://tolszak-dev.blogspot.com/2013/02/simple-qml-vs-efl-comparison.html
- https://www.grapecity.com/blogs/flexgrid-performance-compare-desktop-platforms
- https://digitalcommons.wpi.edu/cgi/viewcontent.cgi?article=2796&context=mqp-all
- https://eugenkiss.github.io/7guis/more
- https://szibele.com/memory-footprint-of-gui-toolkits/
- https://web.wpi.edu/Pubs/E-project/Available/E-project-042518-124639/unrestricted/Developing_a_Benchmark_for_Qt_on_Embedded_Platforms.pdf
- https://lukaskalbertodt.github.io/2023/02/03/tauri-iced-egui-performance-comparison.html

All aspects of performance will be measured. This includes (but is not limited to):

- Size
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

### Size

The size of the executable with the following variants:

- **total**: The executable along with its runtime dependencies (size of container after compilation and cleanup minus size of base image)
    - `docker ps --size -a`
- **executable**: The single executable by itself

### Startup time

The time between the application start to the first complete paint **and** input handling is enabled. This is sent to the benchmarker by printing the epoch time in the most precise method to `stderr` when startup is complete. Startup time has the following variants:

- **cold**: The startup time immediately after first download or compile (no caching, first JIT not yet performed)
- **warm**: The startup time after opening and closing the application 8 times

### Memory usage

- Physical memory usage
- Virtual memory usage
- Private memory usage
- Shared memory usage

### Rendering speed

This can be tested in two ways.

- How quickly an animation can be rendered, assuming all GUI toolkits display the same frames
- The min/avg/max framerate given an animation function and a fixed time

https://linuxhint.com/show_fps_counter_linux_games/

## Test Platforms

Requirements:

- lightweight
- non-tiling (tiling window managers may cause repaints on application launch or resize events)

### Linux X

- Host Distro: [Debian](https://www.debian.org/CD/netinst/)
- Window Manager: ~~[TinyWM](http://incise.org/tinywm.html)~~ [OpenBox](http://openbox.org)

### Linux Wayland (todo)

- Host Distro: [Debian](https://www.debian.org/CD/netinst/)
- Window Manager: [Weston](https://github.com/wayland-project/weston)
