cp /nfs/dust/cms/user/asmusspa/private/CMSSW_9_2_15/src/Analysis/Combine/Pulls/r/*txt .
python3 CalculatePulls.py
root -l -q PlotPulls.C
