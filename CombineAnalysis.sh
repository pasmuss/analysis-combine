root -l -q AnalysisWorkspace.C
#root -l -q AnalysisWorkspaceSR2.C
#root -l -q AnalysisWorkspaceSR3.C
#root -l -q AnalysisWorkspaceSR4.C

for mass in 300 350 400 #450 500 600 700 800 900 1000 1200 1400 1600
do
    cp datacard_Analysis_toySR_template.txt datacard_Analysis_toySR_${mass}GeV.txt
    sed -i "s/MASS/${mass}/g" datacard_Analysis_toySR_${mass}GeV.txt
    text2workspace.py datacard_Analysis_toySR_${mass}GeV.txt --X-allow-no-signal --channel-masks
    combine -M AsymptoticLimits --rMin=-20 --rMax=20 -n Hbb -m $mass --cminDefaultMinimizerStrategy 0 datacard_Analysis_toySR_${mass}GeV.root
    sleep 2
done
