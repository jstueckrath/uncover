#!/bin/sh

MAINPATH=`dirname $(readlink -f $0)`
cd $MAINPATH

if [ ! -d ../old_run-results ];
then
  mkdir ../old_run-results
fi

cp -r -n ./run-results/* ../old_run-results
