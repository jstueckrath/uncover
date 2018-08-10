#!/bin/sh

MAINPATH=`dirname $(readlink -f $0)`

# check if full results should be saved
if [ "$1" = "full" ];
then
  STORAGEFOLDER=$3
else
  STORAGEFOLDER=""
fi

if [ "$4" = "withswap" ]
then
  echo "starting analysis of multi user system with swap rule and injective matches"
  $2 --scn=backw $MAINPATH/MultiUserSystemWithSwap.xml $MAINPATH/MultiUserSystemError.xml "subgraph[-]" $STORAGEFOLDER to=3600 matching=injective
elif [ "$4" = "withswapni" ]
then
  echo "starting analysis of multi user system with swap rule and non-injective matches"
  $2 --scn=backw $MAINPATH/MultiUserSystemWithSwap.xml $MAINPATH/MultiUserSystemError.xml "subgraph[-]" $STORAGEFOLDER to=3600 
else
  echo "starting analysis of multi user system without swap rule (with non-injective matches)"
  $2 --scn=backw $MAINPATH/MultiUserSystem.xml $MAINPATH/MultiUserSystemError.xml "subgraph[-]" $STORAGEFOLDER to=3600
fi

