#/bin/bash
set -x
uniqueLabel=${1}
cp ${uniqueLabel}/expr_${uniqueLabel}_origin.c ${uniqueLabel}/expr_${uniqueLabel}_origin_bkp.c
cp exp1x/myhead.h ${uniqueLabel}/