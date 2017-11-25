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
		std::vector<std::vector<double>> pose_params;
	};

	class SolverDerivForLink {
	public:
		SolverDerivForLink(const std::vector<glm::dmat3x3>& poses);
		const column_vector operator() (const column_vector& arg) const;

	private:
		std::vector<std::vector<double>> pose_params;
	};

	class SolverForSlider {
	public:
		SolverForSlider(const std::vector<glm::dmat3x3>& poses);
		double operator() (const column_vector& arg) const;

	private:
		std::vector<glm::dmat3x3> poses;
		glm::dmat3x3 inv_pose0;
	};

	class SolverFor3RLink {
	public:
		SolverFor3RLink(const std::vector<glm::dmat3x3>& poses);
		double operator() (const column_vector& arg) const;
		double calculateMinLengthError(const std::vector<double>& lengths, int pose_id, const std::vector<glm::dvec2>& points) const;
		double lengthError(const std::vector<double> lengths, const std::vector<glm::dvec2>& points) const;

	private:
		std::vector<glm::dmat3x3> poses;
		glm::dmat3x3 inv_pose0;
	};

	class SolverForRRLink {
	public:
		SolverForRRLink(const std::vector<glm::dmat3x3>& poses0, const std::vector<glm::dmat3x3>& poses1);
		double operator() (const column_vector& arg) const;

	private:
		std::vector<glm::dmat3x3> poses0;
		std::vector<glm::dmat3x3> poses1;
		glm::dmat3x3 inv_pose0;
		glm::dmat3x3 inv_pose1;
	};

}

