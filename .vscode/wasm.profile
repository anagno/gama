include(default)

[settings]
os_build=Linux
arch_build=x86_64
os=Emscripten
arch=wasm
build_type=Release
compiler=clang
compiler.version=6.0
compiler.libcxx=libc++
[options]
[build_requires]
emsdk_installer/1.38.29@bincrafters/stable
[env]
BUILD_TOOS_CMAKE_CXX_COMPILER=g++