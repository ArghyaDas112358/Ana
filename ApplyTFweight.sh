#!/bin/bash

if [[ $ENVDEF == "MyMacOS" ]]; then
	NUMPYLIBRARY=/opt/local/Library/Frameworks/Python.framework/Versions/3.6/lib/python3.6/site-packages/numpy/core/include
elif [[ $ENVDEF == "T3PSI" ]]; then
	NUMPYLIBRARY=/t3home/mhuwiler/.local/lib/python3.6/site-packages/numpy/core/include
else 
	echo "Unknown environment "$ENVDEF
fi

if [[ $# == 0 ]]; then
	echo "ERROR: No argument provided. You need to at least provide the ref of the bare file."
	return 5
elif [[ $# > 2 ]]; then
	echo "WARNING: Too many arguments provided. Arguments after 2nd position will be ignored. "
fi 

INITIALREF=${1}
FINALREF="${INITIALREF}_DNN_test" # Adding suffix to the ref
#Remove suffix from string
#FINALREF=${INITIALREF%".root"}
MAXEVENTS=10
if [[ $# == 2 ]]; then
	FINALREF=${2}
fi

echo $INITIALREF
echo $FINALREF

#root -e 'gInterpreter->AddIncludePath("/opt/local/Library/Frameworks/Python.framework/Versions/3.6/lib/python3.6/site-packages/numpy/core/include");' TestApplyTFweightnew.C
root -e 'gInterpreter->AddIncludePath("'$NUMPYLIBRARY'");' 'ApplyTFweight.C("'$INITIALREF'", "'$FINALREF'", '$MAXEVENTS')'
#root -e 'gInterpreter->AddIncludePath("/opt/local/Library/Frameworks/Python.framework/Versions/3.6/lib/python3.6/site-packages/numpy/core/include");' ApplyTFweight.C
#root -e 'gInterpreter->AddIncludePath("/t3home/mhuwiler/.local/lib/python3.6/site-packages/numpy/core/include/");' ApplyTFweight.C
