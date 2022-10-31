#!/bin/bash

INITIALREF=${1}_DNN
FINALREF=${1}_MVA

root -x 'AddMVAVariableSimple.C("'$INITIALREF'", "'$FINALREF'", "anaMVA/FirstNewTraining/model_optimized/weights.xml")'
