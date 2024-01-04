#!/bin/bash


python3 MakeFitModel.py
combine -M MultiDimFit datacard.txt --saveWorkspace --algo singles --saveFitResult
PostFitShapesFromWorkspace -w higgsCombineTest.MultiDimFit.mH120.root --output postFitPlots.root --fitresult multidimfitTest.root:fit_mdf --postfit
python3 PlotDenomFit.py
