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

resolution = 50

u_num = (u_max - u_min) * resolution + 1
v_num = (v_max - v_min) * resolution + 1
wx_num = (wx_max - wx_min) * resolution + 1
wy_num = (wy_max - wy_min) * resolution + 1
data = np.zeros((wy_num + 1, wx_num + 1), dtype='f')

for wx_index in range(wx_num + 1):
	wx = wx_min + float(wx_max - wx_min) / wx_num * wx_index
	for wy_index in range(wy_num + 1):
		wy = wy_min + float(wy_max - wy_min) / wy_num * wy_index
		
		min_f = 99999999999999999999
		for u_index in range(u_num + 1):
			u = u_min + float(u_max - u_min) / u_num * u_index
			for v_index in range(v_num + 1):
				v = u_min + float(v_max - v_min) / v_num * v_index

				min_f = min(min_f, func([wx, wy, u, v], T_list))

		data[wy_index][wx_index] = min_f
		
data = np.flipud(data)
#print data

plt.imshow(data, interpolation='nearest', cmap="hot")
plt.xticks(np.arange(0, wx_num + 1, resolution), np.arange(wx_min, wx_max + 1, 1))
plt.yticks(np.arange(wy_num, -1, -resolution), np.arange(wy_min, wy_max + 1, 1))
plt.show()
