#!/bin/bash
set -x
rm nbody_*
# CXX=aarch64-linux-gnu-gcc
CXX=aarch64-linux-gnu-g++
# $CXX -O3 -static -march=armv8-a+sve -std=c++11 -fopenmp nbody.cpp -o nbody_0 
$CXX -O3 -static -march=armv8-a+sve -std=c++11 -fopenmp nbody.cpp -o nbody_1
# $CXX -O3 -static nbody.cpp -o nbody_row
# $CXX -O3 -static -DVERSION=1 -march=armv8-a+sve matmul.c -o matmul_1
# $CXX -O3 -static -DVERSION=2 -march=armv8-a+sve matmul.c -o matmul_2
