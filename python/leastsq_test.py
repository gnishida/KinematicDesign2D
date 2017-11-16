import numpy as np
import scipy.optimize
import math

################################################################
# objective function to minimize
# x = (w_x, w_y, u, v)
def func(x, T_list):
	w = np.transpose(np.array([x[0], x[1], 1]))
	G = np.transpose(np.array([x[2], x[3]]))

	T1 = T_list[0]
	error = 0

	for T in T_list:
		error += abs(np.dot(np.dot(T, w) - G, np.dot(T, w) - G) - np.dot(np.dot(T1, w) - G, np.dot(T1, w) - G))
	
	return error

################################################################
# setup transformation matrices
T_list = []

# T = |1 0 200  |
#     |0 1 100+t|
# (0 <= t <= 100)
#for t in range(1, 101):
for t in range(1, 11):
	#T = np.array([[1, 0, 200],[0, 1, 100+t]])
	T = np.array([[1, 0, 200],[0, 1, 190+t]])
	T_list.append(T)

# T = |cos(t) -sin(t) 180+10*cos(t)|
#     |sin(t)  cos(t) 200+10*sin(t)|
# (0 <= t <= PI/2)
for i in range(0, 101):
	t = math.pi / 200 * i
	T = np.array([[math.cos(t), -math.sin(t), 180 + 10 * math.cos(t)],
					[math.sin(t), math.cos(t), 200 + 10 * math.sin(t)]])
	T_list.append(T)
	
# T = |0 -1 190-t|
#     |1  0 210  |
# (0 <= t <= 10)
for t in range(1, 11):
	T = np.array([[0, -1, 190 - t],[1, 0, 210]])
	T_list.append(T)


################################################################
# solve the system in a least square manner
x0 = np.array([0, 0, 0, 0])
x = scipy.optimize.minimize(func, x0, args=(T_list)).x
print x
print func(x, T_list)



