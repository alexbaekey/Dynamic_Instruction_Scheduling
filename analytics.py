import numpy as np
import matplotlib.pyplot as plt
plt.ion()


# val_trace_gcc.txt graph

# queue size ( independent variable )
S = [8,16,32,64,128,256]

# IPC ( dependent variable )
IPC_N1   = [0.99960, 0.99960, 0.99960, 0.99960, 0.99960, 0.99960]
IPC_N2   = [1.95771, 1.99760, 1.99760, 1.99760, 1.99760, 1.99760]
IPC_N4   = [2.73000, 3.75940, 3.98406, 3.98406, 3.98406, 3.98406]
IPC_N8   = [2.82008, 4.57247, 6.72495, 7.85546, 7.88644, 7.88644]


n=1 - 8
n=2
>>> 0.05*1.99760
0.09988000000000001
>>> 1.99760-0.09988
1.89772
 - 8
n=4
>>> 0.05*3.98406
0.19920300000000002
>>> 3.98406-0.199203
3.784857
 - 32

>>> 0.05 * 7.88644
0.39432200000000006
>>> 7.88644-0.394322
7.4921180000000005
 - 64


plt.plot(S, IPC_N1, label='N = 1', marker='o')
plt.plot(S, IPC_N2, label='N = 2', marker='o')
plt.plot(S, IPC_N4, label='N = 4', marker='o')
plt.plot(S, IPC_N8, label='N = 8', marker='o')
plt.title("gcc_trace")
plt.xlabel('Queue Size (S)')
plt.ylabel('IPC')
plt.legend(loc='upper left')
plt.ylim(0,9)
plt.grid()
plt.savefig('figs/gcc_trace.jpg')
plt.clf()



# val_trace_perl.txt graph

# queue size ( independent variable )

S = [8,16,32,64,128,256]

# IPC ( dependent variable )
IPC_N1   = [0.99910, 0.99920, 0.99920, 0.99920, 0.99920, 0.99920]
IPC_N2   = [1.81127, 1.97550, 1.99481, 1.99561, 1.99561, 1.99561]
IPC_N4   = [2.20022, 3.06091, 3.83289, 3.95413, 3.97931, 3.97931]
IPC_N8   = [2.27635, 3.39443, 5.28821, 7.23589, 7.75194, 7.87402]

n=1 - 8 
n=2 - 16
>>> 0.05 * 3.97931
0.19896550000000002
>>> 3.97931-0.1989655
3.7803445
 - 32
>>> 0.05 * 7.87402
0.393701
>>> 7.87402-0.393701
7.480319
 - 128



plt.plot(S, IPC_N1, label='N = 1', marker='o')
plt.plot(S, IPC_N2, label='N = 2', marker='o')
plt.plot(S, IPC_N4, label='N = 4', marker='o')
plt.plot(S, IPC_N8, label='N = 8', marker='o')
plt.title("perl_trace")
plt.xlabel('Queue Size (S)')
plt.ylabel('IPC')
plt.legend(loc='upper left')
plt.ylim(0,9)
plt.grid()
plt.savefig('figs/perl_trace.jpg')
plt.clf()
