cd ..
./detectErrorOneFPEDParallel.sh NMSEproblem345 0.01 100 500000 0 0.01 0.00019998 expr_NMSEproblem345 0.01_100_500000 origin 0
cd - >/dev/null
./testPerformanceOneManual.sh NMSEproblem345 origin 0.01 100

cd ..
./detectErrorOneFPEDParallel.sh NMSEexample34 0.01 3 500000 0 0.01 5.98e-06 expr_NMSEexample34 0.01_3_500000 origin 0
cd - >/dev/null
./testPerformanceOneManual.sh NMSEexample34 origin 0.01 3

cd ..
./detectErrorOneFPEDParallel.sh logexp -8 8 500000 0 -8 3.2e-05 expr_logexp -8_8_500000 origin 0
cd - >/dev/null
./testPerformanceOneManual.sh logexp origin -8 8

cd ..
./detectErrorOneFPEDParallel.sh NMSEproblem343 -1 1 500000 0 -1 4e-06 expr_NMSEproblem343 -1_1_500000 origin 0
cd - >/dev/null
./testPerformanceOneManual.sh NMSEproblem343 origin -1 1

cd ..
./detectErrorOneFPEDParallel.sh exp1x 0.01 0.5 500000 0 0.01 9.8e-07 expr_exp1x 0.01_0.5_500000 origin 0
cd - >/dev/null
./testPerformanceOneManual.sh exp1x origin 0.01 0.5

cd ..
./detectErrorOneFPEDParallel.sh NMSEexample37 0.01 100 500000 0 0.01 0.00019998 expr_NMSEexample37 0.01_100_500000 origin 0
cd - >/dev/null
./testPerformanceOneManual.sh NMSEexample37 origin 0.01 100

cd ..
./detectErrorOneFPEDParallel.sh NMSEexample39 0.01 1 500000 0 0.01 1.98e-06 expr_NMSEexample39 0.01_1_500000 origin 0
cd - >/dev/null
./testPerformanceOneManual.sh NMSEexample39 origin 0.01 1

cd ..
./detectErrorOneFPEDParallel.sh NMSEproblem341 0.01 100 500000 0 0.01 0.00019998 expr_NMSEproblem341 0.01_100_500000 origin 0
cd - >/dev/null
./testPerformanceOneManual.sh NMSEproblem341 origin 0.01 100
