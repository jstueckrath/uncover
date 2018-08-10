#!/bin/sh

MAINPATH=`dirname $(readlink -f $0)`

# check if full results should be saved
if [ "$1" = "full" ];
then
  $2 --scn=backw $MAINPATH/DiningPhilosophers.xml $MAINPATH/DiningPhilosophersError.xml "subgraph[-]" $3 m=injective to=3600
else
  $2 --scn=backw $MAINPATH/DiningPhilosophers.xml $MAINPATH/DiningPhilosophersError.xml "subgraph[-]" m=injective to=3600
fi
