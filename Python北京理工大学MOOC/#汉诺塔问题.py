#汉诺塔问题.py
count = 0
def hanoi(n, src, dst, mid):
    global count
    if n == 1 :
        print("{}:{}->{}".fotmat(1,src,dst))
        count +=1
    else :
        hanoi(n-1, src, mid, dst)
        print("{}:{}->{}".format(1, src, dst))
        count += 1
        hanoi(n-1,mid,dst,src)
        print(count)
