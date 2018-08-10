#!/bin/sh

MAINPATH=`dirname $(readlink -f $0)`

# check if full results should be saved
if [ "$1" = "full" ];
then
  $2 --scn=backw $MAINPATH/BurnsMutEx.xml $MAINPATH/BurnsMutExError.xml "subgraph[-]" $3 m=injective
else
  $2 --scn=backw $MAINPATH/BurnsMutEx.xml $MAINPATH/BurnsMutExError.xml "subgraph[-]" m=injective
fi
