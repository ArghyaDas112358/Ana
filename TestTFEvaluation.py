#!/usr/bin/env python

from TFEvaluation import TFEvaluation


print("Testing TFEvaluation.py")


evaluation = TFEvaluation()

evaluation.Test()


# Test the function which takes a single row
import numpy as np
import h5py
print("Test placeholder")
f = h5py.File("/pnfs/psi.ch/cms/trivcat/store/user/mhuwiler/data/Analysis/taudnn/v10/test_TAU.h5", 'r')
maxnum = 100 #math.floor(len(f["data"])/10)*10
data_set = f['data'][0:1]
print('data_set =', data_set.shape) 
label_set = f['label'][0:1]

# make predictions
pred_array = evaluation.Evaluate(data_set)

print(pred_array)




  