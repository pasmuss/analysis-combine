peak=240
for subrange in SR4
do
    limit_file=Hbb_limits_${subrange}_alluncert_exptoy_untouchedOffset
    [[ -f "$limit_file" ]] && rm "$limit_file"
    if [ "$subrange" = "SR4" ]
    then masses=(1000 1200 1400 1600)
	tail=-0.764043
	width=51.0459
	#root -l -q AnalysisWorkspaceSR4.C
    fi
    echo "$subrange, masses ${masses[@]}"
    for mass in ${masses[@]}
    do
	if [ "$subrange" = "SR4" ]
	then
	    if [ ${mass} = 1000 ]
            then bias=0.017; offbtag=1.129; onlbtag=1.073; kintrig=1.011; purew=1.001
            elif [ ${mass} = 1200 ]
            then bias=0.011; offbtag=1.124; onlbtag=1.083; kintrig=1.011; purew=1.000
            elif [ ${mass} = 1400 ]
            then bias=0.011; offbtag=1.119; onlbtag=1.094; kintrig=1.011; purew=1.001
            elif [ ${mass} = 1600 ]
            then bias=0.010; offbtag=1.116; onlbtag=1.105; kintrig=1.011; purew=1.000
            fi
	fi
	mkdir "${subrange}_${mass}_alluncert_exptoy_untouchedOffset"
	cp datacard_Analysis_toySR_template.txt datacard_Analysis_toy_${subrange}_${mass}GeV.txt
	sed -i "s/MASS/${mass}/g" datacard_Analysis_toy_${subrange}_${mass}GeV.txt
	sed -i "s/SUBRANGE/${subrange}/g" datacard_Analysis_toy_${subrange}_${mass}GeV.txt
	sed -i "s/BIASRESULT/${bias}/g" datacard_Analysis_toy_${subrange}_${mass}GeV.txt
	sed -i "s/OFFBT/${offbtag}/g" datacard_Analysis_toy_${subrange}_${mass}GeV.txt
	sed -i "s/ONLBT/${onlbtag}/g" datacard_Analysis_toy_${subrange}_${mass}GeV.txt
	sed -i "s/KINTO/${kintrig}/g" datacard_Analysis_toy_${subrange}_${mass}GeV.txt
	sed -i "s/PUVAL/${purew}/g" datacard_Analysis_toy_${subrange}_${mass}GeV.txt
	echo "Datacard $mass GeV, $subrange created"
	text2workspace.py datacard_Analysis_toy_${subrange}_${mass}GeV.txt --X-allow-no-signal --channel-masks
	mv datacard_Analysis_toy_${subrange}_${mass}GeV.* "${subrange}_${mass}_alluncert_exptoy_untouchedOffset"
	cd ${subrange}_${mass}_alluncert_exptoy_untouchedOffset
	echo "Starting combine ${subrange} ${mass}"
	combine -M MultiDimFit --setParameters mask_SR=1,r=0 -n Hbb_${subrange} -m ${mass} datacard_Analysis_toy_${subrange}_${mass}GeV.root  --freezeParameters r --redefineSignalPOIs tail --saveWorkspace  --cminDefaultMinimizerStrategy 0 --setParameterRanges r=-5,5:tail=-1,-0.7 #SR3/4: r[-1,1], POI tail
	echo "Snapshot generation $mass GeV, $subrange done"
#	combine -M FitDiagnostics -t -1 -d higgsCombineHbb_${subrange}.MultiDimFit.mH${mass}.root --snapshotName MultiDimFit --setParameters mask_SR=0,r=0,tail=$tail,width=$width,peak=$peak --redefineSignalPOIs r --setParameterRanges r=-0.5,0.5:tail=-1,-0.68:width=40,65:steepnesTF=-0.003,0.03 --freezeParameters offsetTF -n Hbb_${subrange} -m $mass --plots --cminDefaultMinimizerStrategy 0 -v5 > FD.log
	combine -M FitDiagnostics -t -1 -d higgsCombineHbb_${subrange}.MultiDimFit.mH${mass}.root --snapshotName MultiDimFit --expectSignal 1 --setParameters mask_SR=0,r=0,tail=$tail,width=$width,peak=$peak,offsetTF=250 --redefineSignalPOIs r --setParameterRanges r=-0.5,0.5:tail=-1,-0.68:width=40,65:steepnessTF=-0.003,0.03 --freezeParameters offsetTF -n Hbb_${subrange}_injSig -m $mass --plots --cminDefaultMinimizerStrategy 0 -v5 > FDSig.log
	echo "Asimov"
#	combine -M AsymptoticLimits -t -1 -d higgsCombineHbb_${subrange}.MultiDimFit.mH${mass}.root --snapshotName MultiDimFit --redefineSignalPOIs r --setParameterRanges r=-0.5,0.5:tail=-1,-0.68:width=40,65:steepnessTF=-0.003,0.03 --setParameters mask_SR=0,r=0,tail=$tail,width=$width,peak=$peak --freezeParameters offsetTF --cminDefaultMinimizerStrategy 0 -n Hbb_${subrange} -m ${mass} -v 5 > Asimov.log
	cd ..
#	root_name=`readlink -f "${subrange}_${mass}_alluncert_exptoy_untouchedOffset/higgsCombineHbb_${subrange}.AsymptoticLimits.mH${mass}.root"`
#	echo "$root_name" >> "$limit_file"
#	echo "$mass GeV $subrange finished"
    done
#    echo "Merge Combination output:"
#    hadd -f combineMerge_${subrange}.root ${subrange}*inclBiasAndLumi/higgsCombineHbb_${subrange}.AsymptoticLimits.mH*.root
#    echo "Merge Done: combineMerge_${subrange}.root was created"
#    cd /nfs/dust/cms/user/asmusspa/private/CMSSW_10_2_13/src; eval `scramv1 runtime -sh`; cd -
#    PlotLimit -i $limit_file
#    eval `scramv1 runtime -sh`
done

#SR 3: tail = -0.711103; width = 57.801, peak = 240
#SR 4: tail = -0.764043; width = 51.0459, peak = 240


#	combine -M MultiDimFit -t -1 -d higgsCombineHbb_${subrange}.MultiDimFit.mH${mass}.root --snapshotName MultiDimFit --setParameters mask_SR=0,r=0,tail=-0.764043,width=51.0459,peak=240 --cminDefaultMinimizerStrategy 0 --floatParameters peak --redefineSignalPOIs r --floatOtherPOIs 1 -n ${subrange}_fastscan_r_50 -m ${mass} --algo grid --points 50 --fastScan
#	combine -M MultiDimFit -t -1 -d higgsCombineHbb_${subrange}.MultiDimFit.mH${mass}.root --snapshotName MultiDimFit --setParameters mask_SR=0,r=0,tail=-0.764043,width=51.0459,peak=240 --cminDefaultMinimizerStrategy 0 --floatParameters peak --redefineSignalPOIs tail --floatOtherPOIs 1 -n ${subrange}_fastscan_tail_50 -m ${mass} --algo grid --points 50 --fastScan
#	combine -M MultiDimFit -t -1 -d higgsCombineHbb_${subrange}.MultiDimFit.mH${mass}.root --snapshotName MultiDimFit --setParameters mask_SR=0,r=0,tail=-0.764043,width=51.0459,peak=240 --cminDefaultMinimizerStrategy 0 --floatParameters peak --redefineSignalPOIs width --floatOtherPOIs 1 -n ${subrange}_fastscan_width_50 -m ${mass} --algo grid --points 50 --fastScan

#	combine -M MultiDimFit -t -1 -d higgsCombineHbb_${subrange}.MultiDimFit.mH${mass}.root --snapshotName MultiDimFit --setParameters mask_SR=0,r=0,tail=-0.764043,width=51.0459,peak=240 --cminDefaultMinimizerStrategy 0 --floatParameters peak --redefineSignalPOIs r --floatOtherPOIs 1 -n ${subrange}_fastscan_r_100 -m ${mass} --algo grid --points 100 --fastScan
#	combine -M MultiDimFit -t -1 -d higgsCombineHbb_${subrange}.MultiDimFit.mH${mass}.root --snapshotName MultiDimFit --setParameters mask_SR=0,r=0,tail=-0.764043,width=51.0459,peak=240 --cminDefaultMinimizerStrategy 0 --floatParameters peak --redefineSignalPOIs tail --floatOtherPOIs 1 -n ${subrange}_fastscan_tail_100 -m ${mass} --algo grid --points 100 --fastScan
#	combine -M MultiDimFit -t -1 -d higgsCombineHbb_${subrange}.MultiDimFit.mH${mass}.root --snapshotName MultiDimFit --setParameters mask_SR=0,r=0,tail=-0.764043,width=51.0459,peak=240 --cminDefaultMinimizerStrategy 0 --floatParameters peak --redefineSignalPOIs width --floatOtherPOIs 1 -n ${subrange}_fastscan_width_100 -m ${mass} --algo grid --points 100 --fastScan

#	combine -M MultiDimFit -t -1 -d higgsCombineHbb_${subrange}.MultiDimFit.mH${mass}.root --snapshotName MultiDimFit --setParameters mask_SR=0,r=0,tail=-0.764043,width=51.0459,peak=240 --cminDefaultMinimizerStrategy 0 --floatParameters peak --redefineSignalPOIs r --floatOtherPOIs 1 -n ${subrange}_fastscan_r_500 -m ${mass} --algo grid --points 500 --fastScan
#	combine -M MultiDimFit -t -1 -d higgsCombineHbb_${subrange}.MultiDimFit.mH${mass}.root --snapshotName MultiDimFit --setParameters mask_SR=0,r=0,tail=-0.764043,width=51.0459,peak=240 --cminDefaultMinimizerStrategy 0 --floatParameters peak --redefineSignalPOIs tail --floatOtherPOIs 1 -n ${subrange}_fastscan_tail_500 -m ${mass} --algo grid --points 500 --fastScan
#	combine -M MultiDimFit -t -1 -d higgsCombineHbb_${subrange}.MultiDimFit.mH${mass}.root --snapshotName MultiDimFit --setParameters mask_SR=0,r=0,tail=-0.764043,width=51.0459,peak=240 --cminDefaultMinimizerStrategy 0 --floatParameters peak --redefineSignalPOIs width --floatOtherPOIs 1 -n ${subrange}_fastscan_width_500 -m ${mass} --algo grid --points 500 --fastScan
