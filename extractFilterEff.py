#!/usr/bin/env python
from __future__ import division, print_function

import pandas as pd
from uncertainties import ufloat
from uncertainties.umath import * 
from argparse import ArgumentParser
from libEfficiencies import getEff, DumpEffs, ReadEffs


def getFilterEffFromSummary(file): 
	frame = pd.read_csv(file, delimiter=" ", header=None)

	#print(frame)

	numbers = pd.to_numeric(frame.iloc[:,6].str.strip('()'))

	initial = pd.to_numeric(frame.iloc[:,8].str.strip('()'))

	#print(numbers)


	N = initial.sum()

	n = numbers.sum()


	eff = getEff(n, N)

	return eff


def updateEfficiency(eff, sample, file, force=False): 
	filterEffs = ReadEffs(file)
	print (filterEffs)

	if ((sample in filterEffs) and (not force)): 
		raise KeyError("The efficiency for {} already exists in the dictionary. Use option -f to overwrite.".format(sample))
	else: 
		filterEffs[sample] = eff
	DumpEffs(filterEffs, file)


if __name__ == "__main__": 

	parser = ArgumentParser(description="GetEfficiency")
	parser.add_argument("-i", "--file", dest="file", action="store", type=str, default="./summaryfiltereffs.txt", help="File from which to extract filter efficiency")
	parser.add_argument("-s", "--sample", dest="sample", action="store", type=str, default="", help="File from which to extract filter efficiency")
	parser.add_argument("-u", "--update", dest="update", action="store_true", default=False, help="Update filter efficiency in efficiency file ")
	parser.add_argument("-d", "--database", dest="database", action="store", type=str, default="/eos/home-m/mhuwiler/DoctoralThesis/Analysis/scripts/Ana/data/etc/FilterEfficiencies.json", help="Database file where to store the efficiency")
	parser.add_argument("-f", "--force", dest="force", action="store_true", default=False, help="Force updating efficiency if already present")
	parser.add_argument("-b", "--batch", dest="batch", action="store_true",default=False, help="Run in batch mode")

	options = parser.parse_args()


	eff = getFilterEffFromSummary(options.file)

	if (options.update): 
		assert(options.sample != ""), "ERROR: provided no sample name for updating the efficiency."
		updateEfficiency(eff, options.sample, options.database, options.force)


	print(eff)


	