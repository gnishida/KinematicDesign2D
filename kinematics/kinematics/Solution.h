#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace kinematics {

	class Solution {
	public:
		std::vector<glm::dvec2> points;
		double position_error;
		double orientation_error;
		std::vector<glm::dmat3x3> poses;

	public:
		Solution() {}
		Solution(const std::vector<glm::dvec2>& points, double position_error, double orientation_error, const std::vector<glm::dmat3x3>& poses);
	};

}