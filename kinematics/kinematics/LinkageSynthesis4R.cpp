﻿#include "LinkageSynthesis4R.h"
#include "KinematicUtils.h"
#include "Kinematics.h"
#include "PinJoint.h"
#include "SliderHinge.h"
#include "BoundingBox.h"
#include "LeastSquareSolver.h"
#include <opencv2/opencv.hpp>

namespace kinematics {

	LinkageSynthesis4R::LinkageSynthesis4R(const std::vector<Object25D>& fixed_bodies, const std::vector<std::pair<double, double>>& sigmas, bool rotatable_crank, bool avoid_branch_defect, double min_transmission_angle, double min_link_length, const std::vector<double>& weights) {
		this->fixed_bodies = fixed_bodies;
		this->sigmas = sigmas;
		this->rotatable_crank = rotatable_crank;
		this->avoid_branch_defect = avoid_branch_defect;
		this->min_transmission_angle = min_transmission_angle;
		this->min_link_length = min_link_length;
		this->weights = weights;
	}

	/**
	* Calculate solutions of 4R linkage given three poses.
	*
	* @param poses			three poses
	* @param solutions1	the output solutions for the world coordinates of the driving crank at the first pose, each of which contains a pair of the center point and the circle point
	* @param solutions2	the output solutions for the world coordinates of the follower at the first pose, each of which contains a pair of the center point and the circle point
	*/
	void LinkageSynthesis4R::calculateSolution(const std::vector<glm::dmat3x3>& poses, const std::vector<glm::dvec2>& linkage_region_pts, const std::vector<glm::dvec2>& linkage_avoidance_pts, int num_samples, const Object25D& moving_body, std::vector<Solution>& solutions) {
		solutions.clear();

		srand(0);

		int cnt = 0;

		// calculate the bounding boxe of the valid regions
		BBox bbox = boundingBox(linkage_region_pts);
			
		for (int iter = 0; iter < num_samples && cnt < num_samples; iter++) {
			printf("\rsampling %d/%d", cnt, iter + 1);

			// perturbe the poses a little
			double position_error = 0.0;
			double orientation_error = 0.0;
			std::vector<glm::dmat3x3> perturbed_poses = perturbPoses(poses, sigmas, position_error, orientation_error);

			// sample joints within the linkage region
			std::vector<glm::dvec2> points(4);
			for (int i = 0; i < points.size(); i++) {
				while (true) {
					points[i] = glm::dvec2(genRand(bbox.minPt.x, bbox.maxPt.x), genRand(bbox.minPt.y, bbox.maxPt.y));
					if (withinPolygon(linkage_region_pts, points[i])) break;
				}
			}

			if (!optimizeCandidate(perturbed_poses, linkage_region_pts, bbox, points)) continue;

			// check hard constraints
			if (!checkHardConstraints(points, perturbed_poses, linkage_region_pts, linkage_avoidance_pts, moving_body)) continue;

			solutions.push_back(Solution(0, points, position_error, orientation_error, perturbed_poses));
			cnt++;
		}
		printf("\n");
	}

	/**
	* Optimize the linkage parameters based on the rigidity constraints.
	* If it fails to optimize, return false.
	*/
	bool LinkageSynthesis4R::optimizeCandidate(const std::vector<glm::dmat3x3>& poses, const std::vector<glm::dvec2>& linkage_region_pts, const BBox& bbox, std::vector<glm::dvec2>& points) {
		if (poses.size() == 2) {
			if (!optimizeLinkForTwoPoses(poses, linkage_region_pts, points[0], points[2])) return false;
			if (!optimizeLinkForTwoPoses(poses, linkage_region_pts, points[1], points[3])) return false;
		}
		else if (poses.size() == 3) {
			if (!optimizeLinkForThreePoses(poses, linkage_region_pts, points[0], points[2])) return false;
			if (!optimizeLinkForThreePoses(poses, linkage_region_pts, points[1], points[3])) return false;
		}
		else {
			if (!optimizeLink(poses, linkage_region_pts, bbox, points[0], points[2])) return false;
			if (!optimizeLink(poses, linkage_region_pts, bbox, points[1], points[3])) return false;
		}

		return true;
	}

	bool LinkageSynthesis4R::optimizeLink(const std::vector<glm::dmat3x3>& poses, const std::vector<glm::dvec2>& linkage_region_pts, const BBox& bbox, glm::dvec2& A0, glm::dvec2& A1) {
		// setup the initial parameters for optimization
		column_vector starting_point(4);
		starting_point(0, 0) = A0.x;
		starting_point(1, 0) = A0.y;
		starting_point(2, 0) = A1.x;
		starting_point(3, 0) = A1.y;

		try {
			find_min(dlib::bfgs_search_strategy(), dlib::objective_delta_stop_strategy(1e-7), SolverForLink(poses), SolverDerivForLink(poses), starting_point, -1);

			A0.x = starting_point(0, 0);
			A0.y = starting_point(1, 0);
			A1.x = starting_point(2, 0);
			A1.y = starting_point(3, 0);

			// if the joints are outside the valid region, discard it.
			if (!withinPolygon(linkage_region_pts, A0)) return false;
			if (!withinPolygon(linkage_region_pts, A1)) return false;
		}
		catch (std::exception& e) {
			return false;
		}

		return true;
	}

	bool LinkageSynthesis4R::optimizeLinkForThreePoses(const std::vector<glm::dmat3x3>& poses, const std::vector<glm::dvec2>& linkage_region_pts, glm::dvec2& A0, glm::dvec2& A1) {
		// calculate the local coordinate of A1
		glm::dvec2 a = glm::dvec2(glm::inverse(poses[0]) * glm::dvec3(A1, 1));

		glm::dvec2 A2(poses[1] * glm::dvec3(a, 1));
		glm::dvec2 A3(poses[2] * glm::dvec3(a, 1));

		try {
			A0 = circleCenterFromThreePoints(A1, A2, A3);

			// if the center point is outside the valid region, discard it.
			if (!withinPolygon(linkage_region_pts, A0)) return false;

			// if the moving point is outside the valid region, discard it.
			//if (!withinPolygon(linkage_region_pts, A1)) return false;
		}
		catch (char* ex) {
			return false;
		}

		return true;
	}

	bool LinkageSynthesis4R::optimizeLinkForTwoPoses(const std::vector<glm::dmat3x3>& poses, const std::vector<glm::dvec2>& linkage_region_pts, glm::dvec2& A0, glm::dvec2& A1) {
		// calculate the local coordinate of A1
		glm::dvec2 a = glm::dvec2(glm::inverse(poses[0]) * glm::dvec3(A1, 1));

		glm::dvec2 A2(poses[1] * glm::dvec3(a, 1));

		glm::dvec2 M = (A1 + A2) * 0.5;
		glm::dvec2 v = A1 - A2;
		v /= glm::length(v);
		glm::dvec2 h(-v.y, v.x);

		A0 = M + h * glm::dot(A0 - M, h);

		// if the center point is outside the valid region, discard it.
		if (!withinPolygon(linkage_region_pts, A0)) return false;

		return true;
	}

	Solution LinkageSynthesis4R::findBestSolution(const std::vector<glm::dmat3x3>& poses, std::vector<Solution>& solutions, const std::vector<glm::dvec2>& linkage_region_pts, const cv::Mat& dist_map, const BBox& dist_map_bbox, const std::vector<glm::dvec2>& linkage_avoidance_pts, const Object25D& moving_body, int num_particles, int num_iterations, bool record_file) {
		// select the best solution based on the objective function
		if (solutions.size() > 0) {
			particleFilter(solutions, linkage_region_pts, dist_map, dist_map_bbox, linkage_avoidance_pts, moving_body, num_particles, num_iterations, record_file);
			return solutions[0];
		}
		else {
			return Solution(0, { { 0, 0 }, { 0, 2 }, { 2, 0 }, { 2, 2 } }, 0, 0, poses);
		}
	}

	double LinkageSynthesis4R::calculateCost(Solution& solution, const Object25D& moving_body, const cv::Mat& dist_map, const BBox& dist_map_bbox) {
		double dist = 0;
		for (int i = 0; i < solution.points.size(); i++) {
			dist += dist_map.at<double>(solution.points[i].y - dist_map_bbox.minPt.y, solution.points[i].x - dist_map_bbox.minPt.x);
		}
		double tortuosity = tortuosityOfTrajectory(solution.poses, solution.points, moving_body);
		std::vector<glm::dvec2> connected_pts;
		Kinematics kin = constructKinematics(solution.points, moving_body, true, fixed_bodies, connected_pts);
		//double size = glm::length(solution.points[0] - solution.points[2]) + glm::length(solution.points[1] - solution.points[3]) + glm::length(solution.points[0] - connected_pts[0]) + glm::length(solution.points[1] - connected_pts[1]) + glm::length(solution.points[2] - connected_pts[2]) + glm::length(solution.points[3] - connected_pts[3]);
		double size = glm::length(solution.points[0] - connected_pts[0]) + glm::length(solution.points[1] - connected_pts[1]) + glm::length(solution.points[2] - connected_pts[2]) + glm::length(solution.points[3] - connected_pts[3]);

		return solution.position_error * weights[0] + solution.orientation_error * weights[1] + dist * weights[2] + tortuosity * weights[3] + size * weights[4];
	}

	/**
	 * Construct a linkage.
	 */
	Kinematics LinkageSynthesis4R::constructKinematics(const std::vector<glm::dvec2>& points, const Object25D& moving_body, bool connect_joints, const std::vector<Object25D>& fixed_bodies, std::vector<glm::dvec2>& connected_pts) {
		Kinematics kin;
		kin.diagram.addJoint(boost::shared_ptr<kinematics::PinJoint>(new kinematics::PinJoint(0, true, points[0])));
		kin.diagram.addJoint(boost::shared_ptr<kinematics::PinJoint>(new kinematics::PinJoint(1, true, points[1])));
		kin.diagram.addJoint(boost::shared_ptr<kinematics::PinJoint>(new kinematics::PinJoint(2, false, points[2])));
		kin.diagram.addJoint(boost::shared_ptr<kinematics::PinJoint>(new kinematics::PinJoint(3, false, points[3])));
		kin.diagram.addLink(true, kin.diagram.joints[0], kin.diagram.joints[2], true);
		kin.diagram.addLink(false, kin.diagram.joints[1], kin.diagram.joints[3], true);
		kin.diagram.addLink(false, kin.diagram.joints[2], kin.diagram.joints[3], false);

		std::vector<Object25D> copied_fixed_bodies = fixed_bodies;

		// update the geometry
		updateMovingBodies(kin, moving_body);

		if (connect_joints) {
			kin.diagram.connectJointsToBodies(copied_fixed_bodies, connected_pts);
		}

		// add the fixed rigid bodies
		for (int i = 0; i < copied_fixed_bodies.size(); i++) {
			kin.diagram.addBody(kin.diagram.joints[0], kin.diagram.joints[1], copied_fixed_bodies[i]);
		}

		return kin;
	}

	/**
	* update bodies.
	*/
	void LinkageSynthesis4R::updateMovingBodies(Kinematics& kin, const Object25D& moving_body) {
		kin.diagram.bodies.clear();
		kin.diagram.addBody(kin.diagram.joints[2], kin.diagram.joints[3], moving_body);
	}

	bool LinkageSynthesis4R::checkHardConstraints(std::vector<glm::dvec2>& points, const std::vector<glm::dmat3x3>& poses, const std::vector<glm::dvec2>& linkage_region_pts, const std::vector<glm::dvec2>& linkage_avoidance_pts, const Object25D& moving_body) {
		if (glm::length(points[0] - points[1]) < min_link_length) return false;
		if (glm::length(points[2] - points[3]) < min_link_length) return false;

		if (checkFolding(points)) return false;
		if (rotatable_crank && checkRotatableCrankDefect(points)) return false;
		if (avoid_branch_defect && checkBranchDefect(poses, points)) return false;
		if (checkCircuitDefect(poses, points)) return false;
		if (checkOrderDefect(poses, points)) return false;

		// collision check for the main body
		// moving_body[0] means the main body without the joint connectors
		if (checkCollision(poses, points, fixed_bodies, moving_body[0], linkage_avoidance_pts)) return false;

		return true;
	}

	/**
	* Return the Grashof type.
	*
	* 0 -- Grashof (Drag-link)
	* 1 -- Grashof (Crank-rocker)
	* 2 -- Grashof (Rocker-crank)
	* 3 -- Grashof (Double-rocker)
	* 4 -- Non-Grashof (0-0 Rocker)
	* 5 -- Non-Grashof (pi-pi Rocker)
	* 6 -- Non-Grashof (pi-0 Rocker)
	* 7 -- Non-Grashof (0-pi Rocker)
	*/
	int LinkageSynthesis4R::getType(const std::vector<glm::dvec2>& points) {
		double g = glm::length(points[0] - points[1]);
		double a = glm::length(points[0] - points[2]);
		double b = glm::length(points[1] - points[3]);
		double h = glm::length(points[2] - points[3]);

		double T1 = g + h - a - b;
		double T2 = b + g - a - h;
		double T3 = b + h - a - g;

		if (T1 < 0 && T2 < 0 && T3 >= 0) {
			return 0;
		}
		else if (T1 >= 0 && T2 >= 0 && T3 >= 0) {
			return 1;
		}
		else if (T1 >= 0 && T2 < 0 && T3 < 0) {
			return 2;
		}
		else if (T1 < 0 && T2 >= 0 && T3 < 0) {
			return 3;
		}
		else if (T1 < 0 && T2 < 0 && T3 < 0) {
			return 4;
		}
		else if (T1 < 0 && T2 >= 0 && T3 >= 0) {
			return 5;
		}
		else if (T1 >= 0 && T2 < 0 && T3 >= 0) {
			return 6;
		}
		else if (T1 >= 0 && T2 >= 0 && T3 < 0) {
			return 7;
		}
		else {
			return -1;
		}
	}

	bool LinkageSynthesis4R::checkFolding(const std::vector<glm::dvec2>& points) {
		double g = glm::length(points[0] - points[1]);
		double a = glm::length(points[0] - points[2]);
		double b = glm::length(points[1] - points[3]);
		double h = glm::length(points[2] - points[3]);

		double T1 = g + h - a - b;
		double T2 = b + g - a - h;
		double T3 = b + h - a - g;

		if (abs(T1) < 0.05 || abs(T2) < 0.05 || abs(T3) < 0.05) return true;
		else return false;
	}

	std::pair<double, double> LinkageSynthesis4R::checkRange(const std::vector<glm::dvec2>& points) {
		double g = glm::length(points[0] - points[1]);
		double a = glm::length(points[0] - points[2]);
		double b = glm::length(points[1] - points[3]);
		double h = glm::length(points[2] - points[3]);

		double T1 = g + h - a - b;
		double T2 = b + g - a - h;
		double T3 = b + h - a - g;

		double theta_min = 0;
		double theta_max = M_PI * 2;

		int linkage_type = getType(points);
		if (linkage_type == 2) {
			if (crossProduct(points[0] - points[2], points[1] - points[0]) >= 0) {
				theta_min = acos((a * a + g * g - (h - b) * (h - b)) / 2 / a / g);
				theta_max = acos((a * a + g * g - (h + b) * (h + b)) / 2 / a / g);
			}
			else {
				theta_min = -acos((a * a + g * g - (h + b) * (h + b)) / 2 / a / g);
				theta_max = -acos((a * a + g * g - (h - b) * (h - b)) / 2 / a / g);
			}
		}
		else if (linkage_type == 3) {
			if (crossProduct(points[0] - points[2], points[1] - points[0]) >= 0) {
				theta_min = acos((a * a + g * g - (b - h) * (b - h)) / 2 / a / g);
				theta_max = acos((a * a + g * g - (b + h) * (b + h)) / 2 / a / g);
			}
			else {
				theta_min = -acos((a * a + g * g - (b + h) * (b + h)) / 2 / a / g);
				theta_max = -acos((a * a + g * g - (b - h) * (b - h)) / 2 / a / g);
			}
		}
		else if (linkage_type == 4 || linkage_type == 7) {
			theta_max = acos((a * a + g * g - (b + h) * (b + h)) / 2 / a / g);
			theta_min = -theta_max;
		}
		else if (linkage_type == 5) {
			theta_min = acos((a * a + g * g - (b - h) * (b - h)) / 2 / a / g);
			theta_max = M_PI * 2 - theta_min;
		}
		else if (linkage_type == 6) {
			theta_min = acos((a * a + g * g - (h - b) * (h - b)) / 2 / a / g);
			theta_max = M_PI * 2 - theta_min;
		}

		return { theta_min, theta_max };
	}

	/**
	* Check if the linkage has a rotatable crank defect.
	* If the crank is not fully rotatable, true is returned.
	*/
	bool LinkageSynthesis4R::checkRotatableCrankDefect(const std::vector<glm::dvec2>& points) {
		int linkage_type = getType(points);

		if (linkage_type == 0 || linkage_type == 1) {
			return false;
		}
		else {
			return true;
		}
	}

	/**
	* Check if the linkage has order defect.
	* If there is an order defect, true is returned.
	* Otherwise, false is returned.
	*/
	bool LinkageSynthesis4R::checkOrderDefect(const std::vector<glm::dmat3x3>& poses, const std::vector<glm::dvec2>& points) {
		glm::dvec2 inv_W = glm::dvec2(glm::inverse(poses[0]) * glm::dvec3(points[2], 1));

		int linkage_type = getType(points);

		double total_cw = 0;
		double total_ccw = 0;
		double prev = atan2(points[2].y - points[0].y, points[2].x - points[0].x);
		for (int i = 1; i < poses.size(); i++) {
			// calculate the coordinates of the circle point of the driving crank in the world coordinate system
			glm::dvec2 X = glm::dvec2(poses[i] * glm::dvec3(inv_W, 1));
			//std::cout << X.x << "," << X.y << std::endl;

			// calculate the direction from the ground pivot (center point) of the driving crank to the circle point
			glm::dvec2 dir = X - points[0];

			// calculate its angle
			double theta = atan2(dir.y, dir.x);

			if (theta >= prev) {
				total_cw += M_PI * 2 - theta + prev;
				total_ccw += theta - prev;

				if (linkage_type == 4 || linkage_type == 7) {
					total_cw = M_PI * 999;
					total_ccw += theta - prev;
				}
				else if (linkage_type == 5 || linkage_type == 6) {
					if ((theta > 0 && prev > 0) || (theta < 0 && prev < 0)) {
						total_cw = M_PI * 999;
					}
					if ((theta > 0 && prev < 0) || (theta < 0 && prev > 0)) {
						total_ccw = M_PI * 999;
					}
				}
			}
			else {
				total_cw += prev - theta;
				total_ccw += M_PI * 2 - prev + theta;

				if (linkage_type == 4 || linkage_type == 7) {
					total_cw += prev - theta;
					total_ccw = M_PI * 999;
				}
				else if (linkage_type == 5 || linkage_type == 6) {
					if ((theta > 0 && prev < 0) || (theta < 0 && prev > 0)) {
						total_cw = M_PI * 999;
					}
					if ((theta > 0 && prev > 0) || (theta < 0 && prev < 0)) {
						total_ccw = M_PI * 999;
					}
				}
			}

			prev = theta;
		}

		if (total_cw > M_PI * 2 && total_ccw > M_PI * 2) return true;
		else return false;
	}

	/**
	* Check if all the poses are in the same branch.
	* Drag-link and crank-rocker always do not have a branch defect.
	* For other types of linkage, the change in the sign of the angle between the coupler and the follower indicates the change of the branch.
	* If there is an branch defect, true is returned. Otherwise, false is returned.
	*
	* @param poses	pose matrices
	* @param p0		the world coordinates of the fixed point of the driving crank at the first pose
	* @param p1		the world coordinates of the fixed point of the follower at the first pose
	* @param p2		the world coordinates of the moving point of the driving crank at the first pose
	* @param p3		the world coordinates of the moving point of the follower at the first pose
	* @return		true if the branch defect is detected, false otherwise
	*/
	bool LinkageSynthesis4R::checkBranchDefect(const std::vector<glm::dmat3x3>& poses, const std::vector<glm::dvec2>& points) {
		int type = getType(points);

		// drag-link and crank-rocker always do not have a branch defect
		if (type == 0 || type == 1) return false;

		int orig_sign = 1;

		// calculate the local coordinates of the circle points
		glm::dvec2 W1 = glm::dvec2(glm::inverse(poses[0]) * glm::dvec3(points[2], 1));
		glm::dvec2 W2 = glm::dvec2(glm::inverse(poses[0]) * glm::dvec3(points[3], 1));

		for (int i = 0; i < poses.size(); i++) {
			// calculate the coordinates of the circle point of the driving/driven cranks in the world coordinate system
			glm::dvec2 X1 = glm::dvec2(poses[i] * glm::dvec3(W1, 1));
			glm::dvec2 X2 = glm::dvec2(poses[i] * glm::dvec3(W2, 1));

			// calculate its sign
			if (i == 0) {
				orig_sign = crossProduct(X2 - points[1], X1 - X2) >= 0 ? 1 : -1;
			}
			else {
				int sign = crossProduct(X2 - points[1], X1 - X2) >= 0 ? 1 : -1;
				if (sign != orig_sign) {
					return true;
				}
			}
		}

		return false;
	}

	bool LinkageSynthesis4R::checkCircuitDefect(const std::vector<glm::dmat3x3>& poses, const std::vector<glm::dvec2>& points) {
		int type = getType(points);

		// Non-grashof type does not have a circuit defect
		if (type >= 4) return false;

		int orig_sign0 = 1;
		int orig_sign1 = 1;
		int orig_sign2 = 1;
		int orig_sign3 = 1;

		// calculate the local coordinates of the circle points
		glm::dvec2 W1 = glm::dvec2(glm::inverse(poses[0]) * glm::dvec3(points[2], 1));
		glm::dvec2 W2 = glm::dvec2(glm::inverse(poses[0]) * glm::dvec3(points[3], 1));

		for (int i = 0; i < poses.size(); i++) {
			// calculate the coordinates of the circle point of the driving/driven cranks in the world coordinate system
			glm::dvec2 X1 = glm::dvec2(poses[i] * glm::dvec3(W1, 1));
			glm::dvec2 X2 = glm::dvec2(poses[i] * glm::dvec3(W2, 1));

			// calculate its sign
			if (i == 0) {
				orig_sign0 = crossProduct(points[0] - points[1], X1 - points[0]) >= 0 ? 1 : -1;
				orig_sign1 = crossProduct(points[1] - X2, points[0] - points[1]) >= 0 ? 1 : -1;
				orig_sign2 = crossProduct(X1 - points[0], X2 - X1) >= 0 ? 1 : -1;
				orig_sign3 = crossProduct(X2 - X1, points[1] - X2) >= 0 ? 1 : -1;
			}
			else {
				int sign0 = crossProduct(points[0] - points[1], X1 - points[0]) >= 0 ? 1 : -1;
				int sign1 = crossProduct(points[1] - X2, points[0] - points[1]) >= 0 ? 1 : -1;
				int sign2 = crossProduct(X1 - points[0], X2 - X1) >= 0 ? 1 : -1;
				int sign3 = crossProduct(X2 - X1, points[1] - X2) >= 0 ? 1 : -1;

				if (type == 0) {
					if (sign2 != orig_sign2 || sign3 != orig_sign3) {
						return true;
					}
				}
				else if (type == 1) {
					if (sign1 != orig_sign1 || sign3 != orig_sign3) {
						return true;
					}
				}
				else if (type == 2) {
					if (sign0 != orig_sign0 || sign2 != orig_sign2) {
						return true;
					}
				}
				else if (type == 3) {
					if (sign0 != orig_sign0 || sign1 != orig_sign1) {
						return true;
					}
				}
			}
		}

		return false;
	}

	/**
	* Check collision. If there are any collisions, return true.
	* Only the main body is checked for collision.
	*
	* @param poses					N poses
	* @param points				joint coordinates
	* @param fixed_bodies			list of fixed bodies
	* @param moving_body			moving body
	* @param linkage_avoidance_pts	region to avoid for the linkage
	* @return						true if collision occurs
	*/
	bool LinkageSynthesis4R::checkCollision(const std::vector<glm::dmat3x3>& poses, const std::vector<glm::dvec2>& points, const std::vector<Object25D>& fixed_bodies, const Object25D& moving_body, const std::vector<glm::dvec2>& linkage_avoidance_pts) {
		std::vector<glm::dvec2> connector_pts;
		kinematics::Kinematics kinematics = constructKinematics(points, moving_body, false, fixed_bodies, connector_pts);
		kinematics.diagram.initialize();

		// calculate the rotational angle of the driving crank for 1st, 2nd, and last poses
		// i.e., angles[0] = first pose, angles[1] = second pose, angles[2] = last pose
		std::vector<double> angles(3);
		glm::dvec2 w(glm::inverse(poses[0]) * glm::dvec3(points[2], 1));
		for (int i = 0; i < 2; i++) {
			glm::dvec2 W = glm::dvec2(poses[i] * glm::dvec3(w, 1));
			angles[i] = atan2(W.y - points[0].y, W.x - points[0].x);
		}
		{
			glm::dvec2 W = glm::dvec2(poses.back() * glm::dvec3(w, 1));
			angles[2] = atan2(W.y - points[0].y, W.x - points[0].x);
		}

		// order the angles based on their signs
		int type = 0;
		if (angles[0] < 0 && angles[1] < 0 && angles[2] >= 0 && angles[0] >= angles[1]) {
			type = 1;
			angles[2] -= M_PI * 2;
		}
		else if (angles[0] < 0 && angles[1] >= 0 && angles[2] < 0 && angles[0] < angles[2]) {
			type = 2;
			angles[1] -= M_PI * 2;
			angles[2] -= M_PI * 2;
		}
		else if (angles[0] < 0 && angles[1] >= 0 && angles[2] < 0 && angles[0] >= angles[2]) {
			type = 3;
			angles[2] += M_PI * 2;
		}
		else if (angles[0] < 0 && angles[1] >= 0 && angles[2] >= 0 && (poses.size() >= 3 && angles[1] >= angles[2] || poses.size() == 2 && angles[1] - angles[0] > M_PI)) {
			type = 4;
			angles[1] -= M_PI * 2;
			angles[2] -= M_PI * 2;
		}
		else if (angles[0] >= 0 && angles[1] < 0 && angles[2] < 0 && (poses.size() >= 3 && angles[1] < angles[2] || poses.size() == 2 && angles[0] - angles[1] > M_PI)) {
			type = 5;
			angles[1] += M_PI * 2;
			angles[2] += M_PI * 2;
		}
		else if (angles[0] >= 0 && angles[1] < 0 && angles[2] >= 0 && angles[0] < angles[2]) {
			type = 6;
			angles[2] -= M_PI * 2;
		}
		else if (angles[0] >= 0 && angles[1] < 0 && angles[2] >= 0 && angles[0] >= angles[2]) {
			type = 7;
			angles[1] += M_PI * 2;
			angles[2] += M_PI * 2;
		}
		else if (angles[0] >= 0 && angles[1] >= 0 && angles[2] < 0 && angles[0] < angles[1]) {
			type = 8;
			angles[2] += M_PI * 2;
		}

		if (angles[2] < angles[0]) {
			kinematics.invertSpeed();
		}

		// initialize the visited flag
		std::vector<bool> visited(angles.size(), false);
		visited[0] = true;
		int unvisited = 2;

		// run forward until collision is deteted or all the poses are reached
		while (true) {
			try {
				kinematics.stepForward(true, false);
			}
			catch (char* ex) {
				// if only some of the poses are reached before collision, the collision is detected.
				kinematics.clear();
				return true;
			}

			// check if the joints are within the linkage avoidance region
			for (int i = 0; i < kinematics.diagram.joints.size(); i++) {
				if (withinPolygon(linkage_avoidance_pts, kinematics.diagram.joints[i]->pos)) return true;
			}

			// check the transmission angle
			if (avoid_branch_defect) {
				glm::dvec2 a = kinematics.diagram.joints[2]->pos - kinematics.diagram.joints[3]->pos;
				glm::dvec2 b = kinematics.diagram.joints[3]->pos - kinematics.diagram.joints[1]->pos;
				double t_angle = std::acos(glm::dot(a, b) / glm::length(a) / glm::length(b));
				if (std::abs(t_angle) < min_transmission_angle) return true;
			}

			// calculate the angle of the driving crank
			double angle = atan2(kinematics.diagram.joints[2]->pos.y - points[0].y, kinematics.diagram.joints[2]->pos.x - points[0].x);

			// convert the sign of the angle
			if (type == 1 && angle > 0) {
				angle -= M_PI * 2;
			}
			else if (type == 2 && angle > angles[0]) {
				angle -= M_PI * 2;
			}
			else if (type == 3 && angle < angles[0]) {
				angle += M_PI * 2;
			}
			else if (type == 4 && angle > 0) {
				angle -= M_PI * 2;
			}
			else if (type == 5 && angle < 0) {
				angle += M_PI * 2;
			}
			else if (type == 6 && angle > angles[0]) {
				angle -= M_PI * 2;
			}
			else if (type == 7 && angle < angles[0]) {
				angle += M_PI * 2;
			}
			else if (type == 8 && angle < 0) {
				angle += M_PI * 2;
			}

			// check if the poses are reached
			for (int i = 0; i < angles.size(); i++) {
				if (visited[i]) continue;

				if (angles[2] >= angles[0]) {
					if (angle >= angles[i]) {
						visited[i] = true;
						unvisited--;
					}
				}
				else {
					if (angle <= angles[i]) {
						visited[i] = true;
						unvisited--;
					}
				}
			}

			// if all the poses are reached without collision, no collision is detected.
			if (unvisited == 0) {
				kinematics.clear();
				return false;
			}
		}

		kinematics.clear();
		return false;
	}

	double LinkageSynthesis4R::tortuosityOfTrajectory(const std::vector<glm::dmat3x3>& poses, const std::vector<glm::dvec2>& points, const Object25D& moving_body) {
		// calculate the local coordinates of the body points
		glm::dmat3x3 inv_pose0 = glm::inverse(poses[0]);
		std::vector<glm::dvec2> body_pts_local(moving_body.polygons[0].points.size());
		for (int i = 0; i < moving_body.polygons[0].points.size(); i++) {
			body_pts_local[i] = glm::dvec2(inv_pose0 * glm::dvec3(moving_body.polygons[0].points[i], 1));
		}

		// calculate the length of the motion using straight lines between poses
		double length_of_straight = 0.0;
		std::vector<glm::dvec2> prev_body_pts = moving_body.polygons[0].points;
		for (int i = 1; i < poses.size(); i++) {
			std::vector<glm::dvec2> next_body_pts(moving_body.polygons[0].points.size());
			for (int k = 0; k < moving_body.polygons[0].points.size(); k++) {
				next_body_pts[k] = glm::dvec2(poses[i] * glm::dvec3(body_pts_local[k], 1));
				length_of_straight += glm::length(next_body_pts[k] - prev_body_pts[k]);
			}
			prev_body_pts = next_body_pts;
		}
		
		// create a kinematics
		std::vector<glm::dvec2> connector_pts;
		kinematics::Kinematics kinematics = constructKinematics(points, moving_body, false, {}, connector_pts);
		kinematics.diagram.initialize();

		// initialize the trajectory of the moving body
		prev_body_pts = moving_body.polygons[0].points;
		double length_of_trajectory = 0.0;

		// calculate the rotational angle of the driving crank for 1st, 2nd, and last poses
		// i.e., angles[0] = first pose, angles[1] = second pose, angles[2] = last pose
		std::vector<double> angles(3);
		glm::dvec2 w(glm::inverse(poses[0]) * glm::dvec3(points[2], 1));
		for (int i = 0; i < 2; i++) {
			glm::dvec2 W = glm::dvec2(poses[i] * glm::dvec3(w, 1));
			angles[i] = atan2(W.y - points[0].y, W.x - points[0].x);
		}
		{
			glm::dvec2 W = glm::dvec2(poses.back() * glm::dvec3(w, 1));
			angles[2] = atan2(W.y - points[0].y, W.x - points[0].x);
		}

		// order the angles based on their signs
		int type = 0;
		if (angles[0] < 0 && angles[1] < 0 && angles[2] >= 0 && angles[0] >= angles[1]) {
			type = 1;
			angles[2] -= M_PI * 2;
		}
		else if (angles[0] < 0 && angles[1] >= 0 && angles[2] < 0 && angles[0] < angles[2]) {
			type = 2;
			angles[1] -= M_PI * 2;
			angles[2] -= M_PI * 2;
		}
		else if (angles[0] < 0 && angles[1] >= 0 && angles[2] < 0 && angles[0] >= angles[2]) {
			type = 3;
			angles[2] += M_PI * 2;
		}
		else if (angles[0] < 0 && angles[1] >= 0 && angles[2] >= 0 && (poses.size() >= 3 && angles[1] >= angles[2] || poses.size() == 2 && angles[1] - angles[0] > M_PI)) {
			type = 4;
			angles[1] -= M_PI * 2;
			angles[2] -= M_PI * 2;
		}
		else if (angles[0] >= 0 && angles[1] < 0 && angles[2] < 0 && (poses.size() >= 3 && angles[1] < angles[2] || poses.size() == 2 && angles[0] - angles[1] > M_PI)) {
			type = 5;
			angles[1] += M_PI * 2;
			angles[2] += M_PI * 2;
		}
		else if (angles[0] >= 0 && angles[1] < 0 && angles[2] >= 0 && angles[0] < angles[2]) {
			type = 6;
			angles[2] -= M_PI * 2;
		}
		else if (angles[0] >= 0 && angles[1] < 0 && angles[2] >= 0 && angles[0] >= angles[2]) {
			type = 7;
			angles[1] += M_PI * 2;
			angles[2] += M_PI * 2;
		}
		else if (angles[0] >= 0 && angles[1] >= 0 && angles[2] < 0 && angles[0] < angles[1]) {
			type = 8;
			angles[2] += M_PI * 2;
		}

		if (angles[2] < angles[0]) {
			kinematics.invertSpeed();
		}

		// initialize the visited flag
		std::vector<bool> visited(angles.size(), false);
		visited[0] = true;
		int unvisited = 2;

		// run forward until collision is deteted or all the poses are reached
		while (true) {
			try {
				kinematics.stepForward(false, false);	// no collision check
			}
			catch (char* ex) {
				// if only some of the poses are reached before collision, the collision is detected.
				kinematics.clear();
				return length_of_trajectory / length_of_straight;
			}

			// calculate the angle of the driving crank
			double angle = atan2(kinematics.diagram.joints[2]->pos.y - points[0].y, kinematics.diagram.joints[2]->pos.x - points[0].x);

			// update the lengths of the trajectory of the moving body
			std::vector<glm::dvec2> next_body_pts = kinematics.diagram.bodies[0]->getActualPoints()[0];
			for (int i = 0; i < next_body_pts.size(); i++) {
				double length = glm::length(next_body_pts[i] - prev_body_pts[i]);
				length_of_trajectory += length;
				prev_body_pts[i] = next_body_pts[i];
			}

			// convert the sign of the angle
			if (type == 1 && angle > 0) {
				angle -= M_PI * 2;
			}
			else if (type == 2 && angle > angles[0]) {
				angle -= M_PI * 2;
			}
			else if (type == 3 && angle < angles[0]) {
				angle += M_PI * 2;
			}
			else if (type == 4 && angle > 0) {
				angle -= M_PI * 2;
			}
			else if (type == 5 && angle < 0) {
				angle += M_PI * 2;
			}
			else if (type == 6 && angle > angles[0]) {
				angle -= M_PI * 2;
			}
			else if (type == 7 && angle < angles[0]) {
				angle += M_PI * 2;
			}
			else if (type == 8 && angle < 0) {
				angle += M_PI * 2;
			}

			// check if the poses are reached
			for (int i = 0; i < angles.size(); i++) {
				if (visited[i]) continue;

				if (angles[2] >= angles[0]) {
					if (angle >= angles[i]) {
						visited[i] = true;
						unvisited--;
					}
				}
				else {
					if (angle <= angles[i]) {
						visited[i] = true;
						unvisited--;
					}
				}
			}

			// if all the poses are reached without collision, no collision is detected.
			if (unvisited == 0) {
				kinematics.clear();
				return length_of_trajectory / length_of_straight;
			}
		}

		kinematics.clear();
		return length_of_trajectory / length_of_straight;
	}

}