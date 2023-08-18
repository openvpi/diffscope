# DiffScope

DiffSinger Editor developed by OpenVPI.

## Supported Platforms

+ Microsoft Windows (Vista ~ 11)
+ Apple Mac OSX (11+)
+ Linux (Tested on Ubuntu)

## Build From Source

### Requirements

| Component | Requirement |               Detailed               |
|:---------:|:-----------:|:------------------------------------:|
|    Qt     |  \>=5.15.2  |   Core, Gui, Widgets, Svg, Network   |
| Compiler  |  \>=C++17   |        MSVC 2019, GCC, Clang         |
|   CMake   |   \>=3.17   |        >=3.20 is recommended         |
<!-- |  Python   |   \>=3.8    |                  /                   | -->

### Setup Environment

You need to install Qt libraries first. (Tested on Qt 5.15.2 only)

#### Clone Repository

Via HTTPS
```sh
git clone --recursive https://github.com/openvpi/diffscope
```

Via SSH
```sh
git clone --recursive git@github.com:openvpi/diffscope.git
```

#### Install VCPKG Packages

```sh
cd vcpkg
./bootstrap-vcpkg.sh

export QT_DIR=<dir> # directory `Qt5Config.cmake` locates
export Qt5_DIR=$QT_DIR
export VCPKG_KEEP_ENV_VARS=QT_DIR;Qt5_DIR

./vcpkg install \
    --x-manifest-root=../scripts/vcpkg-manifest \
    --x-install-root=./installed \
    --triplet=<triplet>

# triplet:
#   Windows:  `x64-windows` 
#   Mac:      `x64-osx` or `arm64-osx`
#   Linux:    `x64-linux` or `arm64-linux`
```

### Build & Install

```sh
cmake -B build -G Ninja \
    -DCMAKE_INSTALL_PREFIX=<dir> \ # install directory
    -DCMAKE_PREFIX_PATH=<dir> \ # directory `Qt5Config.cmake` locates
    -DCMAKE_TOOLCHAIN_FILE=vcpkg/scripts/buildsystems/vcpkg.cmake \
    -DCMAKE_BUILD_TYPE=Release

cmake --build build --target all

cmake --build build --target install
```