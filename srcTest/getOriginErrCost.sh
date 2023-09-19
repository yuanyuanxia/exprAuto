cd ..
./detectErrorOneFPEDParallel.sh NMSEproblem345 0.01 100 500000 0 0.01 0.00019998 expr_NMSEproblem345 0.01_100_500000 origin 0
cd - >/dev/null
./testPerformanceOneManual.sh NMSEproblem345 origin 0.01 100

cd ..
./detectErrorOneFPEDParallel.sh exp1x 0.01 0.5 500000 0 0.01 9.8e-07 expr_exp1x 0.01_0.5_500000 origin 0
cd - >/dev/null
./testPerformanceOneManual.sh exp1x origin 0.01 0.5

cd ..
./detectErrorOneFPEDParallel.sh NMSEexample34 0.01 3 500000 0 0.01 5.98e-06 expr_NMSEexample34 0.01_3_500000 origin 0
cd - >/dev/null
./testPerformanceOneManual.sh NMSEexample34 origin 0.01 3

