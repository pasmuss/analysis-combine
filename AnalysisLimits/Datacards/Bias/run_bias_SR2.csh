#!/bin/csh -f

set ntoys=10
set ndirs=500
set index_toy=4
echo "++++++ Produce toys with pdfindex = " ${index_toy} "++++++++"
foreach mp(400 450 500 600 700)
    foreach mu(0 1 2 3)
	foreach index_fit(0 2)
	    mkdir newSignal_BIAS_mu${mu}_M-${mp}_pdftoy_${index_toy}_pdffit_${index_fit}_SR2
	    cd newSignal_BIAS_mu${mu}_M-${mp}_pdftoy_${index_toy}_pdffit_${index_fit}_SR2
	    echo Processing Mass point $mp GeV with injected signal strength $mu and fit index ${index_fit}
	    ../htc_all_SR2.csh $ntoys $ndirs $mu $mp $index_toy $index_fit
	    cd ..
	    echo "============================================="
	end
    end
end
