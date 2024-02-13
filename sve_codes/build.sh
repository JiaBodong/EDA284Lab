#!/bin/bash
set -x
rm stencil_* matmul_*
CXX=aarch64-linux-gnu-gcc
#CXX=aarch64-linux-gnu-g++-8
$CXX -O3 -static -DVERSION=0 -march=armv8-a+sve stencil.c -o stencil_0
$CXX -O3 -static -DVERSION=1 -march=armv8-a+sve stencil.c -o stencil_1
$CXX -O3 -static -DVERSION=0 -march=armv8-a+sve matmul.c -o matmul_0
$CXX -O3 -static -DVERSION=1 -march=armv8-a+sve matmul.c -o matmul_1
$CXX -O3 -static -DVERSION=2 -march=armv8-a+sve matmul.c -o matmul_2
