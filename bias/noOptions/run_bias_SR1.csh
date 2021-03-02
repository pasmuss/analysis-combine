#!/bin/csh -f

set ntoys=10
set ndirs=500
#set index_fit=2
set index_toy=5
echo "++++++ Produce toys with pdfindex = " ${index_toy} "++++++++"
foreach mp(300 350 400)
    foreach mu(0 1 2 3)
	foreach index_fit(0 2 3 4)
	    mkdir newSignal_BIAS_mu${mu}_M-${mp}_pdftoy_${index_toy}_pdffit_${index_fit}_SR1
	    cd newSignal_BIAS_mu${mu}_M-${mp}_pdftoy_${index_toy}_pdffit_${index_fit}_SR1
	    echo Processing Mass point $mp GeV with injected signal strength $mu and fit index ${index_fit}
	    ../htc_all_SR1.csh $ntoys $ndirs $mu $mp $index_toy $index_fit
	    cd ..
	    echo "============================================="
	end
    end
end
