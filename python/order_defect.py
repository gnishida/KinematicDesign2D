import numpy as np
import scipy.optimize
import math
import matplotlib.pyplot as plt
from numpy.linalg import inv

	

################################################################
# objective function to minimize
# x = (w_x, w_y, u, v)
def func(x, T_list):
	w = np.transpose(np.array([x[0], x[1], 1]))
	G = np.transpose(np.array([x[2], x[3], 1]))

	error = 0

	for i in range(len(T_list)):
		for j in range(i + 1, len(T_list)):
			T1 = T_list[i]
			T2 = T_list[j]
			error += abs(np.dot(np.dot(T2, w) - G, np.dot(T2, w) - G) - np.dot(np.dot(T1, w) - G, np.dot(T1, w) - G))
	
	return error

################################################################
# order check
def orderCheck(T_list, wx, wy, u, v):
	#print "-------------------------------------"
	#print "order check:"
	inv_w = np.dot(inv(T_list[0]), np.array([wx, wy, 1]))
	
	#w = np.transpose(np.array([wx, wy, 1]))
	G = np.transpose(np.array([u, v, 1]))

	cnt = 0
	prev = 0
	ccw = 1
	for T in T_list:
		dir = np.dot(T, inv_w) - G
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
T1 = np.array([[1, 0, 0], [0, 1, 0], [0, 0, 1]])
T_list.append(T1)
T2 = np.array([[1, 0, 1], [0, 1, -0.5], [0, 0, 1]])
T_list.append(T2)
T3 = np.array([[0.707107, -0.707107, 3], [0.707107, 0.707107, 0.085786], [0, 0, 1]])
T_list.append(T3)
T4 = np.array([[0, -1, 3], [1, 0, 1], [0, 0, 1]])
T_list.append(T4)


u_min = -2
u_max = 3
v_min = -3
v_max = 6
wx_min = -2
wx_max = 3
wy_min = -3
wy_max = 6

################################################################
# solve the system in a least square manner

W = []
G = []
W2 = []
G2 = []

#resolution = 50
resolution = 1

u_num = (u_max - u_min) * resolution + 1
v_num = (v_max - v_min) * resolution + 1
wx_num = (wx_max - wx_min) * resolution + 1
wy_num = (wy_max - wy_min) * resolution + 1

file = open('solution_curve.txt', 'w')

for wx_index in range(wx_num + 1):
	wx = wx_min + float(wx_max - wx_min) / wx_num * wx_index
	for wy_index in range(wy_num + 1):
		wy = wy_min + float(wy_max - wy_min) / wy_num * wy_index
		
		for u_index in range(u_num + 1):
			u = u_min + float(u_max - u_min) / u_num * u_index
			for v_index in range(v_num + 1):
				v = u_min + float(v_max - v_min) / v_num * v_index
				
				x = scipy.optimize.minimize(func, [wx, wy, u, v], args=T_list).x
				f = func([x[0], x[1], x[2], x[3]], T_list)
				if f > 0.05: continue
				
				if orderCheck(T_list, x[0], x[1], x[2], x[3]):
					W.append([x[0], x[1]])
					G.append([x[2], x[3]])
				else:
					W2.append([x[0], x[1]])
					G2.append([x[2], x[3]])

file.close()

W = np.array(W)
G = np.array(G)
W2 = np.array(W2)
G2 = np.array(G2)

print W.shape
print G.shape
print W2.shape
print G2.shape


if W.shape[0] > 0:
	plt.plot(W[:,0], W[:,1], '.', color='#ff0000')
if G.shape[0] > 0:
	plt.plot(G[:,0], G[:,1], '.', color='#0000ff')
if W2.shape[0] > 0:
	plt.scatter(W2[:,0], W2[:,1], color='#ff8888')
if G2.shape[0] > 0:
	plt.plot(G2[:,0], G2[:,1], '.', color='#8888ff')
plt.show()

