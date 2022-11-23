#!/usr/bin/env python

from __future__ import division, print_function

import os
from ROOT import RDataFrame, TFile
from uncertainties import ufloat
from uncertainties.umath import * 

def getEff(n, N): 
	eff = float(n)/float(N)
	#print eff
	err = sqrt(eff*(1.-eff)/float(N))
	#print err
	#return eff, err
	return ufloat(eff, err)


if __name__ == "__main__": 
	from argparse import ArgumentParser

	parser = ArgumentParser(description="GetEfficiency")
	parser.add_argument("filename", action="store", type=str, default="", help="Name of file")
	#parser.add_argument("-N", "--version", dest="iteration", action="store", type=int, default=0, help="Which iteration of inference")
	#parser.add_argument("-i", "--input", dest="input", action="store", type=str, default="file.root", help="Number of points to be tested")
	parser.add_argument("-o", "--object", dest="object", action="store", type=str, default="ntuplizer/EffCalc", help="Efficiency info object within file")

	options = parser.parse_args()


	file = TFile.Open(options.filename, "READ")

	frame = RDataFrame(file.Get(options.object))

	n = frame.Sum("numSelected").GetValue()

	N = frame.Sum("numTotal").GetValue()

	eff = getEff(n, N)

	#print("Number of selected events: {}/{}".format(n, N))

	print("Efficiency: {}".format(eff))

