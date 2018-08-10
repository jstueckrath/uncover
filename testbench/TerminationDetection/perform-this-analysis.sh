#!/bin/sh

MAINPATH=`dirname $(readlink -f $0)`

# check if full results should be saved
if [ "$1" = "full" ];
then
  STORAGEFOLDER=$3
else
  STORAGEFOLDER=""
fi

if [ "$4" = "faulty" ];
then
  echo starting analysis of faulty termination detection protocol
  $2 --scn=backw $MAINPATH/TerminationDetectionFaulty.xml $MAINPATH/TerminationDetectionError.xml minor $STORAGEFOLDER
else
  echo starting analysis of corrected termination detection protocol
  $2 --scn=backw $MAINPATH/TerminationDetectionCorrect.xml $MAINPATH/TerminationDetectionError.xml minor $STORAGEFOLDER
fi
