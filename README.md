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
## Compilation
Swap out clang with gcc or something else if that is your preferred compiler.

Full compilation
```
clang -O3 -std=c99 -c -DBYTEREADER_SEARCH_REPLACE bytefind.c bytereader.c
clang -o bytereader bytefind.o bytereader.o
```
Minimal compilaion
```
clang -O3 -std=c99 -c bytereader.c
clang -o bytereader bytereader.o
```
## Installation
Move the compiled file into a directory in the $PATH
```
mv bytereader /usr/local/bin
```
If you're not running as root, you may not have permission to run the above command.
If your user account is allowed to, put `sudo` or `doas` in front of the command to elevate privilage.

Add aliases to shell, put these lines at the end of your ~/.bashrc, ~/.zshrc, or whichever shell you prefer.
```sh
alias bread='bytereader -a'
alias bfind='bytereader -f'
alias brepl='bytereader -r'
```
