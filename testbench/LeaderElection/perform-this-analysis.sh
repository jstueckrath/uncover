#!/bin/sh
MAINPATH=`dirname $(readlink -f $0)`

echo "generating leader election GTS for "$4" processes"
$2 --scn=lerc $4 $3/LeaderElection.xml
echo "staring analysis"

# check if full results should be saved
if [ "$1" = "full" ];
then
  $2 --scn=backw $3/LeaderElection.xml $MAINPATH/LeaderElectionError.xml minor $3
else
  $2 --scn=backw $3/LeaderElection.xml $MAINPATH/LeaderElectionError.xml minor
fi
