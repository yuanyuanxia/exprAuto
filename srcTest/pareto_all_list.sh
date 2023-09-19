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
taskset -c 0 ./pareto_all_1.sh exp1x 0.5 1
# eps: [-1/2, 1/2]
# log: (1/3, 3)
taskset -c 0 ./pareto_all_1.sh exp1x 0.333333 3

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