#!/bin/bash

ARG=${1}
VERSION=""
STEP=0
STOPEVT=0

if [[ $# -ge 2 ]]; then
	VERSION=${2}
fi
if [[ $# -ge 3 ]]; then
	STEP=${3}
fi
if [[ $# -ge 4 ]]; then
	STOPEVT=${4}
fi

function processFull()
{

	if [ $4 -le 0 ]; then
		. ApplyTFweight.sh $1 $2 $3

		if [ $? -ne 0 ]; then
			echo "Abort due to error. "
			return $?
		fi
	fi

	if [ $4 -le 1 ]; then
		. SelectTauCandidate.sh $1 $2

		if [ $? -ne 0 ]; then
			echo "Abort due to error. "
			return $?
		fi
	fi

	if [ $4 -le 2 ]; then
		. AddAnaBDT.sh $1 $2
		sleep 2
		./anaMVA/inference/ApplyXGBOweight.sh $1 $2

		if [ $? -ne 0 ]; then
			echo "Abort due to error. "
			return $?
		fi
	fi

	if [ $4 -ge 3 ]; then
		echo "ERROR: Only 3 steps. Starting from step $4 will have no effect. "
	fi

}

# Main 

processFull $ARG $VERSION $STOPEVT $STEP

