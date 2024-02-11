# Kritase64

A simple C++ Base64 ([RFC 4648](www.datatracker.ietf.org/doc/rfc4648)) converter. Includes a library and a command-line tool.

# Building

Whilst the library doesn't need building (you just add it to your project, see [Usage](#usage)), the command-line tool does.

## On UNIX-like systems

Run `./configure`, and then `make`. If you also want the tool installed globally, also run `make install`.

## On Windows

Windows isn't officially supported. However the steps above should work just fine via POSIX-like environments, like [MSYS2](www.msys2.org).

s

s

s

s

s

s

s

s

s


s

s

s

s

s


ss

s

s

s

s

s

s

s

s


s

s

s

s

s


ss

s

s

s

s

s

s

s

s


s

s

s

s

s


ss

s

s

s

s

s

s

s

s


s

s

s

s

s


ss

s

s

s

s

s

s

s

s


s

s

s

s

s


ss

s

s

s

s

s

s

s

s


s

s

s

s

s


s

s

s



# Usage <a id='usage'></a>

You can use the library by just including `src/kritase64.hpp` and `src/kritase64.cpp` in your project. The library only depends on the C++ Standard Library.

Aside of a library, the project also provides a simple command-line tool for converting files.

## Features

*	`encode()`, `decode()`: Functions for converting arbitrary buffers.
*	`kritase64::Stream` and its variants: Simple iostreams, so that a base64 string can be treated as a file within source code.

# License

This project is licensed under the MIT License. See `LICENSE.txt` For details. You can download the source code [here](www.gitlab.com/kritomas/kritase64)