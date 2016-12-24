#!/bin/bash
LIBRARY_LOCATION=../../../../src/cpp/
g++ -std=c++11 -D TAU_HEADERONLY -lboost_system -pthread -lboost_thread -I $LIBRARY_LOCATION main.cpp -o demo_gcc_cpp03_boost
