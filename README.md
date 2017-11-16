# Judge library

This is a simple implement of libc.

This libc is for OI judge use, so functions about file and system aren't implemented.

The math library isn't implemented, so use libm.a directly.

You can distribute the file gen, and it will download source from github by git.

## Build

To build this lib, you will need GNU bash, gcc and GNU make.

Type ```./gen``` to generate Makefile.

Type ```make ${BIT}``` to build ${BIT} bit version, default is 32.

Type ```make install PREFIX=${PREFIX}``` to install to ${PREFIX}, default is /usr/local/judgelib.

## Use

To use this lib, compile the program like this:

```shell
gcc test.cpp -o test -I${PREFIX}/include -L${PREFIX}/lib -nostdlib ${PREFIX}/lib/crt.o -ljudge --Wl,--rpath=${PREFIX}/lib -lm
```

Where ${PREFIX} is the install prefix.