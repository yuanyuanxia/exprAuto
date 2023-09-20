#!/bin/bash

# 定义关联数组，模拟map
declare -A x0Starts=(
    ["NMSEproblem345"]=0.01
    ["NMSEexample34"]=0.01
    ["logexp"]=-8
    ["exp1x_log"]=0.01
    ["NMSEproblem343"]=-0.5 # -1
    ["exp1x"]=0.01
    ["NMSEexample37"]=0.01
    ["NMSEexample39"]=0.01
    ["NMSEproblem341"]=0.01
    ["NMSEexample310"]=-0.5 # -1
    ["NMSEexample35"]=0.01
    ["NMSEexample38"]=0.01
    ["NMSEproblem336"]=0.01
    ["NMSEproblem337"]=0.01
    ["NMSEproblem344"]=0.01
    ["NMSEsection311"]=0.01
    ["theta"]=1
    ["ComplexSinCos"]=0
    ["NMSEexample33"]=0.01
    ["NMSEproblem332"]=0.01
    ["NMSEproblem335"]=0.01
    ["NMSEsection35"]=0
    ["polarToCarthesianX"]=1
    ["polarToCarthesianY"]=1
    ["i6"]=0.1
)

declare -A x0Ends=(
    ["NMSEproblem345"]=100
    ["NMSEexample34"]=3
    ["logexp"]=8
    ["exp1x_log"]=0.5
    ["NMSEproblem343"]=0.5 # 1
    ["exp1x"]=0.5
    ["NMSEexample37"]=100
    ["NMSEexample39"]=1
    ["NMSEproblem341"]=100
    ["NMSEexample310"]=0.5 # 1
    ["NMSEexample35"]=100
    ["NMSEexample38"]=100
    ["NMSEproblem336"]=100
    ["NMSEproblem337"]=100
    ["NMSEproblem344"]=100
    ["NMSEsection311"]=100
    ["theta"]=100
    ["ComplexSinCos"]=1
    ["NMSEexample33"]=1
    ["NMSEproblem332"]=1
    ["NMSEproblem335"]=1
    ["NMSEsection35"]=1
    ["polarToCarthesianX"]=10
    ["polarToCarthesianY"]=10
    ["i6"]=10
)

declare -A stepX0s=(
    ["NMSEproblem345"]=0.00019998
    ["NMSEexample34"]=5.98e-06
    ["logexp"]=3.2e-05
    ["exp1x_log"]=9.8e-07
    ["NMSEproblem343"]=2e-06 # 4e-06
    ["exp1x"]=9.8e-07
    ["NMSEexample37"]=0.00019998
    ["NMSEexample39"]=1.98e-06
    ["NMSEproblem341"]=0.00019998
    ["NMSEexample310"]=2e-06 # 4e-06
    ["NMSEexample35"]=0.00019998
    ["NMSEexample38"]=0.00019998
    ["NMSEproblem336"]=0.00019998
    ["NMSEproblem337"]=0.00019998
    ["NMSEproblem344"]=0.00019998
    ["NMSEsection311"]=0.00019998
    ["theta"]=0.0966796875
    ["ComplexSinCos"]=0.0009765625
    ["NMSEexample33"]=0.000966796875
    ["NMSEproblem332"]=0.000966796875
    ["NMSEproblem335"]=0.000966796875
    ["NMSEsection35"]=0.0009765625
    ["polarToCarthesianX"]=0.0087890625
    ["polarToCarthesianY"]=0.0087890625
    ["i6"]=0.00966796875
)

declare -A x1Starts=(
    ["theta"]=1
    ["ComplexSinCos"]=0
    ["NMSEexample33"]=0
    ["NMSEproblem332"]=0
    ["NMSEproblem335"]=0
    ["NMSEsection35"]=0
    ["polarToCarthesianX"]=0
    ["polarToCarthesianY"]=0
    ["i6"]=-5
)

declare -A x1Ends=(
    ["theta"]=100
    ["ComplexSinCos"]=1
    ["NMSEexample33"]=100
    ["NMSEproblem332"]=100
    ["NMSEproblem335"]=100
    ["NMSEsection35"]=1
    ["polarToCarthesianX"]=360
    ["polarToCarthesianY"]=360
    ["i6"]=5
)

declare -A stepX1s=(
    ["theta"]=0.0966796875
    ["ComplexSinCos"]=0.0009765625
    ["NMSEexample33"]=0.0009765625
    ["NMSEproblem332"]=0.0009765625
    ["NMSEproblem335"]=0.0009765625
    ["NMSEsection35"]=0.0009765625
    ["polarToCarthesianX"]=0.3515625
    ["polarToCarthesianY"]=0.3515625
    ["i6"]=0.009765625
)

declare -A func1s=(
    ["NMSEproblem345"]=sin
    ["NMSEexample34"]=cos
    ["logexp"]=exp
    ["exp1x_log"]=exp
    ["NMSEproblem343"]=log
    ["exp1x"]=exp
    ["NMSEexample37"]=exp
    ["NMSEexample39"]=tan
    ["NMSEproblem341"]=cos
    ["NMSEexample310"]=log
    ["NMSEexample35"]=atan
    ["NMSEexample38"]=log
    ["NMSEproblem336"]=log
    ["NMSEproblem337"]=exp
    ["NMSEproblem344"]=exp
    ["NMSEsection311"]=exp
    ["theta"]=atan
    ["ComplexSinCos"]=exp
    ["NMSEexample33"]=sin
    ["NMSEproblem332"]=tan
    ["NMSEproblem335"]=cos
    ["NMSEsection35"]=exp
    ["polarToCarthesianX"]=cos
    ["polarToCarthesianY"]=sin
    ["i6"]=sin
)

declare -A func2s=(
    ["NMSEproblem345"]=tan
    ["NMSEexample34"]=sin
    ["logexp"]=log
    ["exp1x_log"]=log
    ["ComplexSinCos"]=sin
)