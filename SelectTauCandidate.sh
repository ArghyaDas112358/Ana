#!/bin/bash

INITIALREF=${1}_tf
FINALREF="${1}_DNN"
VERSION=""

if [[ $# -ge 2 ]]; then
	VERSION=${2}
fi

echo $INITIALREF
echo $FINALREF

. clean_tau.sh
root -e ".L Tau.h+"  'UpdateTauDNN.C("'$INITIALREF'", "'$FINALREF'", "'$VERSION'")'
