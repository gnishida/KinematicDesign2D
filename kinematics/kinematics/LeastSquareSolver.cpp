#include "LeastSquareSolver.h"
#include "KinematicUtils.h"

namespace kinematics {

	SolverForLink::SolverForLink(const std::vector<glm::dmat3x3>& poses) {
		pose_params.resize(poses.size() - 1, std::vector<double>(3));
		for (int i = 1; i < poses.size(); i++) {
			glm::dmat3x3 D = poses[i] * glm::inverse(poses[0]);
			pose_params[i - 1][0] = std::atan2(D[0][1], D[0][0]);
			pose_params[i - 1][1] = D[2][0];
			pose_params[i - 1][2] = D[2][1];
		}

		this->pose_params = pose_params;
	}

	double SolverForLink::operator() (const column_vector& arg) const {
		glm::dvec2 A(arg(0), arg(1));
		glm::dvec2 B(arg(2), arg(3));

		double ans = 0.0;
		for (int i = 0; i < pose_params.size(); i++) {
			double theta = pose_params[i][0];
			double u = pose_params[i][1];
			double v = pose_params[i][2];

			double z = 2 * (A.x * B.x + A.y * B.y) * (1 - std::cos(theta))
				+ 2 * (A.x * B.y - A.y * B.x) * std::sin(theta)
				- 2 * u * A.x
				- 2 * v * A.y
				+ 2 * B.x * (u * std::cos(theta) + v * std::sin(theta))
				+ 2 * B.y * (-u * std::sin(theta) + v * std::cos(theta))
				+ u * u
				+ v * v;

			ans += z * z;
		}

		return ans;
	}

	SolverDerivForLink::SolverDerivForLink(const std::vector<glm::dmat3x3>& poses) {
		pose_params.resize(poses.size() - 1, std::vector<double>(3));
		for (int i = 1; i < poses.size(); i++) {
			glm::dmat3x3 D = poses[i] * glm::inverse(poses[0]);
			pose_params[i - 1][0] = std::atan2(D[0][1], D[0][0]);
			pose_params[i - 1][1] = D[2][0];
			pose_params[i - 1][2] = D[2][1];
		}

		this->pose_params = pose_params;
	}

	const column_vector SolverDerivForLink::operator() (const column_vector& arg) const {
		glm::dvec2 A(arg(0), arg(1));
		glm::dvec2 B(arg(2), arg(3));

		column_vector ans(4);
		for (int i = 0; i < 4; i++) ans(i) = 0;

		for (int i = 0; i < pose_params.size(); i++) {
			double theta = pose_params[i][0];
			double u = pose_params[i][1];
			double v = pose_params[i][2];

			double z = 2 * (A.x * B.x + A.y * B.y) * (1 - std::cos(theta))
				+ 2 * (A.x * B.y - A.y * B.x) * std::sin(theta)
				- 2 * u * A.x
				- 2 * v * A.y
				+ 2 * B.x * (u * std::cos(theta) + v * std::sin(theta))
				+ 2 * B.y * (-u * std::sin(theta) + v * std::cos(theta))
				+ u * u
				+ v * v;

			ans(0) += 4 * z * (B.x * (1 - std::cos(theta)) + B.y * std::sin(theta) - u);
			ans(1) += 4 * z * (B.y * (1 - std::cos(theta)) - B.x * std::sin(theta) - v);
			ans(2) += 4 * z * (A.x * (1 - std::cos(theta)) - A.y * std::sin(theta) + u * std::cos(theta) + v * std::sin(theta));
			ans(3) += 4 * z * (A.y * (1 - std::cos(theta)) + A.x * std::sin(theta) - u * std::sin(theta) + v * std::cos(theta));
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