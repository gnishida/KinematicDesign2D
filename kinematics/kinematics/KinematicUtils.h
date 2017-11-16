#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <vector>
#include <boost/geometry.hpp>
#include <boost/geometry/core/point_type.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/register/point.hpp>
#include <boost/geometry/geometries/register/ring.hpp>
#include "BBox.h"

namespace kinematics {

	class Polygon2D {
	public:
		std::vector<glm::dvec2> points;
		bool check_collision;

	public:
		Polygon2D() : check_collision(false) {}
		Polygon2D(const std::vector<glm::dvec2>& points, bool check_collision = true) : points(points), check_collision(check_collision) {}
	};

	class Object2D {
	public:
		std::vector<Polygon2D> polygons;
		Polygon2D operator[](int index) const {
			return polygons[index];
		}
		Polygon2D& operator[](int index) {
			return polygons[index];
		}
		void push_back(const Polygon2D& polygon) {
			polygons.push_back(polygon);
		}
		size_t size() const {
			return polygons.size();
		}
		Polygon2D& back() {
			return polygons.back();
		}
		Polygon2D back() const {
			return polygons.back();
		}

	public:
		Object2D() {}
		Object2D(const Polygon2D& polygon) {
			polygons.push_back(polygon);
		}
		Object2D(const std::vector<glm::dvec2>& points) {
			polygons.push_back(Polygon2D(points));
		}
	};

	const double M_PI = 3.14159265;
	const double TOL = 0.0000001;

	double genRand();
	double genRand(double a, double b);
	double genNormal();
	double genNormal(double myu, double sigma);

	glm::dvec2 circleCircleIntersection(const glm::dvec2& center1, double radius1, const glm::dvec2& center2, double radius);
	glm::dvec2 circleCircleIntersection(const glm::dvec2& center1, double radius1, const glm::dvec2& center2, double radius, const glm::dvec2& prev_int);
	glm::dvec2 circleLineIntersection(const glm::dvec2& center, double radius, const glm::dvec2& p1, const glm::dvec2& p2);
	glm::dvec2 circleLineIntersection(const glm::dvec2& center, double radius, const glm::dvec2& p1, const glm::dvec2& p2, const glm::dvec2& prev_int);
	bool polygonPolygonIntersection(const std::vector<glm::dvec2>& polygon1, const std::vector<glm::dvec2>& polygon2);
	bool lineLineIntersection(const glm::dvec2& a, const glm::dvec2& u, const glm::dvec2& b, const glm::dvec2& v, glm::dvec2& intPoint);
	bool segmentSegmentIntersection(const glm::dvec2& a, const glm::dvec2& b, const glm::dvec2& c, const glm::dvec2& d, glm::dvec2& intPoint);
	double distanceToSegment(const glm::dvec2& a, const glm::dvec2& b, const glm::dvec2& c);
	glm::dvec2 circleCenterFromThreePoints(const glm::dvec2& a, const glm::dvec2& b, const glm::dvec2& c);
	glm::dvec2 threeLengths(const glm::dvec2& a, double l0, const glm::dvec2& b, double l1, const glm::dvec2& c, double l2, double r0, double r1, double r2, const glm::dvec2& prev_pos, const glm::dvec2& prev_pos2, const glm::dvec2& prev_pos3);
	double threeLengths(const glm::dvec2& a, double l0, const glm::dvec2& b, double l1, const glm::dvec2& c, double l2, double r0, double r1, double r2, const glm::dvec2& prev_pos, const glm::dvec2& prev_pos2, const glm::dvec2& prev_pos3, double theta0, double theta1, double delta_theta);
	void circleFit(const std::vector<glm::dvec2>& p, glm::dvec2& c, double& r);
	glm::dvec2 closestOffsetPoint(const std::vector<glm::dvec2>& points, const glm::dvec2& p, double margin, int num_samples = 200);
	glm::dvec2 closestPoint(const std::vector<glm::dvec2>& points, const glm::dvec2& p, double& dist, int num_samples = 200);

	glm::dvec2 reflect(const glm::dvec2& p, const glm::dvec2& a, const glm::dvec2& v);
	glm::dmat3x3 affineTransform(const glm::dvec2& p1, const glm::dvec2& p2, const glm::dvec2& q1, const glm::dvec2& q2);
	double crossProduct(const glm::dvec2& v1, const glm::dvec2& v2);
	glm::dmat3x3 calculateTransMatrix(const glm::dvec2& O, const glm::dvec2& X);

	double area(const std::vector<glm::dvec2>& points);
	bool withinPolygon(const std::vector<glm::dvec2>& points, const glm::dvec2& pt);
	bool withinPolygon(const std::vector<std::vector<glm::dvec2>>& polygons, const glm::dvec2& pt);
	BBox boundingBox(const std::vector<glm::dvec2>& points);

	std::vector<glm::dvec2> generateBarPolygon(const glm::dvec2& p1, const glm::dvec2& p2, float link_width);
	std::vector<glm::dvec2> generateRoundedBarPolygon(const glm::dvec2& p1, const glm::dvec2& p2, float link_radius, int num_slices = 36);
	std::vector<glm::dvec2> generateCirclePolygon(const glm::dvec2& p, float radius, int num_slices = 36);

	typedef std::vector<glm::dvec2> polygon;

}

BOOST_GEOMETRY_REGISTER_POINT_2D(glm::dvec2, double, boost::geometry::cs::cartesian, x, y)
BOOST_GEOMETRY_REGISTER_RING(kinematics::polygon)
