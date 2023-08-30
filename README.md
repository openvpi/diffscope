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
|   CMake   |   \>=3.17   |        >=3.20 is recommended         |
|  Python   |   \>=3.8    |                  /                   |

## Build From Source

This application uses Microsoft VCPKG to handle its dependencies on third-party libraries other than Qt, which you can choose to install in advance, but we recommend using the VCPKG package manager as a submodule of this repository.

+ [Build From Source](docs/build-from-source.md)

## Dependencies

### Open-source Libraries

+ [Qt 5.15.2](https://www.qt.io/)
    + GNU LGPL v2.1 or later
+ [vcpkg](https://github.com/microsoft/vcpkg)
    + MIT License
+ [FFmpeg](https://github.com/FFmpeg/FFmpeg)
    + GNU LGPL v2.1 or later
+ [lame](https://lame.sourceforge.io/)
    + GNU LGPL v2.0
+ [SDL](https://github.com/libsdl-org/SDL)
    + Zlib License
+ [QuaZip](https://github.com/stachenov/quazip)
    + GNU LGPL v2.1
+ [fluentui-system-icons](https://github.com/microsoft/fluentui-system-icons)
    + MIT License
+ [SndFile](https://github.com/libsndfile/libsndfile)
    + GNU LGPL v2.1 or later

### Related Projects

+ [FramelessHelper](https://github.com/wangwenx190/framelesshelper)
    + MIT License
+ [qastool](https://github.com/SineStriker/qt-json-autogen)
    + Apache 2.0 License
+ [FFmpeg-fake](https://github.com/SineStriker/ffmpeg-fake)
    + GNU LGPL v2.1 or later
+ [QNrbf](https://github.com/SineStriker/QNrbf)
    + GNU LGPL v2.1 or later
+ [AceTreeModel](https://github.com/SineStriker/AceTreeModel)
    + Apache 2.0 License
+ [ChorusKit](https://github.com/SineStriker/choruskit)
    + GNU LGPL v2.1
+ [qtmediate](https://github.com/SineStriker/qtmediate)
    + GNU LGPL v2.1

<!-- ### Special Thanks

+ [wangwenx190](https://github.com/wangwenx190)

+ [MapleSpe](https://github.com/maplespe) -->

## License

DiffScope is licensed under the Apache 2.0 License.