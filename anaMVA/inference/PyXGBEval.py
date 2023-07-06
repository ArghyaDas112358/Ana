from __future__ import division, print_function
import numpy as np
import pandas as pd
import ctypes
import math
import h5py
import xgboost as xgb
import pickle


class PyXGBEval: 
    def __init__( self ):
        print('Initialising XGBEvaluation')

        self.savedmodel = "../../data/batchsize_10/serialized"

        #classifier = pickle.load(open(options.model, "rb"))

        self.debug = False     

    def Initialise(self, model): 
        self.savedmodel = model

        self.Init()

    def Init(self): 
        print("Using the following model: {}".format(self.savedmodel))

        self.classifier = pickle.load(open(self.savedmodel, "rb"))

        features_save = [("b_D0_pt", "F"), ("b_D0_eta", "F"), ("b_D0_phi", "F"), ("b_D0_vprob", "F"), ("b_D0_fl", "F"), ("b_D0_fsig", "F"),
           ("b_Ds_pt", "F"), ("b_Ds_eta", "F"), ("b_Ds_phi", "F"), ("b_Ds_vprob", "F"), ("b_Ds_fl", "F"), ("b_Ds_fsig", "F"), 
            ("b_D0_lip", "F"), ("b_D0_lips", "F"), ("b_D0_pvip", "F"), ("b_Ds_lip", "F"), ("b_Ds_lips", "F"), ("b_Ds_pvip", "F"), 
            ("b_tau_pt", "F"), ("b_tau_eta", "F"), ("b_tau_phi", "F"), ("b_tau_fl", "F"), ("b_tau_fsig", "F"), ("b_tau_vprob", "F"), 
            ("b_tau_lip", "F"), 
            #("b_tau_lips", "F"), 
            ("b_tau_pvip", "F"), ("b_tau_pvipsig", "F"), ("b_tau_alpha", "F"), ("b_tau_legacyMaxdr", "F"), 
            ("b_tau_pi1pt", "F"), ("b_tau_pi1eta", "F"), ("b_tau_pi1phi", "F"), 
            ("b_tau_pi2pt", "F"), ("b_tau_pi2eta", "F"), ("b_tau_pi2phi", "F"), 
            ("b_tau_pi3pt", "F"), ("b_tau_pi3eta", "F"), ("b_tau_pi3phi", "F"), ("b_tau_sumdnn", "F"), 
            #("BsDstarTauNu_k_charge", "F"), ("BsDstarTauNu_pi_charge", "F"), ("BsDstarTauNu_spi_charge", "F"), 
            ] #("BsDstarTauNu_mu1_vx", "F"), ("BsDstarTauNu_mu1_vy", "F"), ("BsDstarTauNu_tau_pi1_charge", "F"), ("BsDstarTauNu_tau_pi2_charge", "F"), ("BsDstarTauNu_tau_pi3_charge", "F"), ("BsDstarTauNu_mu1_q", "I"), ("BsDstarTauNu_tau_q", "I"), ("BsDstarTauNu_mu1_vz", "F")

        self.features = [item[0] for item in features_save]
        

    def Eval(self, data): 
        #print("Evaluation called")

        dataforconvert = pd.Series(data)

        dataForEval = xgb.DMatrix(dataforconvert, feature_names=self.features)

        return self.Evaluate(dataForEval)


    def Evaluate(self, data): 

        #print(data)
        
        result = self.classifier.predict(data)

        #print(result)

        return result


    def CheckInput(self, dataframe): 
        # open the file and check if the dataframes are  identical
        print("Check")
        print(dataframe)
        if (self.initCheck != True): 
            self.InitCheck("../../data/firstAllTauFix.h5")
        
        print('data_set =', self.dataset.shape) 
        element = self.dataset[self.count]
        element = np.expand_dims(element, 0)
        print(element)
        self.count+=1
        if (not np.allclose(dataframe, element)): #assert(np.allclose(dataframe, self.dataset[self.count]))
            print("Warning: different values in arrays!")
        print(dataframe - element)
        assert(np.allclose(dataframe, element))
        print(dataframe.dtype)
        print(element.dtype)


    def InitCheck(self, filename): 
        self.file = h5py.File(filename, 'r') 
        self.dataset = self.file['data']
        self.initCheck = True
        self.count = 0


    """
    def EvaluateBatch(self, batch): 
        with tf.Session(graph=tf.Graph()) as sess:
            tf.saved_model.loader.load(sess, [tf.saved_model.tag_constants.SERVING], self.savedmodel) #'../pretrained/{}'.format(FLAGS.name)
            output = sess.graph.get_tensor_by_name('Softmax_2:0') #Reshape_5:0
            
            #mock_data = np.ones((BATCHSIZE,NUM_POINT,NFEATURES),dtype=float)
            mock_label = np.ones((self.BATCHSIZE,self.NUM_POINT),dtype=float)
            #mock_glob = np.ones((self.BATCHSIZE,NGLOB),dtype=float)

            feed_dict = {
                'Placeholder:0': batch,
                'Placeholder_1:0': mock_label,
                'Placeholder_2:0': False,
            }

            predictions = sess.run(output, feed_dict)

            return predictions
    """


