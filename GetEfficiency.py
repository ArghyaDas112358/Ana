#!/usr/bin/env python

import os
from ROOT import RDataFrame, TFile


file = TFile.Open("../../data/TestEffCalc.root", "READ")

frame = RDataFrame(file.Get("ntuplizer/EffCalc"))

n = frame.Sum("numSelected").GetValue()

N = frame.Sum("numTotal").GetValue()

print "Number of selected events: {}/{}".format(n, N)

print "Efficiency: {}".format(n/N)

