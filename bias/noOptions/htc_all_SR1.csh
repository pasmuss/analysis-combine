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
	sed -i 's/SUBRANGE/SR1/g' bias_datacard.txt
	sed -i 's/MASS/'${mp}'/g' bias_datacard.txt
	sed -i 's/INDEX/'${index_toy}'/g' bias_datacard.txt
	echo "Mass $mp, no. $i of $ndirs, mu=$mu"
	cat > exe.sh <<EOF
cd ${datacard_dir}
source /cvmfs/cms.cern.ch/cmsset_default.sh
cmsenv
cd -
#Toy Generation
combine bias_datacard.txt -M GenerateOnly -s $seed --setParameters pdf_index=$index_toy -t $ntoys --expectSignal $mu --saveToys -m $mp --freezeParameters pdf_index 
#Estimate bias with alternative function
combine bias_datacard.txt -M FitDiagnostics --plots -s $seed --setParameters pdf_index=$index_fit,bernstein_coefficient_00=2.28831,bernstein_coefficient_01=0.000193003,bernstein_coefficient_02=24.9699,bernstein_coefficient_03=0.799356,bernstein_coefficient_04=6.15331,bernstein_coefficient_05=2.15559,bernstein_coefficient_06=1.84779,turnon_novoeff=228.7,slope_novoeff=0.02 --freezeParameters bernstein_coefficient_02,pdf_index --setParameterRanges r=-20,20:bernstein_coefficient_00=0,5:bernstein_coefficient_01=-1,1:bernstein_coefficient_02=20,30:bernstein_coefficient_03=0,2:bernstein_coefficient_04=2,10:bernstein_coefficient_05=0,10:bernstein_coefficient_06=0,5:turnon_novoeff=200,300:slope_novoeff=0,0.1 --toysFile higgsCombineTest.GenerateOnly.mH${mp}.${seed}.root -m $mp -t $ntoys --cminDefaultMinimizerStrategy=0
EOF

	chmod u+x exe.sh
	../../htc_sub.sh exe.sh
	cd -
	@ i++
	@ seed++
    end
