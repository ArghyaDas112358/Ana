#!/bin/bash

VERSION=${1}

BKGLIST=( B0toDstarDs B0toDstarDsstar B0toDstarDs1 B0toDstarD0K B0toDstarD0Kstar B0toDstarD B0toDstara1 B0toDstar3pi B0toDstar5pi ButoDstarXc But0DstarDK ) #SigTrain  #Sig SigTest 
SIGNALLIST=( Sig SigTrain )
DATALIST=( dataB2 dataD1 dataD2WS )

SAMPLELIST=( "${BKGLIST[@]}" "${SIGNALLIST[@]}" )
shift
while [[ $1 =~ "--" ]]; do # Looping over all arguments, see shift
	if [[ $1 == "--BKG" ]]; then 
		SAMPLELIST=( "${BKGLIST[@]}" )
	elif [[ $1 == "--MC" ]]; then 
		SAMPLELIST=( "${BKGLIST[@]}" "${SIGNALLIST[@]}" )
	elif [[ $1 == "--SIG" ]]; then 
		SAMPLELIST=( "${SIGNALLIST[@]}" )
	elif [[ $1 == "--DATA" ]]; then 
		SAMPLELIST=( "${DATALIST[@]}" )
	elif [[ $1 == "--ALL" ]]; then 
		SAMPLELIST=( "${BKGLIST[@]}" "${SIGNALLIST[@]}" "${DATALIST[@]}" )
	else 
		echo "WARNING: Unknown argument: $1"
	fi

	shift # Shifting down all command line arguments by 1 entry

done

if [[ "${VERSION}" == "" ]]; then
	echo "ERROR: no version provided. Please provide the version of the samples as argument."
	return $?
fi

for ITEM in "${SAMPLELIST[@]}"; do
	#echo $ITEM
	. process.sh $ITEM $VERSION
done

