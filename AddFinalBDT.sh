#!/bin/bash

INITIALREF=${1}_DNN
FINALREF=${1}
VERSION=""

if [[ $# -ge 2 ]]; then
	VERSION=${2}
fi

root -q -x 'AddFinalMVASimple.C("B0toDstarDs_SR", "something", "v6.95", "anaMVA/NewSelection/model_optimized/weights.xml")'
