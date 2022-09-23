#!/bin/bash

if [[ $ENVDEF == "MyMacOS" ]]; then
	NUMPYLIBRARY=/opt/local/Library/Frameworks/Python.framework/Versions/3.6/lib/python3.6/site-packages/numpy/core/include
elif [[ $ENVDEF == "T3PSI" ]]; then
	NUMPYLIBRARY=/t3home/mhuwiler/.local/lib/python3.6/site-packages/numpy/core/include
else 
	echo "Unknown environment "$ENVDEF
fi

#root -e 'gInterpreter->AddIncludePath("/opt/local/Library/Frameworks/Python.framework/Versions/3.6/lib/python3.6/site-packages/numpy/core/include");' TestApplyTFweightnew.C
root -e 'gInterpreter->AddIncludePath("'$NUMPYLIBRARY'");' ApplyTFweight.C
#root -e 'gInterpreter->AddIncludePath("/opt/local/Library/Frameworks/Python.framework/Versions/3.6/lib/python3.6/site-packages/numpy/core/include");' ApplyTFweight.C
#root -e 'gInterpreter->AddIncludePath("/t3home/mhuwiler/.local/lib/python3.6/site-packages/numpy/core/include/");' ApplyTFweight.C
