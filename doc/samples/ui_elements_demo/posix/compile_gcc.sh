#!/bin/bash
LIBRARY_LOCATION=../../../../src/cpp/
g++ -std=c++03 -D TAU_HEADERONLY -D TAU_CPP_03_COMPATIBILITY -I $LIBRARY_LOCATION main.cpp -o demo_gcc_cpp03_posix