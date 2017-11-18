#include "LeastSquareSolver.h"
#include "KinematicUtils.h"

namespace kinematics {

	SolverForLink::SolverForLink(const std::vector<glm::dmat3x3>& poses) {
		this->poses = poses;
	}

	double SolverForLink::operator() (const column_vector& arg) const {
		glm::dvec2 A0(arg(0, 0), arg(1, 0));
		glm::dvec2 a(arg(2, 0), arg(3, 0));

		glm::dvec2 A1(poses[0] * glm::dvec3(a, 1));
		double l1_squared = glm::length(A1 - A0);
		l1_squared = l1_squared * l1_squared;

		double ans = 0.0;
		for (int i = 1; i < poses.size(); i++) {
			glm::dvec2 A(poses[i] * glm::dvec3(a, 1));
			double l_squared = glm::length(A - A0);
			l_squared = l_squared * l_squared;
			ans += (l_squared - l1_squared) * (l_squared - l1_squared);
		}

		return ans;
	}

	SolverForSlider::SolverForSlider(const std::vector<glm::dmat3x3>& poses) {
		this->poses = poses;
	}

	double SolverForSlider::operator() (const column_vector& arg) const {
		glm::dvec2 a(arg(0, 0), arg(1, 0));

		glm::dvec2 A1(poses[0] * glm::dvec3(a, 1));
		glm::dvec2 A2(poses[1] * glm::dvec3(a, 1));

		glm::dvec2 v1 = A2 - A1;
		v1 /= glm::length(v1);

		double ans = 0.0;
		for (int i = 2; i < poses.size(); i++) {
			glm::dvec2 A(poses[i] * glm::dvec3(a, 1));
			glm::dvec2 v = A - A1;
			v /= glm::length(v);

			ans += abs(crossProduct(v1, v));
		}

		return ans;
	}

	SolverForWattI::SolverForWattI(const std::vector<glm::dmat3x3>& poses) {
		this->poses = poses;
		inv_pose0 = glm::inverse(poses[0]);
	}

	double SolverForWattI::operator() (const column_vector& arg) const {
		std::vector<glm::dvec2> points;
		for (int i = 0; i < arg.size() / 2; i++) {
			points[i] = glm::dvec2(arg(i * 2, 0), arg(i * 2 + 1, 0));
		}

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

		double ans = 0.0;
		for (int i = 1; i < poses.size(); i++) {
			double error = calculateMinLengthError(lengths, i, points);
			ans += error * error;
		}

		return ans;
	}

	double SolverForWattI::calculateMinLengthError(const std::vector<double>& lengths, int pose_id, const std::vector<glm::dvec2>& points) const {
		glm::dvec2 p5 = glm::dvec2(glm::inverse(poses[0]) * glm::dvec3(points[5], 1));
		glm::dvec2 p6 = glm::dvec2(glm::inverse(poses[0]) * glm::dvec3(points[6], 1));

		glm::dvec2 P5 = glm::dvec2(poses[pose_id] * glm::dvec3(p5, 1));
		glm::dvec2 P6 = glm::dvec2(poses[pose_id] * glm::dvec3(p6, 1));

		double min_error = std::numeric_limits<double>::max();

		// case 1
		try {
			glm::dvec2 P2 = circleCircleIntersection(points[0], lengths[0], P5, lengths[4]);
			glm::dvec2 P4 = circleCircleIntersection(points[1], lengths[3], P6, lengths[8]);
			try {
				glm::dvec2 P3 = circleCircleIntersection(P2, lengths[1], P4, lengths[6]);
				min_error = std::min(min_error, lengthError(lengths, { points[0], points[1], P2, P3, P4, P5, P6 }));
			}
			catch (char* ex) {
			}
			try {
				glm::dvec2 P3 = circleCircleIntersection(P4, lengths[6], P2, lengths[1]);
				min_error = std::min(min_error, lengthError(lengths, { points[0], points[1], P2, P3, P4, P5, P6 }));
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
				min_error = std::min(min_error, lengthError(lengths, { points[0], points[1], P2, P3, P4, P5, P6 }));
			}
			catch (char* ex) {
			}
			try {
				glm::dvec2 P3 = circleCircleIntersection(P4, lengths[6], P2, lengths[1]);
				min_error = std::min(min_error, lengthError(lengths, { points[0], points[1], P2, P3, P4, P5, P6 }));
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
				min_error = std::min(min_error, lengthError(lengths, { points[0], points[1], P2, P3, P4, P5, P6 }));
			}
			catch (char* ex) {
			}
			try {
				glm::dvec2 P3 = circleCircleIntersection(P4, lengths[6], P2, lengths[1]);
				min_error = std::min(min_error, lengthError(lengths, { points[0], points[1], P2, P3, P4, P5, P6 }));
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
				min_error = std::min(min_error, lengthError(lengths, { points[0], points[1], P2, P3, P4, P5, P6 }));
			}
			catch (char* ex) {
			}
			try {
				glm::dvec2 P3 = circleCircleIntersection(P4, lengths[6], P2, lengths[1]);
				min_error = std::min(min_error, lengthError(lengths, { points[0], points[1], P2, P3, P4, P5, P6 }));
			}
			catch (char* ex) {
			}
		}
		catch (char* ex) {
		}

		return min_error;
	}

	double SolverForWattI::lengthError(const std::vector<double> lengths, const std::vector<glm::dvec2>& points) const {
		double ans = 0.0;
		ans += glm::length2(points[2] - points[0]) - lengths[0];
		ans += glm::length2(points[3] - points[2]) - lengths[1];
		ans += glm::length2(points[1] - points[3]) - lengths[2];
		ans += glm::length2(points[4] - points[1]) - lengths[3];
		ans += glm::length2(points[5] - points[2]) - lengths[4];
		ans += glm::length2(points[5] - points[3]) - lengths[5];
		ans += glm::length2(points[4] - points[3]) - lengths[6];
		ans += glm::length2(points[6] - points[5]) - lengths[7];
		ans += glm::length2(points[6] - points[4]) - lengths[8];
		return ans;
	}

}