import os
import pandas as pd
import uproot



def loadSamples(features, eventFraction=-1) : 


	path = "/eos/home-m/mhuwiler/data/Analysis/v5/" #"../localdata/" #"/pnfs/psi.ch/cms/trivcat/store/user/mhuwiler/Analysis/submit/productionProductionFastApplyDnnNoDNNdebug/" #"/eos/home-m/mhuwiler/data/Analysis/v5/"


	if(eventFraction<0):
		numEvents=-1
	else : 
		numEvents=eventFraction

	print "Number of events: {}".format(numEvents)

	# Poweg to be used if available

	samples={}

	samples["signal"] = uproot.open(os.path.expandvars("/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/SignalOfficialMC50M_converted.root"))["tree"].pandas.df(branches=features, entrystop=numEvents) #Dict-like structure 
	samples["background"] = uproot.open(os.path.expandvars("/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/ParkingBPH1-3Run2018B_converted.root"))["tree"].pandas.df(branches=features, entrystop=numEvents)

	#samples["signalfloatgendstar"] = uproot.open(os.path.expandvars("/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/PrivateProductionGenDstar_converted.root"))["tree"].pandas.df(branches=features, entrystop=numEvents)
	#samples["signalfloat"] = uproot.open(os.path.expandvars("/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/SignalOfficialMC50M_converted.root"))["tree"].pandas.df(branches=features, entrystop=numEvents)
	#samples["backgroundfloat"] = uproot.open(os.path.expandvars("/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/ParkingBPH1-3Run2018B_converted.root"))["tree"].pandas.df(branches=features, entrystop=numEvents)
	#samples["backgroundfloatprevious"] = uproot.open(os.path.expandvars("/eos/home-m/mhuwiler/DoctoralThesis/Analysis/data/DataVeryLarge_converted.root"))["tree"].pandas.df(branches=features, entrystop=numEvents)
	

	return samples


