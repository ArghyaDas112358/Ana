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
data_set = f['data'][0:maxnum]
print('data_set =', data_set.shape) 
label_set = f['label'][0:maxnum]

# make predictions
for i in range(len(data_set[0:10])): 
    batch = data_set[i]
    batch = np.expand_dims(batch, axis=0)
    print(batch.shape)

    pred_array = evaluation.Evaluate(batch)

    print(pred_array)


# Test the legacy way 
for i in range(0, 100, 10): 
    batch = data_set[i:i+10]
    print(batch.shape)

    prediction = evaluation.NN_response(batch)

    print(prediction)




  