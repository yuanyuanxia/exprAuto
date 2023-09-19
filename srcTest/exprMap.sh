#!/bin/bash

# 定义关联数组，模拟map
declare -A x0Starts=(
    ["NMSEproblem345"]=0.01
    ["NMSEexample34"]=0.01
    ["logexp"]=-8
    ["NMSEproblem343"]=-1
    ["exp1x"]=0.01
    ["NMSEexample37"]=0.01
    ["NMSEexample39"]=0.01
    ["NMSEproblem341"]=0.01
)

declare -A x0Ends=(
    ["NMSEproblem345"]=100
    ["NMSEexample34"]=3
    ["logexp"]=8
    ["NMSEproblem343"]=1
    ["exp1x"]=0.5
    ["NMSEexample37"]=100
    ["NMSEexample39"]=1
    ["NMSEproblem341"]=100
)

declare -A stepX0s=(
    ["NMSEproblem345"]=0.00019998
    ["NMSEexample34"]=5.98e-06
    ["logexp"]=3.2e-05
    ["NMSEproblem343"]=4e-06
    ["exp1x"]=9.8e-07
    ["NMSEexample37"]=0.00019998
    ["NMSEexample39"]=1.98e-06
    ["NMSEproblem341"]=0.00019998
)

declare -A func1s=(
    ["NMSEproblem345"]=sin
    ["NMSEexample34"]=cos
    ["logexp"]=exp
    ["NMSEproblem343"]=log
    ["exp1x"]=exp
    ["NMSEexample37"]=exp
    ["NMSEexample39"]=tan
    ["NMSEproblem341"]=cos
)

declare -A func2s=(
    ["NMSEproblem345"]=tan
    ["NMSEexample34"]=sin
    ["logexp"]=log
)