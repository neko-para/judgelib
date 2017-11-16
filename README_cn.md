# Judge library

这是libc的一个简单实现.

这个libc是用于OI评测, 所以关于文件和系统的函数没有实现.

数学库没有实现, 而是直接使用libm.a.

你可以直接分发gen文件, 它将会使用git从github上下载代码.

## 构建

你需要GNU bash, gcc和GNU make来编译这个库.

执行 ```./gen``` 来生成Makefile.

执行 ```make ${BIT}``` 来编译对应位数的库, 默认位数是32.

执行 ```make install PREFIX=${PREFIX}``` 来安装到${PREFIX}, 默认路径是/usr/local/judgelib.

## 使用

为了使用这个库, 这样编译代码:

```shell
gcc test.cpp -o test -I${PREFIX}/include -L${PREFIX}/lib -nostdlib ${PREFIX}/lib/crt.o -ljudge --Wl,--rpath=${PREFIX}/lib -lm
```

${PREFIX}是库的安装目录.