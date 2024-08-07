#!/bin/bash
# NMSEproblem345 := (/ (- x (sin x)) (- x (tan x)))
# sin: [0.01, 100]
# tan: [0.01, 100]
start_time=$(date +%s.%n)
taskset -c 0 ./pareto_all.sh NMSEproblem345
end_time=$(date +%s.%N)
duration=$(echo "$end_time - $start_time" | bc)
echo "NMSEproblem345 time is ${duration} s." 
echo "NMSEproblem345 time is ${duration} s." >> hjwTime.log
## TGen
# taskset -c 0 python3 per_allversion.py sin 0.01 100
# taskset -c 0 python3 per_allversion.py tan 0.01 100

# NMSEexample34: (/ (- 1 (cos x)) (sin x))
# sin: [0.01, 3]
# cos: [0.01, 3]
start_time=$(date +%s.%n)
taskset -c 0 ./pareto_all.sh NMSEexample34
end_time=$(date +%s.%N)
duration=$(echo "$end_time - $start_time" | bc)
echo "NMSEexample34 time is ${duration} s." 
echo "NMSEexample34 time is ${duration} s." >> hjwTime.log
## TGen
# taskset -c 0 python3 per_allversion.py sin 0.01 3
# taskset -c 0 python3 per_allversion.py cos 0.01 3

# logexp := log (+ 1 (exp x))
# exp: [-8, 8]
# log: [1 + exp(-8), 1 + exp(8)] = [1.000335, 2,981.957987] \approx [1, 2982]
start_time=$(date +%s.%n)
taskset -c 0 ./pareto_all.sh logexp -8 8 1 2982
end_time=$(date +%s.%n)
duration=$(echo "$end_time - $start_time" | bc)
echo "logexp time is ${duration} s." 
echo "logexp time is ${duration} s." >> hjwTime.log
## TGen
# taskset -c 0 python3 per_allversion.py exp -8 8
# taskset -c 0 python3 per_allversion.py log 1 2982

# exp1x_log := ((exp(x) - 1.0) / log(exp(x)))
# exp: [0.01, 0.5]
# log: [e^0.01, e^0.5] \approx [1, 1.65]
start_time=$(date +%s.%n)
taskset -c 0 ./pareto_all.sh exp1x_log 0.01 0.5 1 1.65
end_time=$(date +%s.%n)
duration=$(echo "$end_time - $start_time" | bc)
echo "exp1x_log time is ${duration} s." 
echo "exp1x_log time is ${duration} s." >> hjwTime.log
## TGen
# taskset -c 0 python3 per_allversion.py exp 0.01 0.5
# taskset -c 0 python3 per_allversion.py log 1 1.65

# !!!!!!
# NMSEproblem343 := (log (/ (- 1 eps) (+ 1 eps)))
# eps: [-1, 1]
# log: (0, +infty)
# taskset -c 0 ./pareto_all_1.sh NMSEproblem343 0.5 1
# eps: [-1/2, 1/2]
# log: (1/3, 3)
start_time=$(date +%s.%n)
taskset -c 0 ./pareto_all_1.sh NMSEproblem343 0.3 3
end_time=$(date +%s.%N)
duration=$(echo "$end_time - $start_time" | bc)
echo "NMSEproblem343 time is ${duration} s." 
echo "NMSEproblem343 time is ${duration} s." >> hjwTime.log
## TGen
# taskset -c 0 python3 per_allversion.py log 0.3 3

# exp1x := (exp(x) - 1) / x
# exp: [0.01, 0.5]
start_time=$(date +%s.%n)
taskset -c 0 ./pareto_all_1.sh exp1x
end_time=$(date +%s.%n)
duration=$(echo "$end_time - $start_time" | bc)
echo "exp1x time is ${duration} s." 
echo "exp1x time is ${duration} s." >> hjwTime.log
## TGen
# taskset -c 0 python3 per_allversion.py exp 0.01 0.5

# NMSEexample37 := exp(x) - 1
# exp: [0.01, 100]
start_time=$(date +%s.%n)
taskset -c 0 ./pareto_all_1.sh NMSEexample37
end_time=$(date +%s.%N)
duration=$(echo "$end_time - $start_time" | bc)
echo "NMSEexample37 time is ${duration} s." 
echo "NMSEexample37 time is ${duration} s." >> hjwTime.log
## TGen
# taskset -c 0 python3 per_allversion.py exp 0.01 100

# NMSEexample39 := (- (/ 1 x) (/ 1 (tan x)))
# tan: [0.01, 1]
start_time=$(date +%s.%n)
taskset -c 0 ./pareto_all_1.sh NMSEexample39
end_time=$(date +%s.%N)
duration=$(echo "$end_time - $start_time" | bc)
echo "NMSEexample39 time is ${duration} s." 
echo "NMSEexample39 time is ${duration} s." >> hjwTime.log
## TGen
# taskset -c 0 python3 per_allversion.py tan 0.01 1

# NMSEproblem341 := (/ (- 1 (cos x)) (* x x))
# cos: [0.01, 100]
start_time=$(date +%s.%n)
taskset -c 0 ./pareto_all_1.sh NMSEproblem341
end_time=$(date +%s.%N)
duration=$(echo "$end_time - $start_time" | bc)
echo "NMSEproblem341 time is ${duration} s." 
echo "NMSEproblem341 time is ${duration} s." >> hjwTime.log
## TGen
# taskset -c 0 python3 per_allversion.py cos 0.01 100

# NMSEexample310 := (log((1.0 - x)) / log((1.0 + x)))
# x: -1, 1
# log: (0, 2)
# taskset -c 0 ./pareto_all_1.sh NMSEexample310 0 2
# x: -1/2, 1/2
# log: [1/2, 3/2]
start_time=$(date +%s.%n)
taskset -c 0 ./pareto_all_1.sh NMSEexample310 0.5 1.5
end_time=$(date +%s.%N)
duration=$(echo "$end_time - $start_time" | bc)
echo "NMSEexample310 time is ${duration} s." 
echo "NMSEexample310 time is ${duration} s." >> hjwTime.log
## TGen
# taskset -c 0 python3 per_allversion.py log 0.5 1.5

# NMSEexample35 := (atan((x + 1.0)) - atan(x))
# x: [0.01, 100]
# atan: [0.01, 101]
start_time=$(date +%s.%n)
taskset -c 0 ./pareto_all_1.sh NMSEexample35 0.01 101
end_time=$(date +%s.%N)
duration=$(echo "$end_time - $start_time" | bc)
echo "NMSEexample35 time is ${duration} s." 
echo "NMSEexample35 time is ${duration} s." >> hjwTime.log
## TGen
# taskset -c 0 python3 per_allversion.py atan 0.01 101

# NMSEexample38 := ((((x + 1.0) * log((x + 1.0))) - (x * log(x))) - 1.0)
# x: [0.01, 100]
# log: [0.01, 101]
start_time=$(date +%s.%n)
taskset -c 0 ./pareto_all_1.sh NMSEexample38 0.01 101
end_time=$(date +%s.%N)
duration=$(echo "$end_time - $start_time" | bc)
echo "NMSEexample38 time is ${duration} s." 
echo "NMSEexample38 time is ${duration} s." >> hjwTime.log
## TGen
# taskset -c 0 python3 per_allversion.py log 0.01 101

# NMSEproblem336 := (log((x + 1.0)) - log(x))
# x: [0.01, 100]
# log: [0.01, 101]
start_time=$(date +%s.%n)
taskset -c 0 ./pareto_all_1.sh NMSEproblem336 0.01 101
end_time=$(date +%s.%N)
duration=$(echo "$end_time - $start_time" | bc)
echo "NMSEproblem336 time is ${duration} s." 
echo "NMSEproblem336 time is ${duration} s." >> hjwTime.log
## TGen
# taskset -c 0 python3 per_allversion.py log 0.01 101

# NMSEproblem337 := ((exp(x) + (-1.0 * 2.0)) + exp((-1.0 * x)))
# x: [0.01, 100]
# exp: [-100, -0.01] & [0.01, 100]
start_time=$(date +%s.%n)
taskset -c 0 ./pareto_all_1.sh NMSEproblem337 -100 100
end_time=$(date +%s.%N)
duration=$(echo "$end_time - $start_time" | bc)
echo "NMSEproblem337 time is ${duration} s." 
echo "NMSEproblem337 time is ${duration} s." >> hjwTime.log
## TGen
# taskset -c 0 python3 per_allversion.py exp -100 100

# NMSEproblem344 := (exp(x) / (exp(x) - 1.0))
# x: [0.01, 100]
# exp: [0.01, 100]
start_time=$(date +%s.%n)
taskset -c 0 ./pareto_all_1.sh NMSEproblem344
end_time=$(date +%s.%N)
duration=$(echo "$end_time - $start_time" | bc)
echo "NMSEproblem344 time is ${duration} s." 
echo "NMSEproblem344 time is ${duration} s." >> hjwTime.log
## TGen
# taskset -c 0 python3 per_allversion.py exp 0.01 100

# NMSEsection311 := sqrt(((exp((2.0 * x)) - 1.0) / (exp(x) - 1.0)))
# x: [0.01, 100]
# exp: [0.01, 200]
start_time=$(date +%s.%n)
taskset -c 0 ./pareto_all_1.sh NMSEsection311 0.01 200
end_time=$(date +%s.%N)
duration=$(echo "$end_time - $start_time" | bc)
echo "NMSEsection311 time is ${duration} s." 
echo "NMSEsection311 time is ${duration} s." >> hjwTime.log
## TGen
# taskset -c 0 python3 per_allversion.py exp 0 200

# theta := (atan((x2 / x1)) * 57.29577951307855)
# x: [1, 100]
# y: [1, 100]
# atan: [0.01, 100]
start_time=$(date +%s.%n)
taskset -c 0 ./pareto_all_1.sh theta 0.01 100
end_time=$(date +%s.%n)
duration=$(echo "$end_time - $start_time" | bc)
echo "theta time is ${duration} s." 
echo "theta time is ${duration} s." >> hjwTime.log
## TGen
# taskset -c 0 python3 per_allversion.py atan 0.01 100

# ComplexSinCos := (((1.0 / 2.0) * sin(r)) * (exp((-1.0 * i)) - exp(i)))
# x: [0, 1]
# y: [0, 1]
# sin: [0, 1]
# exp: [-1, 1]
start_time=$(date +%s.%n)
taskset -c 0 ./pareto_all.sh ComplexSinCos -1 1 0 1
end_time=$(date +%s.%N)
duration=$(echo "$end_time - $start_time" | bc)
echo "ComplexSinCos time is ${duration} s." 
echo "ComplexSinCos time is ${duration} s." >> hjwTime.log
## TGen
# taskset -c 0 python3 per_allversion.py exp -1 1
# taskset -c 0 python3 per_allversion.py sin 0 1

# NMSEexample33 := (sin((x + eps)) - sin(x))
# eps: [0.01, 1]
# x: [0, 100]
# sin: [0, 101]
start_time=$(date +%s.%n)
taskset -c 0 ./pareto_all_1.sh NMSEexample33 0 101
end_time=$(date +%s.%N)
duration=$(echo "$end_time - $start_time" | bc)
echo "NMSEexample33 time is ${duration} s." 
echo "NMSEexample33 time is ${duration} s." >> hjwTime.log
## TGen
# taskset -c 0 python3 per_allversion.py sin 0 101

# NMSEproblem332 := (tan((x + eps)) - tan(x))
# eps: [0.01, 1]
# x: [0, 100]
# tan: [0, 101]
start_time=$(date +%s.%n)
taskset -c 0 ./pareto_all_1.sh NMSEproblem332 0 101
end_time=$(date +%s.%N)
duration=$(echo "$end_time - $start_time" | bc)
echo "NMSEproblem332 time is ${duration} s." 
echo "NMSEproblem332 time is ${duration} s." >> hjwTime.log
## TGen
# taskset -c 0 python3 per_allversion.py tan 0 101

# NMSEproblem335 := (cos((x + eps)) - cos(x))
# eps: [0.01, 1]
# x: [0, 100]
# cos: [0, 101]
start_time=$(date +%s.%n)
taskset -c 0 ./pareto_all_1.sh NMSEproblem335 0 101
end_time=$(date +%s.%N)
duration=$(echo "$end_time - $start_time" | bc)
echo "NMSEproblem335 time is ${duration} s." 
echo "NMSEproblem335 time is ${duration} s." >> hjwTime.log
## TGen
# taskset -c 0 python3 per_allversion.py cos 0 101

# NMSEsection35 := (exp((a * x)) - 1.0)
# a: [0, 1]
# x: [0, 1]
# exp: [0, 1]
start_time=$(date +%s.%n)
taskset -c 0 ./pareto_all_1.sh NMSEsection35
end_time=$(date +%s.%N)
duration=$(echo "$end_time - $start_time" | bc)
echo "NMSEsection35 time is ${duration} s." 
echo "NMSEsection35 time is ${duration} s." >> hjwTime.log
## TGen
# taskset -c 0 python3 per_allversion.py exp 0 1

# polarToCarthesianX := (radius * cos((0.017453292519944444 * theta)))
# radius: [1, 10]
# theta: [0, 360]
# cos: [0, 2pi] \approx [0, 6.2832]
start_time=$(date +%s.%n)
taskset -c 0 ./pareto_all_1.sh polarToCarthesianX 0 6.2832
end_time=$(date +%s.%N)
duration=$(echo "$end_time - $start_time" | bc)
echo "polarToCarthesianX time is ${duration} s." 
echo "polarToCarthesianX time is ${duration} s." >> hjwTime.log
## TGen
# taskset -c 0 python3 per_allversion.py cos 0 6.2832

# polarToCarthesianY := (radius * sin((0.017453292519944444 * theta)))
# radius: [1, 10]
# theta: [0, 360]
# sin: [0, 2pi] \approx [0, 6.2832]
start_time=$(date +%s.%n)
taskset -c 0 ./pareto_all_1.sh polarToCarthesianY 0 6.2832
end_time=$(date +%s.%N)
duration=$(echo "$end_time - $start_time" | bc)
echo "polarToCarthesianY time is ${duration} s." 
echo "polarToCarthesianY time is ${duration} s." >> hjwTime.log
## TGen
# taskset -c 0 python3 per_allversion.py sin 0 6.2832

# i6 := sin((x * y))
# x: [0.1, 10]
# y: [-5, 5]
# sin: [-50, 50]
start_time=$(date +%s.%n)
taskset -c 0 ./pareto_all_1.sh i6 -50 50
end_time=$(date +%s.%n)
duration=$(echo "$end_time - $start_time" | bc)
echo "i6 time is ${duration} s." 
echo "i6 time is ${duration} s." >> hjwTime.log
## TGen
# taskset -c 0 python3 per_allversion.py sin -50 50