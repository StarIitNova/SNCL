# StarlitNova's C Library

A C library engineered for my needs. It's a bunch of random tools, inspired heavily by stb.

Contributors are listed inside the files they contributed to.

### Table of Contents

1. [Libraries / Provided Modules](#libraries--provided-modules)
2. [Contributing](#contributing)
3. [Building / Usage Guide](#building)
4. [License](#license)

### Libraries / Provided Modules

library | includes | version | category | description | dependencies
--------|----------|---------|----------|-------------|-------------
sncl\_clex | sncl\_clex.h | 1.00 | Compilers | A more capable C lexer based on stb\_c\_lexer | None
sncl\_arraylist | sncl\_arraylist.h | 1.00 | Data Structures | An ArrayList (vector) implementation in C | sncl_typeid.h
sncl\_clioptions | sncl\_clioptions.h | 1.00 | Utility | Command line argument parser for C (better argv parser) | None
sncl\_test | sncl\_test.h | 0.23 | Utility | Test runner for C and C++, based on JUnit 5 but better (not included in main library -- include this yourself) | Unix system
sncl\_typeid | sncl\_typeid.h | X.XX | Utility | Provides type information for versions pre-C23 (and even up to in the future) | None

### Contributing

You're free to clone the repository and make pull requests at any time. If you'd rather,
you can submit an issue but there's no guaranteed anything will happen.

Please note that SNCL was originally made for the Saki build system, and as such should be
developed with that in mind. Extra modules may be added at any time, but things like the
primary makefile should not be so heavily modified that it can no longer ouput the libsncl binary
in the proper directory or for object/dependency files to populate directories that should be clean.

Alongside the guidelines above, ensure that you submit tests with any new API you add and ensure all tests pass. Pull requests
that do not provide adequate testing won't be merged until testing is provided, although if I actually want the library to be
included within SNCL, I might make the tests myself.

For information on building, see below.

### Building

Building is quite simple, although you do have to run the configuration manager as this does build a static library,
and not everyone wants everything inside of their project.

To get started, run `./config.sh` and select the modules you want to be built. Then run `make` and libsncl.a should be generated.

If you're on windows, I'm sorry for not adding a separate mingw make for you, although it shouldn't be hard to just add the c files
directly into your project along with the headers, you don't actually need the static library to be built. The point of SNCL was to
be easily embeddable, not "you have to do it the way intended by my makefile!"

### License

Copyright (c) StarIitNova, all rights reserved.

You are free to use this code with or without the license, but there is no granted warranty for the
code. Any and all damages caused by or associated with this code are not the fault of the authors.

You may reproduce this code without the license, but you may not sell exclusive access to the code unless
part of a larger project (as in, you are not selling access to the code in the repository, instead you'd
be selling access to a full scale commercial application that happens to use this code).
