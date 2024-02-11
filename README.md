# Kritase64

A simple C++ Base64 ([RFC 4648](https://datatracker.ietf.org/doc/rfc4648/)) converter.

# Usage

You can use the library by just including `src/kritase64.hpp` and `src/kritase64.cpp` in your project. The library only depends on the C++ Standard Library.

Aside of a library, the project also provides a simple command-line tool for converting files.

## Features

*	`encode()`, `decode()`: Functions for converting arbitrary buffers.
*	`kritase64::Stream` and its variants: Simple iostreams, so that a base64 string can be treated as a file within source code.

# License

This project is licensed under the MIT License. See `LICENSE.txt` For details. You can download the source code [here](https://gitlab.com/kritomas/kritase64)