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
)

declare -A x1Starts=(
    ["theta"]=1
    ["ComplexSinCos"]=0
)

declare -A x1Ends=(
    ["theta"]=100
    ["ComplexSinCos"]=1
)

declare -A stepX1s=(
    ["theta"]=0.0966796875
    ["ComplexSinCos"]=0.0009765625
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
)

declare -A func2s=(
    ["NMSEproblem345"]=tan
    ["NMSEexample34"]=sin
    ["logexp"]=log
    ["exp1x_log"]=log
    ["ComplexSinCos"]=sin
)