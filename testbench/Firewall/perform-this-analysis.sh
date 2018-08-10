#!/bin/sh

MAINPATH=`dirname $(readlink -f $0)`

# check if full results should be saved
if [ "$1" = "full" ];
then
  $2 --scn=backw $MAINPATH/Firewall.xml $MAINPATH/FirewallError.xml "subgraph["$4"]" $3 m=injective
else
  $2 --scn=backw $MAINPATH/Firewall.xml $MAINPATH/FirewallError.xml "subgraph["$4"]" m=injective
fi
