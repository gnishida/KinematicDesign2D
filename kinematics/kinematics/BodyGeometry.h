#pragma once

#include <vector>
#include <boost/shared_ptr.hpp>
#include <glm/glm.hpp>
#include <QPainter>
#include <QMap>
#include "KinematicUtils.h"

namespace kinematics {

	class Joint;

	class BodyGeometry {
	public:
		boost::shared_ptr<Joint> pivot1;
		boost::shared_ptr<Joint> pivot2;
		Object2D polygons;
		QMap<int, bool> neighbors;

	public:
		BodyGeometry(boost::shared_ptr<Joint> pivot1, boost::shared_ptr<Joint> pivot2) : pivot1(pivot1), pivot2(pivot2) {}
		BodyGeometry(boost::shared_ptr<Joint> pivot1, boost::shared_ptr<Joint> pivot2, const Polygon2D& polygon);
		BodyGeometry(boost::shared_ptr<Joint> pivot1, boost::shared_ptr<Joint> pivot2, const Object2D& polygons);

		std::vector<std::vector<glm::dvec2>> getActualPoints();
		void draw(QPainter& painter, const QPointF& origin, float scale);

		glm::dmat3x2 getLocalToWorldModel();
		glm::dmat3x2 getWorldToLocalModel();
	};

}