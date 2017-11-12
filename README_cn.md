# Judge library

这是libc的一个简单实现.

这个libc是用于OI评测, 所以关于文件和系统的函数没有实现.

## 构建

你需要GNU bash, gcc和GNU make来编译这个库.

执行 ```./gen``` 来生成Makefile.

执行 ```make``` 来编译.

执行 ```make install PREFIX=${PREFIX}``` 来安装到${PREFIX}, 默认路径是/usr/local/judgelib.

## 使用

为了使用这个库, 这样编译代码:

```shell
gcc test.cpp -o test -I${PREFIX}/include -L${PREFIX}/lib -nostdlib ${PREFIX}/lib/crt.o -ljudge.usr --Wl,--rpath=${PREFIX}/lib
```

${PREFIX}是库的安装目录.