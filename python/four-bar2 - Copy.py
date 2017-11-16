import numpy as np
import scipy.optimize
import math
import matplotlib.pyplot as plt


	

################################################################
# objective function to minimize
# x = (w_x, w_y, u, v)
def func(x, T_list):
	w = np.transpose(np.array([x[0], x[1], 1]))
	G = np.transpose(np.array([x[2], x[3]]))

	T1 = T_list[0]
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


u_sample = np.linspace(-20, 30, 51)
#u_sample = np.linspace(-2, 3, 6)
v_sample = np.linspace(-30, 70, 101)
#v_sample = np.linspace(-2, 3, 6)
wx_sample = np.linspace(-20, 30, 51)
#u_sample = np.linspace(-2, 3, 6)
wy_sample = np.linspace(-30, 70, 101)
#v_sample = np.linspace(-2, 3, 6)
#print u_sample
#print v_sample

################################################################
# solve the system in a least square manner

data = np.zeros((len(v_sample), len(u_sample)), dtype='f')

for u in u_sample:
	for v in v_sample:
		min_f = 99999999999999999999

		for wx in wx_sample:
			for wy in wy_sample:
				x = np.zeros(4)
				x[0] = wx * 0.1
				x[1] = wy * 0.1
				x[2] = u * 0.1
				x[3] = v * 0.1
				f = func(x, T_list)
				min_f = min(min_f, f)
		data[v][u] = min_f
#print data

plt.imshow(data, interpolation='nearest', cmap="hot")
plt.xticks(np.arange(0, len(u_sample), len(u_sample) * 10 / (u_sample[-1] - u_sample[0])), np.arange(-2, 4, 1))
plt.yticks(np.arange(len(v_sample) - 1, -1, len(v_sample) * 10 / (v_sample[0] - v_sample[-1])), np.arange(-3, 7, 1))
plt.show()
