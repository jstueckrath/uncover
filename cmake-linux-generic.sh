#!/bin/sh

rm -r -f "build"
mkdir "build"
cd "build"

cmake "$@" ../src
exit $?
