limit_file=Hbb.limits
[[ -f "$limit_file" ]] && rm "$limit_file"

#root -l -q AnalysisWorkspace.C
#root -l -q AnalysisWorkspaceSR2.C
#root -l -q AnalysisWorkspaceSR3.C
#root -l -q AnalysisWorkspaceSR4.C

for subrange in SR1 #SR2 #SR3 SR4
do
    if [ "$subrange" = "SR1" ]; then masses=(300 350 400); fi
    if [ "$subrange" = "SR2" ]; then masses=(400 450 500 600 700); fi
    if [ "$subrange" = "SR3" ]; then masses=(700 800 900 1000); fi
    if [ "$subrange" = "SR4" ]; then masses=(1000 1200 1400 1600); fi
    echo "$subrange, masses ${masses[@]}"
    for mass in ${masses[@]}
    do
	mkdir "${subrange}_${mass}"
	cp datacard_Analysis_toySR_template.txt datacard_Analysis_toy_${subrange}_${mass}GeV.txt
	sed -i "s/MASS/${mass}/g" datacard_Analysis_toy_${subrange}_${mass}GeV.txt
	sed -i "s/SUBRANGE/${subrange}/g" datacard_Analysis_toy_${subrange}_${mass}GeV.txt
	echo "Datacard $mass GeV, $subrange created"
	text2workspace.py datacard_Analysis_toy_${subrange}_${mass}GeV.txt --X-allow-no-signal --channel-masks
	echo "Starting combine"
	combine -M GenerateOnly --setParameters mask_SR=1 -t -1 --toysFrequentist --expectSignal 0 --saveToys --rMin=-20 --rMax=20 -n Hbb_${subrange} -m $mass datacard_Analysis_toy_${subrange}_${mass}GeV.root
	echo "Toy generation $mass GeV, $subrange done"
#	combine -M FitDiagnostics -t -1 --toysFile higgsCombineHbb_${subrange}.GenerateOnly.mH${mass}.123456.root --toysFrequentist --rMin=-20 --rMax=20 -n Hbb_${subrange} -m $mass --cminDefaultMinimizerStrategy 0 -d datacard_Analysis_toy_${subrange}_${mass}GeV.root -v 5 --plots --saveShapes >> "${subrange}_${mass}/${subrange}_${mass}.log"
#	mv *png "${subrange}_${mass}"
	combine -M AsymptoticLimits -t -1 --toysFile higgsCombineHbb_${subrange}.GenerateOnly.mH${mass}.123456.root --toysFrequentist --rMin=-5 --rMax=5 -n Hbb_${subrange} -m $mass --cminDefaultMinimizerStrategy 0 -d datacard_Analysis_toy_${subrange}_${mass}GeV.root
	mv datacard_Analysis_toy_${subrange}_${mass}GeV.* "${subrange}_${mass}"
	root_name=`readlink -f "higgsCombineHbb_${subrange}.AsymptoticLimits.mH${mass}.root"`
	echo "$root_name" >> "$limit_file"
	echo "$mass GeV $subrange finished"
    done
done

echo "Merge Combination output:"
hadd -f combineMerge.root higgsCombineHbb_SR1.AsymptoticLimits.mH*.root
echo "Merge Done: combineMerge.root was created"

cd /afs/desy.de/user/a/asmusspa/Documents/CMSSW_9_2_15/src; eval `scramv1 runtime -sh`; cd -
PlotLimit -i Hbb.limits
cd /afs/desy.de/user/a/asmusspa/Documents/CMSSW_10_2_13/src; cmsenv; cd -

#perhaps to be made: directories for tables, plots and output + move of files to these directories
