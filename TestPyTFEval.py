import numpy as np
import tensorflow as tf


class TestPyTFEval: 
    def __init__( self ):
        print('Initialising TFEvaluation')

        self.savedmodel = "../../data/batchsize_10/serialized"

        self.BATCHSIZE=10
        self.NUM_POINT = 20
        
    def pyArray (self, a):
    	print ("Contents of a :")
    	print (a)
    	c = 0
    	return c
