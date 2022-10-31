#!/bin/bash

INITIALREF=${1}_tf
FINALREF="${1}_DNN"

echo $INITIALREF
echo $FINALREF

. clean_tau.sh
root -e ".L Tau.h+"  'UpdateTauDNN.C("'$INITIALREF'", "'$FINALREF'")'
