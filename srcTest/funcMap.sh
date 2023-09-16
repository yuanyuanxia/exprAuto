#!/bin/bash

# 定义关联数组，模拟map
declare -A bit_range=(
    ["sin"]=7
    ["cos"]=7
    ["tan"]=7
    ["exp"]=7
    ["exp2"]=7
    ["exp10"]=7
    ["log"]=7
    ["log2"]=7
    ["log10"]=7
    ["asinh"]=7
    ["acosh"]=7
    ["atanh"]=7
    ["asin"]=7
    ["acos"]=7
    ["atan"]=7
    ["sinh"]=7
    ["cosh"]=7
    ["tanh"]=7
)
declare -A fnum_range=(
    ["sin"]=1
    ["cos"]=1
    ["tan"]=1
    ["exp"]=1
    ["exp2"]=1
    ["exp10"]=1
    ["log"]=1
    ["log2"]=1
    ["log10"]=1
    ["asinh"]=1
    ["acosh"]=1
    ["atanh"]=1
    ["asin"]=1
    ["acos"]=1
    ["atan"]=1
    ["sinh"]=1
    ["cosh"]=1
    ["tanh"]=1
)
declare -A degree_range=(
    ["sin"]=7
    ["cos"]=7
    ["tan"]=7
    ["exp"]=8
    ["exp2"]=8
    ["exp10"]=8
    ["log"]=8
    ["log2"]=8
    ["log10"]=8
    ["asinh"]=8
    ["acosh"]=8
    ["atanh"]=8
    ["asin"]=8
    ["acos"]=8
    ["atan"]=8
    ["sinh"]=8
    ["cosh"]=8
    ["tanh"]=8
)

# 打印map中的值
# echo "bit_range:"
# for key in "${!bit_range[@]}"; do
#     echo "$key: ${bit_range[$key]}"
# done

# echo "fnum_range:"
# for key in "${!fnum_range[@]}"; do
#     echo "$key: ${fnum_range[$key]}"
# done

# echo "degree_range:"
# for key in "${!degree_range[@]}"; do
#     echo "$key: ${degree_range[$key]}"
# done