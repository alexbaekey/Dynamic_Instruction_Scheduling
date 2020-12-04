import numpy as np
import matplotlib.pyplot as plt
plt.ion()


# val_trace_gcc.txt graph

# queue size ( independent variable )
S = [8,16,32,64,128,256]

# IPC ( dependent variable )
IPC_N1   = [0.99325, 0.99830, 0.99830, 0.99830, 0.99830, 0.99830]
IPC_N2   = [1.87547, 1.98610, 1.99322, 1.99322, 1.99322, 1.99322]
IPC_N4   = [2.67094, 3.54108, 3.90320, 3.96668, 3.96668, 3.96668]
IPC_N8   = [2.82008, 4.53721, 6.48088, 7.51880, 7.83085, 7.83085]

#within 5% of terminal:
#N1 - clearly 8
#N2 - 
#>>> 0.05*1.99322
#0.099661
#>>> 1.99322-0.099661
#1.893559
#so 16

#N4 - 
#>>> 0.05*3.96668
#0.198334
#>>> 3.96668-0.198334
#3.768346
#so 32

#N8
#>>> 0.05*7.83085
#0.3915425
#>>> 7.83085-0.3915425
#7.4393075
#so 64

plt.plot(S, IPC_N1, label='N = 1', marker='o')
plt.plot(S, IPC_N2, label='N = 2', marker='o')
plt.plot(S, IPC_N4, label='N = 4', marker='o')
plt.plot(S, IPC_N8, label='N = 8', marker='o')
plt.title("gcc_trace")
plt.xlabel('Queue Size (S)')
plt.ylabel('IPC')
plt.legend()
plt.ylim(0,9)
plt.grid()
plt.savefig('figs/gcc_trace.jpg')
plt.clf()



# val_trace_perl.txt graph

# queue size ( independent variable )

S = [8,16,32,64,128,256]

# IPC ( dependent variable )
IPC_N1   = [0.97666, 0.99681, 0.99711, 0.99711, 0.99711, 0.99711]
IPC_N2   = [1.68293, 1.88679, 1.97863, 1.98491, 1.98491, 1.98491]
IPC_N4   = [2.18007, 2.91290, 3.68460, 3.90625, 3.94011, 3.94011]
IPC_N8   = [2.27635, 3.36814, 5.17598, 6.94927, 7.61035, 7.74593]


#Witin 5% of terminal
#N1 - clearly 8
#N2-
#0.05*1.98491 = 0.0992455
#1.98491 - 0.0992455 = 1.8856644999999999
#so 16 is within 5%
#N4
#>>> 0.05*3.94011
#0.1970055
#>>> 3.94011-0.1970055
#3.7431045
#so 64 is within 5%
#N8
#>>> 0.05*7.74593
#0.38729650000000004
#>>> 7.74593-0.3872965
#7.358633500000001
#so 128

plt.plot(S, IPC_N1, label='N = 1', marker='o')
plt.plot(S, IPC_N2, label='N = 2', marker='o')
plt.plot(S, IPC_N4, label='N = 4', marker='o')
plt.plot(S, IPC_N8, label='N = 8', marker='o')
plt.title("perl_trace")
plt.xlabel('Queue Size (S)')
plt.ylabel('IPC')
plt.legend()
plt.ylim(0,9)
plt.grid()
plt.savefig('figs/perl_trace.jpg')
plt.clf()
