# Simple C++ Utility Library
A Simple C++ Utility Library.

Currently only tested on Linux and built with Clang 5.0, but it should work with Macs and gcc. Windows support is in progress.

The code is split into a "system" folder and a "util" folder. "system" contains abstractions over platform-specific apis. "util" is a client of "system" and contains higher-level abstractions.

# Requirements

For building:
1. [Conan](https://conan.io)
2. [CMake](https://cmake.org)

There are no other release dependencies. The unit testing framework is [Catch2](https://github.com/catchorg/Catch2) and is downloaded automatically by Conan.

# Build

## Conan
To install via Conan run:
```
make
```

## Release
To otherwise build a release artifact use:
```
make release
```
The artifact will be in ./build/release/libUtil.a

## Debug
To otherwise build a debug artifact use:
```
make debug
```
The artifact will be in ./build/debug/libUtil.a

## Documentation
To generate documentation use:
```
make doc
```
