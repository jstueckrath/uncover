#!/bin/sh

MAINPATH=`dirname $(readlink -f $0)`

# check if full results should be saved
if [ "$1" = "full" ];
then
  STORAGEFOLDER=$3
else
  STORAGEFOLDER=""
fi

if [ "$4" = "uq" ]
then
  echo "starting analysis of backery algorithm with UQ rules"
  $2 --scn=backw $MAINPATH/BakeryUQ.xml $MAINPATH/BakeryError.xml "subgraph[-]" $STORAGEFOLDER to=3600 matching=injective
else
  echo "starting analysis of backery algorithm up to path length $4"
  $2 --scn=backw $MAINPATH/Bakery.xml $MAINPATH/BakeryError.xml "subgraph[$4]" $STORAGEFOLDER to=3600 matching=injective
fi

