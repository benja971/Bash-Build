import random

y = 2
pn = 1/365
pm = 1/(100*365)
dy = 0

print(pn, pm)

while y < 1000:
    for i in range(y):
        if random.random() < pn: dy+=1
        if random.random() < pm: dy-=1
    
    y += dy
    print(y)