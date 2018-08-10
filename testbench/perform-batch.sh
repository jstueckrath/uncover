#!/bin/sh

# change working directory to script folder
MAINPATH=`dirname $(readlink -f $0)`
cd $MAINPATH

# create run-results folder if non-existant; this does not delete old run results
if [ ! -d "run-results" ]; then
  mkdir "run-results"
fi

# starts the given test case with the given parameters and stores a brief info of the run into the timelog
run_with_time(){
  date >> run-results/timelog.txt
  time -f "%C\nprocess time: %U\ntotal time: %E" ./perform-analysis.sh $@ 2>> run-results/timelog.txt
  echo "" >> run-results/timelog.txt
}

# same as run_with_time(), but the first parameter needs to be a number stating how often the test case should be run
runm_with_time() {
  TEMP=$1
  shift
  for i in `seq 1 $TEMP`;
  do
    run_with_time $@
  done 
}

# simply starts the given test case with the given parameters
run() {
  ./perform-analysis.sh $@
}

# same as run(), but the first parameter needs to be a number stating how often the test case should be run
runm() {
  TEMP=$1
  shift
  for i in `seq 1 $TEMP`;
  do
    run $@
  done 
}

# termination detection protocol with error
# minor ordering
runm_with_time 10 TerminationDetection faulty

# termination detection protocol with fixed error
# minor ordering
runm_with_time 10 TerminationDetection correct

# multi user access control system
# subgraph ordering; terminates unbounded
runm_with_time 10 MultiUserSystem 
runm_with_time 10 MultiUserSystem withswap
runm_with_time 10 MultiUserSystem withswapni

# backery algorithm for mutual exclusion
# subgraph ordering
#run_with_time Bakery uq

# dining philosophers
# subgraph ordering; universally quantified rules; terminates unbounded
runm_with_time 10 DiningPhilosophers

# leader election protocol
# minor ordering
for i in `seq 1 20`;
do
  run_with_time LeaderElection $i
done

# verification of public-private server communication with different path restrictions
# subgraph ordering; parameter is the path bound
for i in `seq 1 150`;
do
  run_with_time PublicPrivateServer $i
done

# verification of firewall security
run_with_time Firewall 4

# Burns mutual exclusion protocol on a path
#run_with_time BurnsMutEx
