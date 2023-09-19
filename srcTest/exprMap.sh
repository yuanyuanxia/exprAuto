#!/bin/bash

# 定义关联数组，模拟map
declare -A x0Starts=(
    ["NMSEproblem345"]=0.01
    ["exp1x"]=0.01
    ["NMSEexample34"]=0.01
)

declare -A x0Ends=(
    ["NMSEproblem345"]=100
    ["exp1x"]=0.5
    ["NMSEexample34"]=3
)

declare -A stepX0s=(
    ["NMSEproblem345"]=0.00019998
    ["exp1x"]=9.8e-07
    ["NMSEexample34"]=5.98e-06
)