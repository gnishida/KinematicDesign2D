#include "LinkageSynthesisWattI.h"
#include "KinematicUtils.h"
#include "Kinematics.h"
#include "PinJoint.h"
#include "SliderHinge.h"
#include "BoundingBox.h"
#include "LeastSquareSolver.h"
#include <opencv2/opencv.hpp>

namespace kinematics {

	/**
	* Calculate solutions of Watt I.
	*
	* @param poses			three poses
	* @param solutions1	the output solutions for the world coordinates of the driving crank at the first pose, each of which contains a pair of the center point and the circle point
	* @param solutions2	the output solutions for the world coordinates of the follower at the first pose, each of which contains a pair of the center point and the circle point
	*/
	void LinkageSynthesisWattI::calculateSolution(const std::vector<glm::dmat3x3>& poses, const std::vector<glm::dvec2>& linkage_region_pts, const std::vector<glm::dvec2>& linkage_avoidance_pts, int num_samples, const std::vector<Object25D>& fixed_body_pts, const Object25D& body_pts, std::vector<std::pair<double, double>>& sigmas, bool rotatable_crank, bool avoid_branch_defect, double min_link_length, std::vector<Solution>& solutions, std::vector<glm::dvec2>& enlarged_linkage_region_pts) {
		solutions.clear();

		srand(0);

		// calculate the center of the valid regions
		BBox bbox = boundingBox(linkage_region_pts);
		glm::dvec2 bbox_center = bbox.center();

		int cnt = 0;
		for (int scale = 1; scale <= 3 && cnt == 0; scale++) {
			// calculate the enlarged linkage region for the sampling region
			enlarged_linkage_region_pts.clear();
			for (int i = 0; i < linkage_region_pts.size(); i++) {
				enlarged_linkage_region_pts.push_back((linkage_region_pts[i] - bbox_center) * (double)scale + bbox_center);
			}

			// calculate the bounding boxe of the valid regions
			BBox enlarged_bbox = boundingBox(enlarged_linkage_region_pts);

			for (int iter = 0; iter < num_samples && cnt == 0; iter++) {
				printf("\rsampling %d/%d", cnt, iter + 1);

				// perturbe the poses a little
				double position_error = 0.0;
				double orientation_error = 0.0;
				std::vector<glm::dmat3x3> perturbed_poses = perturbPoses(poses, sigmas, position_error, orientation_error);

				// sample joints within the linkage region
				std::vector<glm::dvec2> points(7);
				for (int i = 0; i < points.size(); i++) {
					while (true) {
						points[i] = glm::dvec2(genRand(enlarged_bbox.minPt.x, enlarged_bbox.maxPt.x), genRand(enlarged_bbox.minPt.y, enlarged_bbox.maxPt.y));
						if (withinPolygon(enlarged_linkage_region_pts, points[i])) break;
					}
				}

				if (!optimizeCandidate(perturbed_poses, enlarged_linkage_region_pts, enlarged_bbox, points)) continue;

				// check hard constraints
				std::vector<std::vector<int>> zorder;
				if (!checkHardConstraints(points, perturbed_poses, enlarged_linkage_region_pts, linkage_avoidance_pts, fixed_body_pts, body_pts, rotatable_crank, avoid_branch_defect, min_link_length)) continue;

				solutions.push_back(Solution(points, position_error, orientation_error, perturbed_poses));
				cnt++;
			}
		}
		printf("\n");
	}

	/**
	* Optimize the linkage parameters based on the rigidity constraints.
	* If it fails to optimize, return false.
	*/
	bool LinkageSynthesisWattI::optimizeCandidate(const std::vector<glm::dmat3x3>& poses, const std::vector<glm::dvec2>& linkage_region_pts, const BBox& bbox, std::vector<glm::dvec2>& points) {
		// setup the initial parameters for optimization
		column_vector starting_point(points.size() * 2);
		for (int i = 0; i < points.size(); i++) {
			starting_point(i * 2 + 0, 0) = points[i].x;
			starting_point(i * 2 + 1, 0) = points[i].y;
		}

		column_vector lower_bound(points.size() * 2);
		column_vector upper_bound(points.size() * 2);
		double min_range = std::numeric_limits<double>::max();
		for (int i = 0; i < points.size(); i++) {
			// set the lower bound
			lower_bound(i * 2 + 0, 0) = bbox.minPt.x;
			lower_bound(i * 2 + 0, 0) = std::min(lower_bound(i * 2 + 0, 0), starting_point(i * 2 + 0, 0));

			lower_bound(i * 2 + 1, 0) = bbox.minPt.y;
			lower_bound(i * 2 + 1, 0) = std::min(lower_bound(i * 2 + 1, 0), starting_point(i * 2 + 1, 0));

			// set the upper bound
			upper_bound(i * 2 + 0, 0) = bbox.maxPt.x;
			upper_bound(i * 2 + 0, 0) = std::max(upper_bound(i * 2 + 0, 0), starting_point(i * 2 + 0, 0));

			upper_bound(i * 2 + 1, 0) = bbox.maxPt.y;
			upper_bound(i * 2 + 1, 0) = std::max(upper_bound(i * 2 + 1, 0), starting_point(i * 2 + 1, 0));

			min_range = std::min(min_range, upper_bound(i * 2 + 0, 0) - lower_bound(i * 2 + 0, 0));
			min_range = std::min(min_range, upper_bound(i * 2 + 1, 0) - lower_bound(i * 2 + 1, 0));
		}

		try {
			find_min_bobyqa(SolverForWattI(poses), starting_point, 32, lower_bound, upper_bound, min_range * 0.19, min_range * 0.0001, 1000);

			//check(poses, starting_point);

			for (int i = 0; i < points.size(); i++) {
				points[0] = glm::dvec2(starting_point(i * 2 + 0, 0), starting_point(i * 2 + 1, 0));

				// if the joints are outside the valid region, discard it.
				if (!withinPolygon(linkage_region_pts, points[0])) return false;
			}
		}
		catch (std::exception& e) {
			return false;
		}

		return true;
	}

	Solution LinkageSynthesisWattI::findBestSolution(const std::vector<glm::dmat3x3>& poses, std::vector<Solution>& solutions, const std::vector<glm::dvec2>& linkage_region_pts, const cv::Mat& dist_map, const BBox& dist_map_bbox, const std::vector<glm::dvec2>& linkage_avoidance_pts, const std::vector<Object25D>& fixed_body_pts, const Object25D& body_pts, bool rotatable_crank, bool avoid_branch_defect, double min_link_length, const std::vector<double>& weights, int num_particles, int num_iterations, bool record_file) {
		// select the best solution based on the objective function
		if (solutions.size() > 0) {
			particleFilter(solutions, linkage_region_pts, dist_map, dist_map_bbox, linkage_avoidance_pts, fixed_body_pts, body_pts, rotatable_crank, avoid_branch_defect, min_link_length, weights, num_particles, num_iterations, record_file);
			return solutions[0];
		}
		else {
			return Solution({ { 0, 0 }, { 2, 0 }, { 0, 2 }, { 1, 1 }, { 2, 2 }, { 1, 3 } }, 0, 0, poses);
		}
	}

	double LinkageSynthesisWattI::calculateCost(Solution& solution, const Object25D& body_pts, const cv::Mat& dist_map, const BBox& dist_map_bbox, const std::vector<double>& weights) {
		double dist = 0;
		for (int i = 0; i < solution.points.size(); i++) {
			dist += dist_map.at<double>(solution.points[i].y - dist_map_bbox.minPt.y, solution.points[i].x - dist_map_bbox.minPt.x);
		}
		double tortuosity = tortuosityOfTrajectory(solution.poses, solution.points, body_pts);
		double size = glm::length(solution.points[0] - solution.points[2]) + glm::length(solution.points[1] - solution.points[3]) + glm::length(solution.points[2] - solution.points[3]);

		return solution.position_error * weights[0] + solution.orientation_error * weights[1] + dist * weights[2] + tortuosity * weights[3] + size * weights[4];
	}

	/**
	* Construct a linkage.
	*/
	Kinematics LinkageSynthesisWattI::constructKinematics(const std::vector<glm::dvec2>& points, const Object25D& body_pts, const std::vector<Object25D>& fixed_body_pts) {
		Kinematics kin;
		kin.diagram.addJoint(boost::shared_ptr<kinematics::PinJoint>(new kinematics::PinJoint(0, true, points[0])));
		kin.diagram.addJoint(boost::shared_ptr<kinematics::PinJoint>(new kinematics::PinJoint(1, true, points[1])));
		kin.diagram.addJoint(boost::shared_ptr<kinematics::PinJoint>(new kinematics::PinJoint(2, false, points[2])));
		kin.diagram.addJoint(boost::shared_ptr<kinematics::PinJoint>(new kinematics::PinJoint(3, false, points[3])));
		kin.diagram.addJoint(boost::shared_ptr<kinematics::PinJoint>(new kinematics::PinJoint(4, false, points[4])));
		kin.diagram.addJoint(boost::shared_ptr<kinematics::PinJoint>(new kinematics::PinJoint(5, false, points[5])));
		kin.diagram.addJoint(boost::shared_ptr<kinematics::PinJoint>(new kinematics::PinJoint(6, false, points[6])));
		kin.diagram.addLink(true, kin.diagram.joints[0], kin.diagram.joints[2]);
		kin.diagram.addLink(false, { kin.diagram.joints[1], kin.diagram.joints[3], kin.diagram.joints[4] });
		kin.diagram.addLink(false, { kin.diagram.joints[2], kin.diagram.joints[3], kin.diagram.joints[5] });
		kin.diagram.addLink(false, kin.diagram.joints[4], kin.diagram.joints[6]);
		kin.diagram.addLink(false, kin.diagram.joints[5], kin.diagram.joints[6]);

		std::vector<Object25D> copied_fixed_body_pts = fixed_body_pts;

		// update the geometry
		updateBodies(kin, body_pts);

		// add the fixed rigid bodies
		for (int i = 0; i < copied_fixed_body_pts.size(); i++) {
			kin.diagram.addBody(kin.diagram.joints[0], kin.diagram.joints[1], copied_fixed_body_pts[i]);
		}

		return kin;
	}

	/**
	* update bodies.
	*/
	void LinkageSynthesisWattI::updateBodies(Kinematics& kin, const Object25D& body_pts) {
		kin.diagram.bodies.clear();
		kin.diagram.addBody(kin.diagram.joints[2], kin.diagram.joints[5], body_pts);
	}

	bool LinkageSynthesisWattI::checkHardConstraints(std::vector<glm::dvec2>& points, const std::vector<glm::dmat3x3>& poses, const std::vector<glm::dvec2>& linkage_region_pts, const std::vector<glm::dvec2>& linkage_avoidance_pts, const std::vector<Object25D>& fixed_body_pts, const Object25D& body_pts, bool rotatable_crank, bool avoid_branch_defect, double min_link_length) {
		if (glm::length(points[0] - points[1]) < min_link_length) return false;
		if (glm::length(points[2] - points[3]) < min_link_length) return false;

		//if (checkFolding(points)) return false;
		if (rotatable_crank && checkRotatableCrankDefect(points)) return false;
		if (avoid_branch_defect && checkBranchDefect(poses, points)) return false;
		if (checkCircuitDefect(poses, points)) return false;
		if (checkOrderDefect(poses, points)) return false;

		// collision check for the main body
		// body_pts[0] means the main body without the joint connectors
		if (checkCollision(poses, points, fixed_body_pts, body_pts[0], linkage_avoidance_pts)) return false;

		return true;
	}

	int LinkageSynthesisWattI::getType(const std::vector<glm::dvec2>& points) {
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

	/**
	* Check if the linkage has a rotatable crank defect.
	* If the crank is not fully rotatable, true is returned.
	*/
	bool LinkageSynthesisWattI::checkRotatableCrankDefect(const std::vector<glm::dvec2>& points) {
		int linkage_type = getType(points);
		int linkage_type2 = getType({ points[3], points[4], points[5], points[6] });

		if ((linkage_type == 0 || linkage_type == 1) && (linkage_type2 == 0 || linkage_type2 == 1)) {
			return false;
		}
		else {
			return true;
		}
	}

	std::pair<double, double> LinkageSynthesisWattI::checkRange(const std::vector<glm::dvec2>& points) {
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

		return{ theta_min, theta_max };
	}

	bool LinkageSynthesisWattI::checkOrderDefect(const std::vector<glm::dmat3x3>& poses, const std::vector<glm::dvec2>& points) {
		glm::dvec2 inv_W = glm::dvec2(glm::inverse(poses[0]) * glm::dvec3(points[2], 1));

		int linkage_type = getType(points);
		std::pair<double, double> range = checkRange(points);

		double total_cw = 0;
		double total_ccw = 0;
		double prev = 0;
		for (int i = 0; i < poses.size(); i++) {
			// calculate the coordinates of the circle point of the driving crank in the world coordinate system
			glm::dvec2 X = glm::dvec2(poses[i] * glm::dvec3(inv_W, 1));

			// calculate the direction from the ground pivot (center point) of the driving crank to the circle point
			glm::dvec2 dir = X - points[0];

			// calculate its angle
			double theta = atan2(dir.y, dir.x);

			if (theta >= prev) {
				if (linkage_type == 0 || linkage_type == 2) {
					total_cw += M_PI * 2 - theta + prev;
					total_ccw += theta - prev;
				}
				else if (linkage_type == 2 || linkage_type == 3 || linkage_type == 4 || linkage_type == 7) {
					total_cw = M_PI * 999; // out of range
					total_ccw += theta - prev;
				}
				else if (linkage_type == 5 || linkage_type == 6) {
					if (theta < range.first) {
						theta += M_PI * 2;
					}
					total_cw = M_PI * 999; // out of range
					total_ccw += theta - prev;
				}
			}
			else {
				if (linkage_type == 0 || linkage_type == 2) {
					total_cw += prev - theta;
					total_ccw += M_PI * 2 - prev + theta;
				}
				else if (linkage_type == 2 || linkage_type == 3 || linkage_type == 4 || linkage_type == 7) {
					total_cw += prev - theta;
					total_ccw = M_PI * 999;	// out of range
				}
				else if (linkage_type == 5 || linkage_type == 6) {
					if (theta < range.first) {
						theta += M_PI * 2;
					}
					total_cw += prev - theta;
					total_ccw = M_PI * 999;	// out of range
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
	bool LinkageSynthesisWattI::checkBranchDefect(const std::vector<glm::dmat3x3>& poses, const std::vector<glm::dvec2>& points) {
		int type = getType(points);

		// drag-link and crank-rocker always do not have a branch defect
		if (type == 0 || type == 1) return false;

		int orig_sign = crossProduct(points[3] - points[2], points[1] - points[3]) >= 0 ? 1 : -1;

		for (int i = 1; i < poses.size(); i++) {
			std::vector<glm::dvec2> current_points = calculatePositions(poses, i, points);

			// calculate its sign
			int sign = crossProduct(current_points[3] - current_points[2], current_points[1] - current_points[3]) >= 0 ? 1 : -1;
			if (sign != orig_sign) {
				return true;
			}
		}

		return false;
	}

	bool LinkageSynthesisWattI::checkCircuitDefect(const std::vector<glm::dmat3x3>& poses, const std::vector<glm::dvec2>& points) {
		int type = getType(points);

		int orig_sign = 1;
		int orig_sign2 = 1;
		if (type == 0) {
			orig_sign = crossProduct(points[2] - points[0], points[3] - points[2]) >= 0 ? 1 : -1;
		}
		else if (type == 1) {
			orig_sign = crossProduct(points[3] - points[2], points[1] - points[3]) >= 0 ? 1 : -1;
		}
		else if (type == 2) {
			orig_sign = crossProduct(points[0] - points[1], points[2] - points[0]) >= 0 ? 1 : -1;
		}
		else if (type == 3) {
			orig_sign = crossProduct(points[1] - points[3], points[0] - points[1]) >= 0 ? 1 : -1;
		}
		orig_sign2 = crossProduct(points[6] - points[5], points[4] - points[6]) >= 0 ? 1 : -1;

		for (int i = 1; i < poses.size(); i++) {
			std::vector<glm::dvec2> current_points = calculatePositions(poses, i, points);

			int sign, sign2;
			if (type == 0) {
				sign = crossProduct(current_points[2] - current_points[0], current_points[3] - current_points[2]) >= 0 ? 1 : -1;
			}
			else if (type == 1) {
				sign = crossProduct(current_points[3] - current_points[2], current_points[1] - current_points[3]) >= 0 ? 1 : -1;
			}
			else if (type == 2) {
				sign = crossProduct(current_points[0] - current_points[1], current_points[2] - current_points[0]) >= 0 ? 1 : -1;
			}
			else if (type == 3) {
				sign = crossProduct(current_points[1] - current_points[3], current_points[0] - current_points[1]) >= 0 ? 1 : -1;
			}
			else {
				sign = orig_sign;
			}

			sign2 = crossProduct(current_points[6] - current_points[5], current_points[4] - current_points[6]) >= 0 ? 1 : -1;

			if (sign != orig_sign || sign2 != orig_sign2) return true;
		}

		return false;
	}

	bool LinkageSynthesisWattI::checkCollision(const std::vector<glm::dmat3x3>& poses, const std::vector<glm::dvec2>& points, const std::vector<Object25D>& fixed_body_pts, const Object25D& body_pts, const std::vector<glm::dvec2>& linkage_avoidance_pts) {
		kinematics::Kinematics kinematics = constructKinematics(points, body_pts, fixed_body_pts);
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

	double LinkageSynthesisWattI::tortuosityOfTrajectory(const std::vector<glm::dmat3x3>& poses, const std::vector<glm::dvec2>& points, const Object25D& body_pts) {
		return false;
	}
	
	std::vector<glm::dvec2> LinkageSynthesisWattI::calculatePositions(const std::vector<glm::dmat3x3>& poses, int pose_id, const std::vector<glm::dvec2>& points) {
		// length of links
		std::vector<double> lengths(9);
		lengths[0] = glm::length(points[2] - points[0]);
		lengths[1] = glm::length(points[3] - points[2]);
		lengths[2] = glm::length(points[3] - points[1]);
		lengths[3] = glm::length(points[4] - points[1]);
		lengths[4] = glm::length(points[5] - points[2]);
		lengths[5] = glm::length(points[5] - points[3]);
		lengths[6] = glm::length(points[4] - points[3]);
		lengths[7] = glm::length(points[6] - points[5]);
		lengths[8] = glm::length(points[6] - points[4]);

		glm::dvec2 p5 = glm::dvec2(glm::inverse(poses[0]) * glm::dvec3(points[5], 1));
		glm::dvec2 p6 = glm::dvec2(glm::inverse(poses[0]) * glm::dvec3(points[6], 1));

		glm::dvec2 P5 = glm::dvec2(poses[pose_id] * glm::dvec3(p5, 1));
		glm::dvec2 P6 = glm::dvec2(poses[pose_id] * glm::dvec3(p6, 1));

		// case 1
		try {
			glm::dvec2 P2 = circleCircleIntersection(points[0], lengths[0], P5, lengths[4]);
			glm::dvec2 P4 = circleCircleIntersection(points[1], lengths[3], P6, lengths[8]);
			try {
				glm::dvec2 P3 = circleCircleIntersection(P2, lengths[1], P4, lengths[6]);
				if (checkPositions(lengths, { points[0], points[1], P2, P3, P4, P5, P6 })) {
					return{ points[0], points[1], P2, P3, P4, P5, P6 };
				}
			}
			catch (char* ex) {
			}
			try {
				glm::dvec2 P3 = circleCircleIntersection(P4, lengths[6], P2, lengths[1]);
				if (checkPositions(lengths, { points[0], points[1], P2, P3, P4, P5, P6 })) {
					return{ points[0], points[1], P2, P3, P4, P5, P6 };
				}
			}
			catch (char* ex) {
			}
		}
		catch (char* ex) {
		}

		// case 2
		try {
			glm::dvec2 P2 = circleCircleIntersection(points[0], lengths[0], P5, lengths[4]);
			glm::dvec2 P4 = circleCircleIntersection(P6, lengths[8], points[1], lengths[3]);
			try {
				glm::dvec2 P3 = circleCircleIntersection(P2, lengths[1], P4, lengths[6]);
				if (checkPositions(lengths, { points[0], points[1], P2, P3, P4, P5, P6 })) {
					return{ points[0], points[1], P2, P3, P4, P5, P6 };
				}
			}
			catch (char* ex) {
			}
			try {
				glm::dvec2 P3 = circleCircleIntersection(P4, lengths[6], P2, lengths[1]);
				if (checkPositions(lengths, { points[0], points[1], P2, P3, P4, P5, P6 })) {
					return{ points[0], points[1], P2, P3, P4, P5, P6 };
				}
			}
			catch (char* ex) {
			}
		}
		catch (char* ex) {
		}

		// case 3
		try {
			glm::dvec2 P2 = circleCircleIntersection(P5, lengths[4], points[0], lengths[0]);
			glm::dvec2 P4 = circleCircleIntersection(points[1], lengths[3], P6, lengths[8]);
			try {
				glm::dvec2 P3 = circleCircleIntersection(P2, lengths[1], P4, lengths[6]);
				if (checkPositions(lengths, { points[0], points[1], P2, P3, P4, P5, P6 })) {
					return{ points[0], points[1], P2, P3, P4, P5, P6 };
				}
			}
			catch (char* ex) {
			}
			try {
				glm::dvec2 P3 = circleCircleIntersection(P4, lengths[6], P2, lengths[1]);
				if (checkPositions(lengths, { points[0], points[1], P2, P3, P4, P5, P6 })) {
					return{ points[0], points[1], P2, P3, P4, P5, P6 };
				}
			}
			catch (char* ex) {
			}
		}
		catch (char* ex) {
		}

		// case 4
		try {
			glm::dvec2 P2 = circleCircleIntersection(P5, lengths[4], points[0], lengths[0]);
			glm::dvec2 P4 = circleCircleIntersection(P6, lengths[8], points[1], lengths[3]);
			try {
				glm::dvec2 P3 = circleCircleIntersection(P2, lengths[1], P4, lengths[6]);
				if (checkPositions(lengths, { points[0], points[1], P2, P3, P4, P5, P6 })) {
					return{ points[0], points[1], P2, P3, P4, P5, P6 };
				}
			}
			catch (char* ex) {
			}
			try {
				glm::dvec2 P3 = circleCircleIntersection(P4, lengths[6], P2, lengths[1]);
				if (checkPositions(lengths, { points[0], points[1], P2, P3, P4, P5, P6 })) {
					return{ points[0], points[1], P2, P3, P4, P5, P6 };
				}
			}
			catch (char* ex) {
			}
		}
		catch (char* ex) {
		}

		throw "No valid solution.";
	}

	bool LinkageSynthesisWattI::checkPositions(const std::vector<double>& lengths, const std::vector<glm::dvec2>& points) {
		if (std::abs(glm::length(points[2] - points[0]) - lengths[0]) > TOL) return false;
		if (std::abs(glm::length(points[3] - points[2]) - lengths[1]) > TOL) return false;
		if (std::abs(glm::length(points[3] - points[1]) - lengths[2]) > TOL) return false;
		if (std::abs(glm::length(points[4] - points[1]) - lengths[3]) > TOL) return false;
		if (std::abs(glm::length(points[5] - points[2]) - lengths[4]) > TOL) return false;
		if (std::abs(glm::length(points[5] - points[3]) - lengths[5]) > TOL) return false;
		if (std::abs(glm::length(points[4] - points[3]) - lengths[6]) > TOL) return false;
		if (std::abs(glm::length(points[6] - points[5]) - lengths[7]) > TOL) return false;
		if (std::abs(glm::length(points[6] - points[4]) - lengths[8]) > TOL) return false;

		return true;
	}

}
