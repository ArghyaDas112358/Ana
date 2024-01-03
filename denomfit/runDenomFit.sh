#!/bin/bash


python3 MakeFitModel.py
combine -M MultiDimFit datacard.txt --saveWorkspace --algo singles
combine -M FitDiagnostics higgsCombineTest.MultiDimFit.mH120.root --snapshotName MultiDimFit --saveWorkspace -t 100 --expectSignal 1. --toysNoSystematics
#python3 PlotDenomFit.py
