#pragma once

#include <QPainter>
#include <QPoint>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <boost/shared_ptr.hpp>
#include <QMap>
#include "Joint.h"
#include "Link.h"
#include "BodyGeometry.h"
#include "KinematicDiagram.h"

namespace kinematics {
	
	class Kinematics {
	public:
		KinematicDiagram diagram;
		KinematicDiagram diagram_original;
		double simulation_speed;
		double min_angle;
		double max_angle;

	public:
		Kinematics(double simulation_speed = 0.02);

		void clear();
		void load(const QString& filename);
		void save(const QString& filename);
		void initialize();
		void reset();
		void forwardKinematics(bool collision_check);
		void stepForward(bool collision_check, bool need_recovery_for_collision = true, bool motion_range_restricted = false);
		void stepBackward(bool collision_check, bool need_recovery_for_collision = true, bool motion_range_restricted = false);
		void draw(QPainter& painter, const QPointF& origin, float scale, bool show_linkage) const;
		void speedUp();
		void speedDown();
		void invertSpeed();
	};

}
