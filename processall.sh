#!/bin/bash

VERSION=${1}

STAGE=0

BKGLIST=( B0toDstarDs B0toDstarDsstar B0toDstarDs1 B0toDstarD0K B0toDstarD0Kstar B0toDstarD B0toDstara1 B0toDstar3pi B0toDstar5pi ButoDstarXc ButoDstarDK ) #SigTrain  #Sig "SigTest" 
SIGNALLIST=( Sig SigTest )
DATALIST=( dataB2 dataD1 dataD2)
WSLIST=( dataD1WS dataD2WS )

SAMPLELIST=( ) #( "${BKGLIST[@]}" "${SIGNALLIST[@]}" )
WSPROCLIST=( ) #( "${WSLIST[@]}" )
DATAPROCESSED=0
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
		DATAPROCESSED=1
	elif [[ $1 == "--WS" ]]; then 
		WSPROCLIST=( "${WSLIST[@]}" )
	elif [[ $1 == "--ALL" ]]; then 
		SAMPLELIST=( "${BKGLIST[@]}" "${SIGNALLIST[@]}" "${DATALIST[@]}" )
		DATAPROCESSED=1
		WSPROCLIST=( "${WSLIST[@]}" )
	elif [[ $1 == "--MIN" ]]; then 
		SAMPLELIST=( "${BKGLIST[@]}" Sig dataB2 )
		WSPROCLIST=( dataD2WS )
	elif [[ $1 == "--TEST" ]]; then 
		SAMPLELIST=( B0toDstarDs B0toDstara1 )
		WSPROCLIST=( dataD2WS )
	elif [[ $1 == "--STEP" ]]; then 
		shift
		STAGE=$1
	else 
		echo "WARNING: Unknown argument: $1"
	fi

	shift # Shifting down all command line arguments by 1 entry

done

if [[ "${VERSION}" == "" ]]; then
	echo "ERROR: no version provided. Please provide the version of the samples as argument."
	return $?
fi

echo "Start processing the following sample list: ${SAMPLELIST[@]}"
echo "and the following samples with wrong sign: ${WSPROCLIST[@]}"
DATESTRING=$(date '+%Y_%m_%d__%H_%M_%S')
echo $DATESTRING >> failprocessing.txt

for ITEM in "${SAMPLELIST[@]}"; do
	#echo $ITEM
	. process.sh $ITEM $VERSION $STAGE
	RETURNCODE=$?
	if [ $RETURNCODE -ne 0 ]; then 
		echo $ITEM >> failprocessing.txt
	fi
done

if (( $DATAPROCESSED )) && [[ $STAGE -lt 1 ]]; then
	STAGE=1
fi
for ITEM in "${WSPROCLIST[@]}"; do
	#echo $ITEM
	. process.sh $ITEM $VERSION $STAGE 0 1
	RETURNCODE=$?
	if [ $RETURNCODE -ne 0 ]; then 
		echo $ITEM >> failprocessing.txt
	fi
done

