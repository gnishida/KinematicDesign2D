#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <dlib/optimization.h>

namespace kinematics {
	typedef dlib::matrix<double, 0, 1> column_vector;

	class SolverForLink {
	public:
		SolverForLink(const std::vector<glm::dmat3x3>& poses);
		double operator() (const column_vector& arg) const;

	private:
		std::vector<glm::dmat3x3> poses;
	};

	class SolverForSlider {
	public:
		SolverForSlider(const std::vector<glm::dmat3x3>& poses);
		double operator() (const column_vector& arg) const;

	private:
		std::vector<glm::dmat3x3> poses;
	};

	class SolverForWattI {
	public:
		SolverForWattI(const std::vector<glm::dmat3x3>& poses);
		double operator() (const column_vector& arg) const;
		double calculateMinLengthError(const std::vector<double>& lengths, int pose_id, const std::vector<glm::dvec2>& points) const;
		double lengthError(const std::vector<double> lengths, const std::vector<glm::dvec2>& points) const;

	private:
		std::vector<glm::dmat3x3> poses;
		glm::dmat3x3 inv_pose0;
	};

}

