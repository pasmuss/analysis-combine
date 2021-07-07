#!/bin/csh -f	
if ($#argv<4) then
    echo Need to give ntoys per dir, ndirs, signal strength and the masspoint mp for the corresponding datacard
    exit
    endif
    #+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    # 0- PREPARE SCRIPT TO SUBMIT 
    echo "-------------------------------------------------------------"
    set ntoys=$1
    set ndirs=$2
    set mu=$3
    set mp=$4
    set index_toy=$5
    set index_fit=$6
    #+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    # 1- SUBMIT JOBS
    echo "-------------------------------------------------------------"
    set datacard_dir="/nfs/dust/cms/user/asmusspa/private/CMSSW_10_2_13/src/Combine/bias/noOptions"
    set i=0
    set seed=123456 
    while ($i<$ndirs)
	set exedir = "HTC_$i"
	if (-d $exedir) then
	    echo "Similar jobs were already submitted. Move ot remove directories "
	    exit
        endif
	mkdir -p $exedir
	cd $exedir
	cp ${datacard_dir}/bias_datacard_templ.txt .
	mv bias_datacard_templ.txt bias_datacard.txt
	sed -i 's/SUBRANGE/SR4/g' bias_datacard.txt
	sed -i 's/MASS/'${mp}'/g' bias_datacard.txt
	sed -i 's/INDEX/'${index_toy}'/g' bias_datacard.txt
	echo "Mass $mp, no. $i of $ndirs, mu=$mu"
	cat > exe.sh <<EOF
cd ${datacard_dir}
source /cvmfs/cms.cern.ch/cmsset_default.sh
cmsenv
cd -
#Toy Generation with alternative function
combine bias_datacard.txt -M GenerateOnly -s $seed --setParameters pdf_index=$index_toy -t $ntoys --expectSignal $mu --saveToys -m $mp --freezeParameters pdf_index 
#Estimate bias; what would the fit look like if we used the actual function but reality was the alternative one?
combine bias_datacard.txt -M FitDiagnostics -s $seed --setParameters pdf_index=$index_fit,peak=240,tail=-0.764043,width=51.0459 --freezeParameters pdf_index,peak --setParameterRanges r=-10,10:tail=-1,-0.7:width=40,65 --toysFile higgsCombineTest.GenerateOnly.mH${mp}.${seed}.root -m $mp -t $ntoys --cminDefaultMinimizerStrategy=0
EOF

	chmod u+x exe.sh
	../../htc_sub.sh exe.sh
	cd -
	@ i++
	@ seed++
    end
