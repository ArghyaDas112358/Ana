
. fetchstuff.sh
#combine -M FitDiagnostics datacard.txt --saveWithUncertainties --saveOverallShapes --saveNormalizations --saveWorkspace --plots
#combine -M FitDiagnostics datacard.txt
combine -M MultiDimFit datacard.txt --saveWorkspace
#combine -M MultiDimFit higgsCombineTest.MultiDimFit.mH120.root --snapshotName MultiDimFit --saveWorkspace -t 100 --expectSignal 1. --toysNoSystematics
combine -M FitDiagnostics higgsCombineTest.MultiDimFit.mH120.root --snapshotName MultiDimFit --saveWorkspace -t 100 --expectSignal 1. --toysNoSystematics
