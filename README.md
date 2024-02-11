# Kritase64

A simple C++ Base64 ([RFC 4648](https://datatracker.ietf.org/doc/rfc4648)) converter. Includes a library and a command-line tool.

# Building

Whilst the library doesn't need building (you just add it to your project, see [Usage](#usage)), the command-line tool does.

## On UNIX-like systems

Run `./configure`, and then `make`. If you want the tool installed globally, also run `make install`.

## On Windows

Windows isn't officially supported. However, the steps above should theortically work just fine on POSIX-like environments, like [MSYS2](https://www.msys2.org).

# Usage <a id='usage'></a>

You can use the library by just including `src/kritase64.hpp` and `src/kritase64.cpp` in your project. The library only depends on the C++ Standard Library.

To use the command-line tool, just run `kritase64`.

## Features

*	`kritase64::encode()`, `kritase64::decode()`: Functions for converting arbitrary buffers.
*	`kritase64::Stream` and its variants: Simple iostreams, so that a base64 string can be treated as a file within source code.

# License

This project is licensed under the MIT License. See `LICENSE.txt` For details. You can download the source code [here](https://gitlab.com/kritomas/kritase64).