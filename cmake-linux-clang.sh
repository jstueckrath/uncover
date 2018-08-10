#!/bin/sh

./cmake-linux-generic.sh -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang
exit $?
