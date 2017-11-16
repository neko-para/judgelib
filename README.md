# Judge library

This is a simple implement of libc.

This libc is for OI judge use, so functions about file and system disappear.

## Build

To build this lib, you will need GNU bash, gcc and GNU make.

Type ```./gen ${BIT}``` to generate Makefile for ${BIT} system.

Type ```make``` to build.

Type ```make install PREFIX=${PREFIX}``` to install to ${PREFIX}, default is /usr/local/judgelib.

## Use

To use this lib, compile the program like this:

```shell
gcc test.cpp -o test -I${PREFIX}/include -L${PREFIX}/lib -nostdlib ${PREFIX}/lib/crt.o -ljudge --Wl,--rpath=${PREFIX}/lib -lm
```

Where ${PREFIX} is the install prefix.