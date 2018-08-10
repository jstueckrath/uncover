#!/bin/sh
MAINPATH=`dirname $(readlink -f $0)`

# delete old results
rm -f -r $MAINPATH/run-results/*
