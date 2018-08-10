#!/bin/sh
MAINPATH=`dirname $(readlink -f $0)`

# search for executable (build versions have priority)
if [ -x $MAINPATH/../build/uncover ];
then
  # starting newest build version (if working with IDE)
  EXECTHIS=$MAINPATH/../build/uncover
else
  # starting stored build (if working on server)
  EXECTHIS=$MAINPATH/uncover
fi

# check if full results should be saved
if [ "$1" = "full" ];
then
  FULL_RESULTS="full"
  # delete this parameter after use
  shift
else
  FULL_RESULTS="normal"
fi

# generate filename appendix with date and time
FNAPP=$(date '+%F') # add _%X to also concat the time to folder name
FNAPP=$(echo $FNAPP | tr : - )
FNAPP="_$FNAPP"

# search for the first available name for a result folder
COUNTER=1
while [ -x $MAINPATH/run-results/$1$FNAPP"_"$COUNTER ]; do
   COUNTER=`expr $COUNTER + 1`
done
RESULTDIR=$MAINPATH/run-results/$1$FNAPP"_"$COUNTER

# create a folder to store the results
mkdir $RESULTDIR
cd $RESULTDIR

# store problem name ( = its folder name) and delete parameter
PROBLEM_NAME=$1
shift

# perform the analysis
$MAINPATH/$PROBLEM_NAME/perform-this-analysis.sh $FULL_RESULTS $EXECTHIS $RESULTDIR "$@" 2>&1 | tee run.log

