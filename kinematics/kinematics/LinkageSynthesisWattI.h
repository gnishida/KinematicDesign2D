#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "LinkageSynthesis.h"
#include "Solution.h"
#include "BBox.h"

namespace kinematics {

	class LinkageSynthesisWattI : public LinkageSynthesis {
	public:
		LinkageSynthesisWattI() {}

	public:
		void calculateSolution(const std::vector<glm::dmat3x3>& poses, const std::vector<glm::dvec2>& linkage_region_pts, const std::vector<glm::dvec2>& linkage_avoidance_pts, int num_samples, const std::vector<Object25D>& fixed_body_pts, const Object25D& body_pts, std::vector<std::pair<double, double>>& sigmas, bool rotatable_crank, bool avoid_branch_defect, double min_link_length, std::vector<Solution>& solutions, std::vector<glm::dvec2>& enlarged_linkage_region_pts);
		bool optimizeCandidate(const std::vector<glm::dmat3x3>& poses, const std::vector<glm::dvec2>& linkage_region_pts, const BBox& bbox, std::vector<glm::dvec2>& points);
		Solution findBestSolution(const std::vector<glm::dmat3x3>& poses, std::vector<Solution>& solutions, const std::vector<glm::dvec2>& linkage_region_pts, const cv::Mat& dist_map, const BBox& dist_map_bbox, const std::vector<glm::dvec2>& linkage_avoidance_pts, const std::vector<Object25D>& fixed_body_pts, const Object25D& body_pts, bool rotatable_crank, bool avoid_branch_defect, double min_link_length, const std::vector<double>& weights, int num_particles, int num_iterations, bool record_file);
		double calculateCost(Solution& solution, const Object25D& body_pts, const cv::Mat& dist_map, const BBox& dist_map_bbox, const std::vector<double>& weights);
		Kinematics constructKinematics(const std::vector<glm::dvec2>& points, const Object25D& body_pts, const std::vector<Object25D>& fixed_body_pts);
		void updateBodies(Kinematics& kinematics, const Object25D& body_pts);

		bool checkHardConstraints(std::vector<glm::dvec2>& points, const std::vector<glm::dmat3x3>& poses, const std::vector<glm::dvec2>& linkage_region_pts, const std::vector<glm::dvec2>& linkage_avoidance_pts, const std::vector<Object25D>& fixed_body_pts, const Object25D& body_pts, bool rotatable_crank, bool avoid_branch_defect, double min_link_length);
		int getType(const std::vector<glm::dvec2>& points);
		bool checkRotatableCrankDefect(const std::vector<glm::dvec2>& points);
		std::pair<double, double> checkRange(const std::vector<glm::dvec2>& points);
		bool checkOrderDefect(const std::vector<glm::dmat3x3>& poses, const std::vector<glm::dvec2>& points);
		bool checkBranchDefect(const std::vector<glm::dmat3x3>& poses, const std::vector<glm::dvec2>& points);
		bool checkCircuitDefect(const std::vector<glm::dmat3x3>& poses, const std::vector<glm::dvec2>& points);
		bool checkCollision(const std::vector<glm::dmat3x3>& poses, const std::vector<glm::dvec2>& points, const std::vector<Object25D>& fixed_body_pts, const Object25D& body_pts, const std::vector<glm::dvec2>& linkage_avoidance_pts);
		double tortuosityOfTrajectory(const std::vector<glm::dmat3x3>& poses, const std::vector<glm::dvec2>& points, const Object25D& body_pts);

		std::vector<glm::dvec2> calculatePositions(const std::vector<glm::dmat3x3>& poses, int pose_id, const std::vector<glm::dvec2>& points);
		bool checkPositions(const std::vector<double>& lengths, const std::vector<glm::dvec2>& points);
	};

}
