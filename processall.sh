#!/bin/bash

VERSION=${1}

SAMPLELIST=(Sig SigTrain SigTest B0toDstarDs B0toDstarDsstar B0toDstarDs1)

for ITEM in "${SAMPLELIST[@]}"; do
	. process.sh $ITEM $VERSION
done

