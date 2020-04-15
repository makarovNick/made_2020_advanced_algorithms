class Node(object):
    def __init__(self, localValue=0, globalValue=0, localLazy=0, globalLazy=0):
        self.localValue = localValue
        self.globalValue = globalValue
        self.localLazy = localLazy
        self.globalLazy = globalLazy


class SegmentTree2D(object):
    def __init__(self, n, m):

        self.n = n
        self.m = m
        self.tree = []

        for _ in range(4 * n):
            li = []
            for __ in range(4 * m):
                li.append(Node())

            self.tree.append(li)

    def update(self, qxLo, qxHi, qyLo, qyHi, v):
        self.updateByX((1, 1), 1, self.n, qxLo, qxHi, qyLo, qyHi, v)

    def query(self, qxLo, qxHi, qyLo, qyHi):
        return self.queryByX((1, 1), 1, self.n, qxLo, qxHi, qyLo, qyHi)

    def updateByX(self, nodeID, xLo, xHi, qxLo, qxHi, qyLo, qyHi, v):
        if (qxLo <= xLo and xHi <= qxHi):
            self.updateByY((nodeID[0], 1), xLo, xHi, 1, self.m, qxLo, qxHi, qyLo, qyHi, v, True)
            return
        elif (xLo > qxHi or xHi < qxLo):
            return
        else:
            xMid = (xLo + xHi) // 2
            left = (nodeID[0] * 2, 1)
            right = (left[0] + 1, 1)

            self.updateByX(left, xLo, xMid, qxLo, qxHi, qyLo, qyHi, v)
            self.updateByX(right, xMid + 1, xHi, qxLo, qxHi, qyLo, qyHi, v)

            txLo = max(qxLo, xLo)
            txHi = min(qxHi, xHi)
            scaled_v = v * ((1 * (txHi - txLo + 1)) / (xHi - xLo + 1))

            self.updateByY((nodeID[0], 1), xLo, xHi, 1, self.m, txLo, txHi, qyLo, qyHi, scaled_v, False)

    def updateByY(self, nodeID, xLo, xHi, yLo, yHi, qxLo, qxHi, qyLo, qyHi, v, covered):
        if (qyLo <= yLo and yHi <= qyHi):
            if (covered):
                self.tree[nodeID[0]][nodeID[1]].globalLazy += v
                self.tree[nodeID[0]][nodeID[1]].globalValue += v * (xHi - xLo + 1) * (yHi - yLo + 1)
            else:
                self.tree[nodeID[0]][nodeID[1]].localLazy += v
                self.tree[nodeID[0]][nodeID[1]].localValue += v * (xHi - xLo + 1) * (yHi - yLo + 1)
            
            return
        elif (yHi < qyLo or qyHi < yLo):
            return
        else:

            yMid = (yLo + yHi) // 2
            left = (nodeID[0], nodeID[1] * 2)
            right = (left[0], left[1] + 1)

            self.updateByY(left, xLo, xHi, yLo, yMid, qxLo, qxHi, qyLo, qyHi, v, covered)
            self.updateByY(right, xLo, xHi, yMid + 1, yHi, qxLo, qxHi, qyLo, qyHi, v, covered)

            self.tree[nodeID[0]][nodeID[1]].localValue = self.tree[left[0]][left[1]].localValue\
                                                       + self.tree[right[0]][right[1]].localValue\
                                                       + (self.tree[nodeID[0]][nodeID[1]].localLazy) * (xHi - xLo + 1) * (yHi - yLo + 1)
            self.tree[nodeID[0]][nodeID[1]].globalValue = self.tree[left[0]][left[1]].globalValue \
                                                        + self.tree[right[0]][right[1]].globalValue\
                                                        + (self.tree[nodeID[0]][nodeID[1]].globalLazy) * (xHi - xLo + 1) * (yHi - yLo + 1)

    def queryByX(self, nodeID, xLo, xHi, qxLo, qxHi, qyLo, qyHi):
        if (qxLo <= xLo and xHi <= qxHi):
            return self.queryByY((nodeID[0], 1), xLo, xHi, 1, self.m, qxLo, qxHi, qyLo, qyHi, 0)
        elif (xLo > qxHi or xHi < qxLo):
            return 0
        else:
            xMid = (xLo + xHi) // 2
            left = (nodeID[0] * 2, nodeID[1])
            right = (left[0] + 1, left[1])
            txLo = max(qxLo, xLo)
            txHi = min(qxHi, xHi)
            return self.queryByY(nodeID, xLo, xHi, 1, self.m, txLo, txHi, qyLo, qyHi, 0) + self.queryByX(left, xLo, xMid, qxLo, qxHi, qyLo, qyHi) + self.queryByX(right, xMid + 1, xHi, qxLo, qxHi, qyLo, qyHi)

    def queryByY(self, nodeID, xLo, xHi, yLo, yHi, qxLo, qxHi, qyLo, qyHi, lazy):
        if (qyLo <= yLo and yHi <= qyHi):
            if (qxLo <= xLo and xHi <= qxHi):
                return self.tree[nodeID[0]][nodeID[1]].localValue + self.tree[nodeID[0]][nodeID[1]].globalValue + lazy * (xHi - xLo + 1) * (yHi - yLo + 1)
            elif (xLo > qxHi or xHi < qxLo):
                return 0
            else:
                scaled_value = self.tree[nodeID[0]][nodeID[1]].globalValue * 1 * (qxHi - qxLo + 1) / (xHi - xLo + 1)
                return scaled_value + lazy * (qxHi - qxLo + 1) * (yHi - yLo + 1)
        elif (yLo > qyHi or yHi < qyLo):
            return 0
        else:
            yMid = (yLo + yHi) // 2
            left = (nodeID[0], nodeID[1] * 2)
            right = (left[0], left[1] + 1)

            if (qxLo <= xLo and xHi <= qxHi):
                return self.queryByY(left, xLo, xHi, yLo, yMid, qxLo, qxHi, qyLo, qyHi, lazy + self.tree[nodeID[0]][nodeID[1]].globalLazy + self.tree[nodeID[0]][nodeID[1]].localLazy) + self.queryByY(right, xLo, xHi, yMid + 1, yHi, qxLo, qxHi, qyLo, qyHi, lazy + self.tree[nodeID[0]][nodeID[1]].globalLazy + self.tree[nodeID[0]][nodeID[1]].localLazy)
            elif (xLo > qxHi or xHi < qxLo):
                return 0
            else:
                return self.queryByY(left, xLo, xHi, yLo, yMid, qxLo, qxHi, qyLo, qyHi, lazy + self.tree[nodeID[0]][nodeID[1]].globalLazy) + self.queryByY(right, xLo, xHi, yMid + 1, yHi, qxLo, qxHi, qyLo, qyHi, lazy + self.tree[nodeID[0]][nodeID[1]].globalLazy)

X, Y = map(int, input().split(' '))
segTree = SegmentTree2D(X + 2, Y + 2)

N = int(input())
for i in range(N):
    vals = list(map(int, input().strip().split(' ')))
    if vals[0] == 1:
        segTree.update(vals[1], vals[3], vals[2], vals[4], vals[5])
    else:
        print(int(segTree.query(vals[1], vals[3], vals[2], vals[4])))

for i in range(X + 2):
    for j in range(Y + 2):
        print(int(segTree.query(i, i, j, j)), end=' ')
    print('')