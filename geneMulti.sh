#!/bin/bash
# Description: generate multi dimension data
# Usage: ./geneDataMulti.exe {x0Start} {x0End} {x1Start} {x1End} {x2Start} {x2End}
gcc geneDataMulti.c -IincludeTEST -o geneDataMulti.exe
./geneDataMulti.exe 1 2 3 4 5 6

# devide inputData.txt into many files
split -l 16384 -d -a 2 inputData.txt inputData_
# add ".txt" to files
find . -name "inputData_*" | xargs -i mv {} {}.txt