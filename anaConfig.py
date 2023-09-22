import ROOT


ROOT.gROOT.LoadMacro("FileFlow.h+")
from ROOT import Ana


filesUsed = ["Sig", "dataB2", "B0toDstarDs", "B0toDstarDsstar", "B0toDstarD", "ButoDstarDK", "B0toDstarD0K", "B0toDstar3pi", "dataD2WS"] #"SigPart", "dataD2WS", "dataD2TauWS", , "B0toDstar5pi" ["Sig", "dataD1", "B0toDstarDs", "B0toDstarDsstar", "B0toDstarD", "ButoDstarDK", "B0toDstarD0K", "BkgDstara1", "B0toDstar3pi", "dataD2WS"]

regions = ["SR", "CR", "SB"]

variables = ["b_tau_rhomass1", "b_tau_rhomass2", "b_B_q2", "b_B_m", "b_tau_m"]

