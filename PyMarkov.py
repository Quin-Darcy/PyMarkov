import numpy as np
from PIL import Image
import cv2
import math
import random

def nTerm(count):
    x = 0
    temp = [0]*10
    nums = [0]*10

    weights = tuple(count)

    for j in range(10):
        nums[j] = j

    x = random.choices(nums, weights, k = 1)
    for j in x:
        ret = j

    return ret

def mMult(count, tran):
    arr = [0]*10

    for i in range(0, 10):
        for j in range(0, 10):
            arr[i] += count[j] * tran[j][i]

    return arr

def setTran(arr):
    k = 0
    temp = [0]*10
    tran = [[0]*10 for _ in range(10)]
    
    for x in range(0, 399):
        tran[arr[x]][arr[x+1]] += 1 
        temp[arr[x]] += 1

    for x in range(0, 10):
        for y in range(0, 10):
            if ((tran[x][y] > 0) and (temp[x] != 0)):
                tran[x][y] /= temp[x]

    return tran

def getCount(quad):
    arr = [0]*10
    for z in range(10):
        for x in range(400):
            if (quad[x] == z):
                arr[z] += 1

    for i in range(10):
        arr[i] = arr[i]/400

    return arr

def getQ(seq):
    p = 0
    r = 0

    q = np.full((5,5,20,20), 0)
    for y in range(5):
        for x in range(20):
            for i in range(5):
                for j in range(20):
                    q[i][y][p%20][r%20] = seq[p*10+r]
                    p += 1
            p = 0
            r += 1
                
    return q
            
def getSeq():
    with open('Gen0.txt', 'r') as f:
        arr = []
        for line in f:
            line = line.split()
            if line:
                line = [int(i) for i in line]
                arr.append(line)

    seq = [y for x in arr for y in x]  

    return seq

def flat(seq):
    arr = [y for x in seq for y in x]

    return arr
    
def main():
    t = 0
    arr = [0]*400
    newM = [[0]*10 for _ in range(10)]
    temp = [[0]*20 for _ in range(500)]
    newArr = [0]*10000
    q = np.full((5,5,20,20), 0)
    p = np.full((5,5,20,20), 0)
    seq = getSeq()
    q = getQ(seq)

    for x in range(5):
        for y in range(5):
            for i in range(20):
                for j in range(20):
                    arr[t] = q[x][y][i][j]
                    t += 1
            t = 0
            count = getCount(arr)
            tran = setTran(arr)
            for i in range(20):
                for j in range(20):
                    p[x][y][i][j] = nTerm(count)
                    count = mMult(count, tran)

    with open('text.txt', 'w') as f:
        for i in range(5):
            for j in range(5):
                for x in range(20):
                    for y in range(20):
                        f.write(str(p[i][j][x][y]))
                        f.write(' ')
                    f.write('\n')
                f.write('\n\n')

    t = 0
    for x in range(5):
        for i in range(20):
            for y in range(5):
                temp[t] = p[x][y][i]
                t += 1

    newArr = flat(temp)

    with open('quads.txt', 'w') as f:
        for i in range(10000):
            if (i % 100 == 0):
                f.write('\n')
                f.write(str(newArr[i]))
                f.write(' ')
            else:
                f.write(str(newArr[i]))
                f.write(' ')

    


main()
