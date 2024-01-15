import ROOT


ROOT.gROOT.LoadMacro("FileFlow.h+")
from ROOT import Ana


data = "data" #"dataB2"

dataWS = "WS"

Sig = "Sig"

samples = [Sig, data, "B0toDstarDs", "B0toDstarDsstar", "B0toDstarD", "ButoDstarDK", "B0toDstarD0K", "B0toDstar3pi", dataWS] #"SigPart", "dataD2WS", "dataD2TauWS", , "B0toDstar5pi" ["Sig", "dataD1", "B0toDstarDs", "B0toDstarDsstar", "B0toDstarD", "ButoDstarDK", "B0toDstarD0K", "BkgDstara1", "B0toDstar3pi", "dataD2WS"]

regions = ["SR", "CR", "SB"]

variables = ["b_tau_rhomass1", "b_tau_rhomass2", "b_B_q2", "b_B_m", "b_tau_m"]

MVAsamples = ["Sig", "dataD1"] #, "dataA1"


def OpenFiles(version): 
	Ana.Init(version)
	for file in samples: 
		Ana.filemanager.OpenItem(file)

def CloseFiles(): 
	Ana.filemanager.CloseAll()

def DebugMode(): 
	global samples
	samples = ["Sig", "B0toDstarDs", "B0toDstarDsstar"] # Using a reduced set of files for debugging
	global regions
	regions = ["SR"]
	global variables
	variables = ["b_B_m"]

def Denominator(): 
	global Sig
	Sig = "B0toDstar3pi"
