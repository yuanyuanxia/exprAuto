# 用来计算一个仅包含n个变量的多项式的不同计算序列的数量。这是计算任意表达式的计算序列的数量的基础。
from scipy.special import comb


def computeNum(n):
    if (n == 1):
        return 1
    else:
        sum = 0
        for i in range(1, int(n / 2 + 1)):
            current = comb(n, i) * computeNum(i) * computeNum(n - i)
            if(i * 2 == n): # 此时有重复计算
                current = current / 2
            sum = sum + current
        return sum


print("hello, I want to compute the number of different computing queue")
formula = "The formula is: f(n)=sigma(i = 1 : k){C(i, n) * f(i) * f(n - i)}, k = floor(n/2)"
print("formula is %s" % (formula))
# n = 10
# sum = computeNum(n)
# print("when n = %d, sum is %d" % (n, sum))

for i in range(1, 15):
    sum = computeNum(i)
    print("when i = %2d, sum is %d" % (i, sum))
