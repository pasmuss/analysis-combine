for subrange in SR1
do
    limit_file=Hbb_limits_${subrange}_alluncert_smooth
    [[ -f "$limit_file" ]] && rm "$limit_file"
    if [ "$subrange" = "SR1" ]
    then masses=(300 350 400)
    fi
    echo "$subrange, masses ${masses[@]}"
    for mass in ${masses[@]}
    do
	if [ ${mass} = 300 ]
	then bias=1.116; offbtag=1.096; onlbtag=1.037; kintrig=1.016; purew=1.001
        elif [ ${mass} = 350 ]
	then bias=0.753; offbtag=1.093; onlbtag=1.039; kintrig=1.014; purew=1.002
	elif [ ${mass} = 400 ]
	then bias=0.45; offbtag=1.091; onlbtag=1.041; kintrig=1.013; purew=1.003
	fi
	mkdir "${subrange}_${mass}_alluncert_smooth"
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
	mv datacard_Analysis_toy_${subrange}_${mass}GeV.* "${subrange}_${mass}_alluncert_smooth"
	cd ${subrange}_${mass}_alluncert_smooth
	echo "Starting combine ${subrange} ${mass}"
#	combine -M MultiDimFit --setParameters mask_SR=1,r=0 -n Hbb_${subrange} -m ${mass} datacard_Analysis_toy_${subrange}_${mass}GeV.root  --freezeParameters r --redefineSignalPOIs turnon_novoeff --saveWorkspace  --cminDefaultMinimizerStrategy 0 #SR1/2: r[-20,20], POI turnon_novoeff
#	echo "Snapshot generation $mass GeV, $subrange done"
#	combine -M FitDiagnostics -t -1 -d higgsCombineHbb_${subrange}.MultiDimFit.mH${mass}.root --snapshotName MultiDimFit --setParameters mask_SR=0,r=0,bernstein_coefficient_00=2.28831,bernstein_coefficient_01=0.000193003,bernstein_coefficient_02=24.9699,bernstein_coefficient_03=0.799356,bernstein_coefficient_04=6.15331,bernstein_coefficient_05=2.15559,bernstein_coefficient_06=1.84779,turnon_novoeff=228.7,slope_novoeff=0.02 --freezeParameters bernstein_coefficient_02 --redefineSignalPOIs r -n Hbb_${subrange} --setParameterRanges r=-20,20:bernstein_coefficient_00=0,5:bernstein_coefficient_01=-1,1:bernstein_coefficient_02=20,30:bernstein_coefficient_03=0,2:bernstein_coefficient_04=2,10:bernstein_coefficient_05=0,10:bernstein_coefficient_06=0,5:turnon_novoeff=200,300:slope_novoeff=0,0.1:slopelinTF=-2e-4:2e-4 -m $mass --plots --cminDefaultMinimizerStrategy 0 -v 5 > FD.log
	combine -M FitDiagnostics -d datacard_Analysis_toy_${subrange}_${mass}GeV.root --setParameters mask_SR=0,r=0,bernstein_coefficient_00=2.28831,bernstein_coefficient_01=0.000193003,bernstein_coefficient_02=24.9699,bernstein_coefficient_03=0.799356,bernstein_coefficient_04=6.15331,bernstein_coefficient_05=2.15559,bernstein_coefficient_06=1.84779,turnon_novoeff=228.7,slope_novoeff=0.02 --freezeParameters bernstein_coefficient_02 --redefineSignalPOIs r -n Hbb_${subrange} --setParameterRanges r=-20,20:bernstein_coefficient_00=0,5:bernstein_coefficient_01=-1,1:bernstein_coefficient_02=20,30:bernstein_coefficient_03=0,2:bernstein_coefficient_04=2,10:bernstein_coefficient_05=0,10:bernstein_coefficient_06=0,5:turnon_novoeff=200,300:slope_novoeff=0,0.1 -m $mass --plots --saveShapes --saveWithUncertainties --rebinFactor 10 --cminDefaultMinimizerStrategy 0 -v 5 > FDplain.log
#	combine -M FitDiagnostics -d datacard_Analysis_toy_${subrange}_${mass}GeV.root --expectSignal 1 --setParameters mask_SR=0,r=0,bernstein_coefficient_00=2.28831,bernstein_coefficient_01=0.000193003,bernstein_coefficient_02=24.9699,bernstein_coefficient_03=0.799356,bernstein_coefficient_04=6.15331,bernstein_coefficient_05=2.15559,bernstein_coefficient_06=1.84779,turnon_novoeff=228.7,slope_novoeff=0.02 --freezeParameters bernstein_coefficient_02 --redefineSignalPOIs r -n Hbb_${subrange} --setParameterRanges r=-20,20:bernstein_coefficient_00=0,5:bernstein_coefficient_01=-1,1:bernstein_coefficient_02=20,30:bernstein_coefficient_03=0,2:bernstein_coefficient_04=2,10:bernstein_coefficient_05=0,10:bernstein_coefficient_06=0,5:turnon_novoeff=200,300:slope_novoeff=0,0.1 -m $mass --plots --cminDefaultMinimizerStrategy 0 -v 5 > FDplainSig.log
#	echo "Asimov"
#	combine -M AsymptoticLimits -t -1 -d higgsCombineHbb_${subrange}.MultiDimFit.mH${mass}.root --snapshotName MultiDimFit --redefineSignalPOIs r --setParameters mask_SR=0,r=0,bernstein_coefficient_00=2.28831,bernstein_coefficient_01=0.000193003,bernstein_coefficient_02=24.9699,bernstein_coefficient_03=0.799356,bernstein_coefficient_04=6.15331,bernstein_coefficient_05=2.15559,bernstein_coefficient_06=1.84779,turnon_novoeff=228.7,slope_novoeff=0.02 --freezeParameters bernstein_coefficient_02 --cminDefaultMinimizerStrategy 0 -n Hbb_${subrange} --setParameterRanges r=-20,20:bernstein_coefficient_00=0,5:bernstein_coefficient_01=-1,1:bernstein_coefficient_02=20,30:bernstein_coefficient_03=0,2:bernstein_coefficient_04=2,10:bernstein_coefficient_05=0,10:bernstein_coefficient_06=0,5:turnon_novoeff=200,300:slope_novoeff=0,0.1:slopelinTF=-2e-4,2e-4 -m ${mass} -v 5 > Asimov.log
#	combine -M AsymptoticLimits -d datacard_Analysis_toy_${subrange}_${mass}GeV.root --setParameters mask_SR=0,r=0,bernstein_coefficient_00=2.28831,bernstein_coefficient_01=0.000193003,bernstein_coefficient_02=24.9699,bernstein_coefficient_03=0.799356,bernstein_coefficient_04=6.15331,bernstein_coefficient_05=2.15559,bernstein_coefficient_06=1.84779,turnon_novoeff=228.7,slope_novoeff=0.02 --freezeParameters bernstein_coefficient_02 --cminDefaultMinimizerStrategy 0 -n Hbb_${subrange} --setParameterRanges r=-20,20:bernstein_coefficient_00=0,5:bernstein_coefficient_01=-1,1:bernstein_coefficient_02=20,30:bernstein_coefficient_03=0,2:bernstein_coefficient_04=2,10:bernstein_coefficient_05=0,10:bernstein_coefficient_06=0,5:turnon_novoeff=200,300:slope_novoeff=0,0.1 -m ${mass} -v 5 > Plain.log
	cd ..
	root_name=`readlink -f "${subrange}_${mass}_alluncert_smooth/higgsCombineHbb_${subrange}.AsymptoticLimits.mH${mass}.root"`
	echo "$root_name" >> "$limit_file"
	echo "$mass GeV $subrange finished"
    done
    echo "Merge Combination output:"
    hadd -f combineMerge_${subrange}.root ${subrange}_${mass}_alluncert_smooth/higgsCombineHbb_${subrange}.AsymptoticLimits.mH*.root
    echo "Merge Done: combineMerge_${subrange}.root was created"
    cd /nfs/dust/cms/user/asmusspa/private/CMSSW_10_2_13/src; eval `scramv1 runtime -sh`; cd -
    PlotLimit -i $limit_file
    eval `scramv1 runtime -sh`
done
