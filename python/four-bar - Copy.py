import numpy as np
import scipy.optimize
import math
import matplotlib.pyplot as plt


	

################################################################
# objective function to minimize
# x = (w_x, w_y, u, v)
def func(x, T_list, u):
	w = np.transpose(np.array([x[0], x[1], 1]))
	G = np.transpose(np.array([u, x[2]]))

	T1 = T_list[0]
	error = 0

	for T in T_list:
		error += abs(np.dot(np.dot(T, w) - G, np.dot(T, w) - G) - np.dot(np.dot(T1, w) - G, np.dot(T1, w) - G))
	
	return error
	
def func2(x, T_list, v):
	w = np.transpose(np.array([x[0], x[1], 1]))
	G = np.transpose(np.array([x[2], v]))

	T1 = T_list[0]
	error = 0

	for T in T_list:
		error += abs(np.dot(np.dot(T, w) - G, np.dot(T, w) - G) - np.dot(np.dot(T1, w) - G, np.dot(T1, w) - G))
	
	return error

################################################################
# setup transformation matrices
T_list = []
T1 = np.array([[1, 0, 1], [0, 1, 1]])
T_list.append(T1)
T2 = np.array([[1, 0, 2], [0, 1, 0.5]])
T_list.append(T2)
T3 = np.array([[1/math.sqrt(2.0), -1/math.sqrt(2.0), 3], [1/math.sqrt(2), 1/math.sqrt(2), 1.5]])
T_list.append(T3)
T4 = np.array([[0, -1, 2], [1, 0, 2]])
T_list.append(T4)


u_sample = np.linspace(-2, 3, 1000)
v_sample = np.linspace(-3, 7, 2000)

################################################################
# solve the system in a least square manner

W = []
G = []
for u in u_sample:
	v_list = []
	
	#for i in range(10):
	for i in range(100):
		x0 = np.random.rand(3) * 20 - 10
		x = scipy.optimize.minimize(func, x0, args=(T_list, u)).x
		f = func(x, T_list, u)
		if f > 0.01: continue
		
		if x[2] in v_list: continue
		
		v_list.append(x[2])
		W.append([x[0] + 1, x[1] + 1])
		G.append([u, x[2]])

for v in v_sample:
	u_list = []
	
	#for i in range(10):
	for i in range(100):
		x0 = np.random.rand(3) * 20 - 10
		x = scipy.optimize.minimize(func2, x0, args=(T_list, v)).x
		f = func2(x, T_list, v)
		if f > 0.01: continue
		
		if x[2] in u_list: continue
		
		u_list.append(x[2])
		W.append([x[0] + 1, x[1] + 1])
		G.append([x[2], v])

W = np.array(W)
G = np.array(G)
print W.shape
print G.shape
plt.plot(W[:,0], W[:,1], 'r.')
plt.plot(G[:,0], G[:,1], 'b.')
plt.show()

