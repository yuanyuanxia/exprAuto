cd ..
./detectErrorOneFPEDParallel.sh NMSEproblem345 0.01 100 500000 0 0.01 0.00019998 expr_NMSEproblem345 0.01_100_500000 origin 0
cd - >/dev/null
taskset -c 0 ./testPerformanceOneManual.sh NMSEproblem345 origin 0.01 100

cd ..
./detectErrorOneFPEDParallel.sh NMSEexample34 0.01 3 500000 0 0.01 5.98e-06 expr_NMSEexample34 0.01_3_500000 origin 0
cd - >/dev/null
taskset -c 0 ./testPerformanceOneManual.sh NMSEexample34 origin 0.01 3

cd ..
./detectErrorOneFPEDParallel.sh logexp -8 8 500000 0 -8 3.2e-05 expr_logexp -8_8_500000 origin 0
cd - >/dev/null
taskset -c 0 ./testPerformanceOneManual.sh logexp origin -8 8

cd ..
./detectErrorOneFPEDParallel.sh exp1x_log 0.01 0.5 500000 0 0.01 9.8e-07 expr_exp1x_log 0.01_0.5_500000 origin 0
cd - >/dev/null
taskset -c 0 ./testPerformanceOneManual.sh exp1x_log origin 0.01 0.5

cd ..
# ./detectErrorOneFPEDParallel.sh NMSEproblem343 -1 1 500000 0 -1 4e-06 expr_NMSEproblem343 -1_1_500000 origin 0
./detectErrorOneFPEDParallel.sh NMSEproblem343 -0.5 0.5 500000 0 -0.5 2e-06 expr_NMSEproblem343 -0.5_0.5_500000 origin 0
cd - >/dev/null
# taskset -c 0 ./testPerformanceOneManual.sh NMSEproblem343 origin -1 1
taskset -c 0 ./testPerformanceOneManual.sh NMSEproblem343 origin -0.5 0.5

cd ..
./detectErrorOneFPEDParallel.sh exp1x 0.01 0.5 500000 0 0.01 9.8e-07 expr_exp1x 0.01_0.5_500000 origin 0
cd - >/dev/null
taskset -c 0 ./testPerformanceOneManual.sh exp1x origin 0.01 0.5

cd ..
./detectErrorOneFPEDParallel.sh NMSEexample37 0.01 100 500000 0 0.01 0.00019998 expr_NMSEexample37 0.01_100_500000 origin 0
cd - >/dev/null
taskset -c 0 ./testPerformanceOneManual.sh NMSEexample37 origin 0.01 100

cd ..
./detectErrorOneFPEDParallel.sh NMSEexample39 0.01 1 500000 0 0.01 1.98e-06 expr_NMSEexample39 0.01_1_500000 origin 0
cd - >/dev/null
taskset -c 0 ./testPerformanceOneManual.sh NMSEexample39 origin 0.01 1

cd ..
./detectErrorOneFPEDParallel.sh NMSEproblem341 0.01 100 500000 0 0.01 0.00019998 expr_NMSEproblem341 0.01_100_500000 origin 0
cd - >/dev/null
taskset -c 0 ./testPerformanceOneManual.sh NMSEproblem341 origin 0.01 100

cd ..
# ./detectErrorOneFPEDParallel.sh NMSEexample310 -1 1 500000 0 -1 4e-06 expr_NMSEexample310 -1_1_500000 origin 0
./detectErrorOneFPEDParallel.sh NMSEexample310 -0.5 0.5 500000 0 -0.5 2e-06 expr_NMSEexample310 -0.5_0.5_500000 origin 0
cd - >/dev/null
# taskset -c 0 ./testPerformanceOneManual.sh NMSEexample310 -1 1
taskset -c 0 ./testPerformanceOneManual.sh NMSEexample310 origin -0.5 0.5

cd ..
./detectErrorOneFPEDParallel.sh NMSEexample35 0.01 100 500000 0 0.01 0.00019998 expr_NMSEexample35 0.01_100_500000 origin 0
cd - >/dev/null
taskset -c 0 ./testPerformanceOneManual.sh NMSEexample35 origin 0.01 100

cd ..
./detectErrorOneFPEDParallel.sh NMSEexample38 0.01 100 500000 0 0.01 0.00019998 expr_NMSEexample38 0.01_100_500000 origin 0
cd - >/dev/null
taskset -c 0 ./testPerformanceOneManual.sh NMSEexample38 origin 0.01 100

cd ..
./detectErrorOneFPEDParallel.sh NMSEproblem336 0.01 100 500000 0 0.01 0.00019998 expr_NMSEproblem336 0.01_100_500000 origin 0
cd - >/dev/null
taskset -c 0 ./testPerformanceOneManual.sh NMSEproblem336 origin 0.01 100

cd ..
./detectErrorOneFPEDParallel.sh NMSEproblem337 0.01 100 500000 0 0.01 0.00019998 expr_NMSEproblem337 0.01_100_500000 origin 0
cd - >/dev/null
taskset -c 0 ./testPerformanceOneManual.sh NMSEproblem337 origin 0.01 100

cd ..
./detectErrorOneFPEDParallel.sh NMSEproblem344 0.01 100 500000 0 0.01 0.00019998 expr_NMSEproblem344 0.01_100_500000 origin 0
cd - >/dev/null
taskset -c 0 ./testPerformanceOneManual.sh NMSEproblem344 origin 0.01 100

cd ..
./detectErrorOneFPEDParallel.sh NMSEsection311 0.01 100 500000 0 0.01 0.00019998 expr_NMSEsection311 0.01_100_500000 origin 0
cd - >/dev/null
taskset -c 0 ./testPerformanceOneManual.sh NMSEsection311 origin 0.01 100

cd ..
./detectErrorTwoFPEDParallel.sh theta 1 100 1 100 1024 1024 0 0 1 1 0.0966796875 0.0966796875 expr_theta 1_100_1_100_1024_1024 origin 0
cd - >/dev/null
taskset -c 0 ./testPerformanceTwoManual.sh theta origin 1 100 1 100

cd ..
./detectErrorTwoFPEDParallel.sh ComplexSinCos 0 1 0 1 1024 1024 0 0 0 0 0.0009765625 0.0009765625 expr_ComplexSinCos 0_1_0_1_1024_1024 origin 0
cd - >/dev/null
taskset -c 0 ./testPerformanceTwoManual.sh ComplexSinCos origin 0 1 0 1

cd ..
./detectErrorTwoFPEDParallel.sh NMSEexample33 0.01 1 0 100 1024 1024 0 0 0.01 0 0.000966796875 0.09765625 expr_NMSEexample33 0.01_1_0_100_1024_1024 origin 0
cd - >/dev/null
taskset -c 0 ./testPerformanceTwoManual.sh NMSEexample33 origin 0.01 1 0 100

cd ..
./detectErrorTwoFPEDParallel.sh NMSEproblem332 0.01 1 0 100 1024 1024 0 0 0.01 0 0.000966796875 0.09765625 expr_NMSEproblem332 0.01_1_0_100_1024_1024 origin 0
cd - >/dev/null
taskset -c 0 ./testPerformanceTwoManual.sh NMSEproblem332 origin 0.01 1 0 100

cd ..
./detectErrorTwoFPEDParallel.sh NMSEproblem335 0.01 1 0 100 1024 1024 0 0 0.01 0 0.000966796875 0.09765625 expr_NMSEproblem335 0.01_1_0_100_1024_1024 origin 0
cd - >/dev/null
taskset -c 0 ./testPerformanceTwoManual.sh NMSEproblem335 origin 0.01 1 0 100

cd ..
./detectErrorTwoFPEDParallel.sh NMSEsection35 0 1 0 1 1024 1024 0 0 0 0 0.0009765625 0.0009765625 expr_NMSEsection35 0_1_0_1_1024_1024 origin 0
cd - >/dev/null
taskset -c 0 ./testPerformanceTwoManual.sh NMSEsection35 origin 0 1 0 1

cd ..
./detectErrorTwoFPEDParallel.sh polarToCarthesianX 1 10 0 360 1024 1024 0 0 1 0 0.0087890625 0.3515625 expr_polarToCarthesianX 1_10_0_360_1024_1024 origin 0
cd - >/dev/null
taskset -c 0 ./testPerformanceTwoManual.sh polarToCarthesianX origin 1 10 0 360

cd ..
./detectErrorTwoFPEDParallel.sh polarToCarthesianY 1 10 0 360 1024 1024 0 0 1 0 0.0087890625 0.3515625 expr_polarToCarthesianY 1_10_0_360_1024_1024 origin 0
cd - >/dev/null
taskset -c 0 ./testPerformanceTwoManual.sh polarToCarthesianY origin 1 10 0 360

cd ..
./detectErrorTwoFPEDParallel.sh i6 0.1 10 -5 5 1024 1024 0 0 0.1 -5 0.00966796875 0.009765625 expr_i6 0.1_10_-5_5_1024_1024 origin 0
cd - >/dev/null
taskset -c 0 ./testPerformanceTwoManual.sh i6 origin 0.1 10 -5 5