#!/bin/bash


BKGLIST=( B0toDstarDs B0toDstarDsstar B0toDstarDs1 B0toDstarD0K B0toDstarD0Kstar B0toDstarD B0toDstara1 B0toDstar3pi B0toDstar5pi ButoDstarXc ButoDstarDK ) #SigTrain  #Sig "SigTest" 
SIGNALLIST=( Sig SigTest )
DATALIST=( dataB2 dataD1 dataD2)
WSLIST=( dataD1WS dataD2WS )

SAMPLELIST=( "${BKGLIST[@]}" "${SIGNALLIST[@]}" )
#shift
while [[ $1 =~ "--" ]]; do # Looping over all arguments, see shift
	if [[ $1 == "--BKG" ]]; then 
		SAMPLELIST=( "${BKGLIST[@]}" )
	elif [[ $1 == "--MC" ]]; then 
		SAMPLELIST=( "${BKGLIST[@]}" "${SIGNALLIST[@]}" )
	elif [[ $1 == "--SIG" ]]; then 
		SAMPLELIST=( "${SIGNALLIST[@]}" )
	elif [[ $1 == "--DATA" ]]; then 
		SAMPLELIST=( "${DATALIST[@]}" )
	elif [[ $1 == "--WS" ]]; then 
		SAMPLELIST=( "${WSLIST[@]}" )
	elif [[ $1 == "--DATAWS" ]]; then 
		SAMPLELIST=( "${DATALIST[@]}" "${WSLIST[@]}" )
	elif [[ $1 == "--ALL" ]]; then 
		SAMPLELIST=( "${BKGLIST[@]}" "${SIGNALLIST[@]}" "${DATALIST[@]}" "${WSLIST[@]}" )
	elif [[ $1 == "--MIN" ]]; then 
		SAMPLELIST=( "${BKGLIST[@]}" Sig dataB2 dataD2WS )
	elif [[ $1 == "--TEST" ]]; then 
		SAMPLELIST=( B0toDstarDs B0toDstara1 )
	elif [[ $1 == "--STEP" ]]; then 
		shift
		STAGE=$1
	else 
		echo "WARNING: Unknown argument: $1"
	fi

	shift # Shifting down all command line arguments by 1 entry

done

# if [[ "${VERSION}" == "" ]]; then
# 	echo "ERROR: no version provided. Please provide the version of the samples as argument."
# 	return $?
# fi

DATESTRING=$(date '+%Y_%m_%d__%H_%M_%S')
echo $DATESTRING >> failprocessing.txt

for ITEM in "${SAMPLELIST[@]}"; do
	#echo $ITEM
	echo "$ITEM"
	. AddFinalBDT.sh $ITEM ../../data/v6.95/SR anaMVA/MVAinSBdata/model_optimized/weights.xml
	RETURNCODE=$?
	if [ $RETURNCODE -ne 0 ]; then 
		echo $ITEM >> failprocessing.txt
	fi
done

