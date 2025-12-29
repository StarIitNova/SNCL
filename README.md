# StarlitNova's C Library

A C library engineered for my needs. It's a bunch of random tools, inspired heavily by stb.

Contributors are listed inside the files they contributed to.

### Libraries / Provided Modules

library | includes | version | category | description
--------|----------|---------|----------|------------
sncl\_clex | sncl\_clex.h | 1.00 | Compilers | A more capable C lexer based on stb\_c\_lexer
sncl\_arraylist | sncl\_arraylist.h | 1.00 | Data Structures | An ArrayList (vector) implementation in C
sncl\_clioptions | sncl\_clioptions.h | 1.00 | Utility | Command line argument parser for C (better argv parser)

### Contributing

You're free to clone the repository and make pull requests at any time. If you'd rather,
you can submit an issue but there's no guaranteed anything will happen.

Please note that SNCL was originally made for the Saki build system, and as such should be
developed with that in mind. Extra modules may be added at any time, but things like the
primary makefile should not be so heavily modified that it can no longer ouput the libsncl binary
in the proper directory or for object/dependency files to populate directories that should be clean.

### License

Copyright (c) StarIitNova, all rights reserved.

You are free to use this code with or without the license, but there is no granted warranty for the
code. Any and all damages caused by or associated with this code are not the fault of the authors.

You may reproduce this code without the license, but you may not sell exclusive access to the code unless
part of a larger project (as in, you are not selling access to the code in the repository, instead you'd
be selling access to a full scale commercial application that happens to use this code).
