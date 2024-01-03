#!/bin/bash


python3 MakeFitModel.py
combine -M MultiDimFit datacard.txt --saveWorkspace --algo singles
python3 PlotDenomFit.py
