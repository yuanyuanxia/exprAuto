# NMSEproblem345 := (/ (- x (sin x)) (- x (tan x)))
# sin: [0.01, 100]
# tan: [0.01, 100]
taskset -c 0 ./pareto_all.sh NMSEproblem345

# NMSEexample34: (/ (- 1 (cos x)) (sin x))
# sin: [0.01, 100]
# cos: [0.01, 100]
taskset -c 0 ./pareto_all.sh NMSEexample34

# logexp := log (+ 1 (exp x))
# exp: [-8, 8]
# log: [1 + exp(-8), 1 + exp(8)] = [1.000335, 2,981.957987] \approx [1, 2982]
taskset -c 0 ./pareto_all.sh logexp -8 8 1 2982

# !!!!!!
# NMSEproblem343 := (log (/ (- 1 eps) (+ 1 eps)))
# eps: [-1, 1]
# log: (0, +infty)
taskset -c 0 ./pareto_all_1.sh NMSEproblem343 0.5 1
# eps: [-1/2, 1/2]
# log: (1/3, 3)
taskset -c 0 ./pareto_all_1.sh NMSEproblem343 0.3 3

# exp1x := (exp(x) - 1) / x
# exp: [0.01, 0.5]
taskset -c 0 ./pareto_all_1.sh exp1x

# NMSEexample37 := exp(x) - 1
# exp: [0.01, 100]
taskset -c 0 ./pareto_all_1.sh NMSEexample37

# NMSEexample39 := (- (/ 1 x) (/ 1 (tan x)))
# tan: [0.01, 1]
taskset -c 0 ./pareto_all_1.sh NMSEexample39

# NMSEproblem341 := (/ (- 1 (cos x)) (* x x))
# cos: [0.01, 100]
taskset -c 0 ./pareto_all_1.sh NMSEproblem341

# NMSEexample310 := (log((1.0 - x)) / log((1.0 + x)))
# x: -1, 1
# log: (0, 2)
taskset -c 0 ./pareto_all_1.sh NMSEexample310 0 2
# x: -1/2, 1/2
# log: [1/2, 3/2]
taskset -c 0 ./pareto_all_1.sh NMSEexample310 0.5 1.5

# NMSEexample35 := (atan((x + 1.0)) - atan(x))
# x: [0.01, 100]
# atan: [0.01, 101]
taskset -c 0 ./pareto_all_1.sh NMSEexample35 0.01 101

# NMSEexample38 := ((((x + 1.0) * log((x + 1.0))) - (x * log(x))) - 1.0)
# x: [0.01, 100]
# log: [0.01, 101]
taskset -c 0 ./pareto_all_1.sh NMSEexample38 0.01 101

# NMSEproblem336 := (log((x + 1.0)) - log(x))
# x: [0.01, 100]
# log: [0.01, 101]
taskset -c 0 ./pareto_all_1.sh NMSEproblem336 0.01 101

# NMSEproblem337 := ((exp(x) + (-1.0 * 2.0)) + exp((-1.0 * x)))
# x: [0.01, 100]
# log: [-100, -0.01] & [0.01, 100]
taskset -c 0 ./pareto_all_1.sh NMSEproblem337 -100 100

# NMSEproblem344 := (exp(x) / (exp(x) - 1.0))
# x: [0.01, 100]
# exp: [0.01, 100]
taskset -c 0 ./pareto_all_1.sh NMSEproblem344

# NMSEsection311 := sqrt(((exp((2.0 * x)) - 1.0) / (exp(x) - 1.0)))
# x: [0.01, 100]
# exp: [0.01, 200]
taskset -c 0 ./pareto_all_1.sh NMSEsection311 0.01 200