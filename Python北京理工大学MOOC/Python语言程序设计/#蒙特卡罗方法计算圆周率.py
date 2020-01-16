#CalpiV2.py
#蒙特卡罗方法计算圆周率
from random import random
from time import perf_counter
DARTS = 1000*10000
hits = 0
start = perf_counter()
for i in range(1,DARTS+1):
    x,y=random(),random()
    dist = pow(x**2+y**2,0.5)
    if dist <=1.0:
        hits = hits+1
pi = 4*(hits/DARTS)
print("圆周率的值为：{:.50f}".format(pi))
print("运行时间为：{:.10f}s".format(perf_counter()-start))
