# DiffScope

DiffSinger Editor developed by OpenVPI.

## Supported Platforms

+ Microsoft Windows (Vista ~ 11)
+ Apple Mac OSX (11+)
+ GNU/Linux (Tested on Ubuntu)

## Requirements

| Component | Requirement |               Detailed               |
|:---------:|:-----------:|:------------------------------------:|
|    Qt     |  \>=5.15.2  |   Core, Gui, Widgets, Svg, Network   |
| Compiler  |  \>=C++17   |        MSVC 2019, GCC, Clang         |
|   CMake   |   \>=3.19   |        >=3.20 is recommended         |
|  Python   |   \>=3.8    |                  /                   |

## Build From Source

This application uses Microsoft VCPKG to handle its dependencies on third-party libraries other than Qt, which you can choose to install in advance, but we recommend using the VCPKG package manager as a submodule of this repository.

+ [Build From Source](docs/build-from-source.md)

## Related Projects

+ [DiffSinger](https://github.com/openvpi/DiffSinger)
    + Apache-2.0

## License

DiffScope is licensed under the Apache 2.0 License.