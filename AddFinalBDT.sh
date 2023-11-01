#!/bin/bash

INITIALREF=${1}_SR
FINALREF=${1}_AR
VERSION=""

if [[ $# -ge 2 ]]; then
	VERSION=${2}
fi

root -q -x 'AddFinalMVASimple.C("'$INITIALREF'", "'$FINALREF'", "'$VERSION'", "anaMVA/NewSelection/model_optimized/weights.xml")'
