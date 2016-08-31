#!/bin/bash
LIBRARY_LOCATION=../../../../src/cpp/
g++ -D TAU_HEADERONLY -D TAU_CPP_03_COMPATIBILITY -I $LIBRARY_LOCATION main.cpp -o demo
