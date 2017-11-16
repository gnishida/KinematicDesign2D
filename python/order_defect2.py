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
# order check
def orderCheck(T_list, wx, wy, u, v):
	#print "-------------------------------------"
	#print "order check:"
	w = np.transpose(np.array([wx, wy, 1]))
	G = np.transpose(np.array([u, v]))

	cnt = 0
	prev = 0
	ccw = 1
	for T in T_list:
		dir = np.dot(T, w) - G
		theta = math.atan2(dir[1], dir[0])
		#print theta / math.pi * 180
		if cnt == 1:
			if theta > prev:
				if theta - prev > math.pi:
					ccw = -1
			else:
				prev -= math.pi * 2
				if theta - prev > math.pi:
					ccw = -1
		elif cnt > 1:
			if ccw == 1:
				if theta > prev:
					if theta - prev > math.pi: return False
				else:
					prev -= math.pi * 2
					if theta - prev > math.pi: return False
			else:
				if theta > prev:
					prev += math.pi * 2
					if prev - theta > math.pi: return False
				else:
					if prev - theta > math.pi: return False
		
		cnt += 1
		prev = theta
		
	#print "Good!!!!!!!!!!!!!!!!!"
	return True

################################################################
# setup transformation matrices
T_list = []
T1 = np.array([[1, 0, 0], [0, 1, 0]])
T_list.append(T1)
T2 = np.array([[1, 0, 1], [0, 1, -0.5]])
T_list.append(T2)
T3 = np.array([[0.707107, -0.707107, 3], [0.707107, 0.707107, 0.085786]])
T_list.append(T3)
T4 = np.array([[0, -1, 3], [1, 0, 1]])
T_list.append(T4)


u_sample = np.linspace(-2, 3, 100)
v_sample = np.linspace(-3, 7, 200)

################################################################
# solve the system in a least square manner

W = []
G = []
W2 = []
G2 = []
for u in u_sample:
	v_list = []
	
	#for i in range(10):
	for i in range(50):
		x0 = np.random.rand(3) * 20 - 10
		x = scipy.optimize.minimize(func, x0, args=(T_list, u)).x
		f = func(x, T_list, u)
		if f > 0.1: continue
		
		if x[2] in v_list: continue
		
		v_list.append(x[2])
		
		if orderCheck(T_list, x[0], x[1], u, x[2]):
			W.append([x[0], x[1]])
			G.append([u, x[2]])
		else:
			W2.append([x[0], x[1]])
			G2.append([u, x[2]])

'''
for v in v_sample:
	u_list = []
	
	#for i in range(10):
	for i in range(1):
		x0 = np.random.rand(3) * 20 - 10
		x = scipy.optimize.minimize(func2, x0, args=(T_list, v)).x
		f = func2(x, T_list, v)
		if f > 0.01: continue
		
		if x[2] in u_list: continue
		
		u_list.append(x[2])
		
		if orderCheck(T_list, x[0], x[1], x[2], v):
			W.append([x[0], x[1]])
			G.append([x[2], v])
		else:
			W2.append([x[0], x[1]])
			G2.append([x[2], v])
'''

W = np.array(W)
W2 = np.array(W2)
G = np.array(G)
G2 = np.array(G2)
#print W.shape
#print G.shape
#plt.plot(W[:,0], W[:,1], 'r.')
print G.shape
print G2.shape
plt.plot(G[:,0], G[:,1], 'b.')
plt.plot(G2[:,0], G2[:,1], 'r.')
plt.show()

