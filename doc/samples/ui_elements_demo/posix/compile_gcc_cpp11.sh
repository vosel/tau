#!/bin/bash
LIBRARY_LOCATION=../../../../src/cpp/
g++ -std=c++11 -D TAU_HEADERONLY -I $LIBRARY_LOCATION main.cpp -o demo_gcc_cpp11_posix
