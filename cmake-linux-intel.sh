#!/bin/sh

./cmake-linux-generic.sh -DCMAKE_CXX_COMPILER=icpc -DCMAKE_C_COMPILER=icc
exit $?
