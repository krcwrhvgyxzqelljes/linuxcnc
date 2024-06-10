Precompiled library of the zlib library from:

- `https://github.com/zlib-ng/zlib-ng <https://github.com/zlib-ng/zlib-ng>`__.

Compiled with:

~~~
mkdir build
cd build
cmake -G "MinGW Makefiles" -DCMAKE_INSTALL_PREFIX:VAR=".." -DCMAKE_BUILD_TYPE=Release -DZLIB_COMPAT=ON ..
namke
~~~
