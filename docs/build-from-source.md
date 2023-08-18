# Build From Source

## Install Qt 5.15.2

+ If you're using Arm64 (Aarch64 Linux or Apple M1/M2), you need to compile Qt from source and install yourself.

+ The online installers provide only x86_64 prebuilt binaries.

### Mirrors

+ Official: https://download.qt.io/official_releases/online_installers

+ SJTU: https://mirror.sjtu.edu.cn/qt/official_releases/online_installers

+ Tsinghua: https://mirrors.tuna.tsinghua.edu.cn/qt/official_releases/online_installers

+ Aliyun: https://mirrors.aliyun.com/qt/official_releases/online_installers

+ USTC: https://mirrors.ustc.edu.cn/qtproject/official_releases/online_installers

### Installer Options

```sh
./qt-unified-xxx-x64-online --mirror https://mirrors.sjtug.sjtu.edu.cn/qt
```

### Compilers

+ Windows: MSVC 2019 or later
+ Mac: Clang
+ GNU/Linux: GCC 9.4.0 or later

## Build

### Clone Repository

Via HTTPS
```sh
git clone --recursive https://github.com/openvpi/diffscope.git
```

Via SSH
```sh
git clone --recursive git@github.com:openvpi/diffscope.git
```

### Install VCPKG Packages

Use msys terminal (Git Bash) on Windows

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
    -DCMAKE_INSTALL_PREFIX=<dir> \  # install directory
    -DCMAKE_PREFIX_PATH=<dir> \     # directory `Qt5Config.cmake` locates
    -DCMAKE_TOOLCHAIN_FILE=vcpkg/scripts/buildsystems/vcpkg.cmake \
    -DCMAKE_BUILD_TYPE:STRING=Release
    -DBUILD_LIBRARIES_IN_PLACE:BOOL=TRUE

cmake --build build --target all

cmake --build build --target install
```