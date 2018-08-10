#!/bin/sh
MAINPATH=`dirname $(readlink -f $0)`

echo "starting public private server analysis with path restriction "$4

# check if full results should be saved
if [ "$1" = "full" ];
then
  $2 --scn=backw $MAINPATH/PublicPrivateServer.xml $MAINPATH/PublicPrivateServerError.xml "subgraph["$4"]" $3
else
  $2 --scn=backw $MAINPATH/PublicPrivateServer.xml $MAINPATH/PublicPrivateServerError.xml "subgraph["$4"]"
fi
