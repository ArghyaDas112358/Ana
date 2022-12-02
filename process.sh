#!/bin/bash

ARG=${1}
VERSION=""
STOPEVT=0

if [[ $# -ge 2 ]]; then
	VERSION=${2}
fi
if [[ $# -ge 3 ]]; then
	STOPEVT=${3}
fi

function processFull()
{

	. ApplyTFweight.sh $1 $2 $3

	if [ $? -ne 0 ]; then
		echo "Abort due to error. "
		return $?
	fi

	. SelectTauCandidate.sh $1 $2

	if [ $? -ne 0 ]; then
		echo "Abort due to error. "
		return $?
	fi

	. AddAnaBDT.sh $1 $2

	if [ $? -ne 0 ]; then
		echo "Abort due to error. "
		return $?
	fi

}

# Main 

processFull $ARG $VERSION $STOPEVT

