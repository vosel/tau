#!/bin/bash
LIBRARY_LOCATION=../../../../src/cpp/
g++ -lboost_system -pthread -lboost_thread -D TAU_HEADERONLY -D TAU_CPP_03_COMPATIBILITY -I $LIBRARY_LOCATION main.cpp -o demo
