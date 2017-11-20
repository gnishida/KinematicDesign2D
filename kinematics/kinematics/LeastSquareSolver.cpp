#include "LeastSquareSolver.h"
#include "KinematicUtils.h"

namespace kinematics {

	SolverForLink::SolverForLink(const std::vector<glm::dmat3x3>& poses) {
		this->poses = poses;
		inv_pose0 = glm::inverse(poses[0]);
	}

	double SolverForLink::operator() (const column_vector& arg) const {
		glm::dvec2 A0(arg(0, 0), arg(1, 0));
		glm::dvec2 A1(arg(2, 0), arg(3, 0));

		glm::dvec2 a(inv_pose0 * glm::dvec3(A1, 1));
		double l_initial = glm::length2(A1 - A0);

		double ans = 0.0;
		for (int i = 1; i < poses.size(); i++) {
			glm::dvec2 A(poses[i] * glm::dvec3(a, 1));
			double l = glm::length2(A - A0);
			ans += (l_initial - l) * (l_initial - l);
		}

		return ans;
	}

	SolverForSlider::SolverForSlider(const std::vector<glm::dmat3x3>& poses) {
		this->poses = poses;
		inv_pose0 = glm::inverse(poses[0]);
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

	SolverFor3RLink::SolverFor3RLink(const std::vector<glm::dmat3x3>& poses) {
		this->poses = poses;
		inv_pose0 = glm::inverse(poses[0]);
	}

	double SolverFor3RLink::operator() (const column_vector& arg) const {
		glm::dvec2 P0(arg(0, 0), arg(1, 0));
		glm::dvec2 P2(arg(2, 0), arg(3, 0));
		std::vector<glm::dvec2> P1(poses.size());
		for (int i = 0; i < poses.size(); i++) {
			P1[i].x = arg(4 + i * 2, 0);
			P1[i].y = arg(5 + i * 2, 0);
		}

		glm::dvec2 p2(inv_pose0 * glm::dvec3(P2, 1));
		double l0 = glm::length2(P1[0] - P0);
		double l1 = glm::length2(P1[0] - P2);

		double ans = 0.0;
		for (int i = 1; i < poses.size(); i++) {
			double l0b = glm::length2(P1[i] - P0);

			glm::dvec2 P(poses[i] * glm::dvec3(p2, 1));
			double l1b = glm::length2(P1[i] - P);

			ans += (l0 - l0b) * (l0 - l0b) + (l1 - l1b) * (l1 - l1b);
		}

		return ans;
	}

	SolverForRRLink::SolverForRRLink(const std::vector<glm::dmat3x3>& poses0, const std::vector<glm::dmat3x3>& poses1) {
		this->poses0 = poses0;
		this->poses1 = poses1;
		inv_pose0 = glm::inverse(poses0[0]);
		inv_pose1 = glm::inverse(poses1[0]);
	}

	double SolverForRRLink::operator() (const column_vector& arg) const {
		glm::dvec2 A0(arg(0, 0), arg(1, 0));
		glm::dvec2 A1(arg(2, 0), arg(3, 0));

		glm::dvec2 a0(inv_pose0 * glm::dvec3(A0, 1));
		glm::dvec2 a1(inv_pose1 * glm::dvec3(A1, 1));

		double l_initial = glm::length2(A1 - A0);

		double ans = 0.0;
		for (int i = 1; i < poses1.size(); i++) {
			glm::dvec2 A0b(poses0[i] * glm::dvec3(a0, 1));
			glm::dvec2 A1b(poses1[i] * glm::dvec3(a1, 1));
			double l = glm::length2(A1b - A0b);
			ans += (l_initial - l) * (l_initial - l);
		}

		return ans;
	}

}