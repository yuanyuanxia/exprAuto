uniqueLabel=logexp
if [ $# -eq 1 ]; then
    uniqueLabel=$1
fi
sed -n '/double irram_sintan/,/^}/{p}' input/${uniqueLabel}_opt.cc > middle.cpp
cat  prefix.cpp middle.cpp suffix.cpp > ${uniqueLabel}.tmp.cpp
sed -i  "s/irram_sintan/expr_${uniqueLabel}_CPP/g" ${uniqueLabel}.tmp.cpp
sed -i  "s/replacement/${uniqueLabel}/g" ${uniqueLabel}.tmp.cpp
cp ${uniqueLabel}.tmp.cpp ../srcTest/${uniqueLabel}/expr_${uniqueLabel}_NumOpt.cpp
mv ${uniqueLabel}.tmp.cpp ./output/expr_${uniqueLabel}_NumOpt.cpp
rm middle.cpp
