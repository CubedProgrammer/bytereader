# bytereader
## Features
Certain features can be included by defining the right macros and also compiling the right files.

For gcc and clang, to define a macro XYZ, use the -DXYZ command line option. For MSVC, /DXYZ should do.
### Search and Replace
Compilation macro:
```
BYTEREADER_SEARCH_REPLACE
```
Files to compile:
```
bytefind.c
```
Remember that bytereader.c will always be necessary.
