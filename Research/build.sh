#!/bin/bash
set -x
rm matmul_*
CXX=aarch64-linux-gnu-gcc
#CXX=aarch64-linux-gnu-g++-8

$CXX -O3 -static -DVERSION=0 matmul.c -o matmul_0
# $CXX -O3 -static -DVERSION=1 matmul.c -o matmul_1
# $CXX -O3 -static -DVERSION=2 matmul.c -o matmul_2
