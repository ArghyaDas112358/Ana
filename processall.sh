#!/bin/bash

VERSION=${1}

SAMPLELIST=( B0toDstarDs B0toDstarDsstar B0toDstarDs1 B0toDstarD0K B0toDstarD0Kstar ) #SigTrain  #Sig SigTest 

if [[ "${VERSION}" == "" ]]; then
	echo "ERROR: no version provided. Please provide the version of the samples as argument."
	return $?
fi

for ITEM in "${SAMPLELIST[@]}"; do
	. process.sh $ITEM $VERSION
done

