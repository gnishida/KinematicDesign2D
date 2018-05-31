#include "KinematicDiagram.h"
#include "PinJoint.h"
#include "SliderHinge.h"
#include "Gear.h"
#include "KinematicUtils.h"
#include <QDomDocument>
#include <QFile>
#include <QTextStream>
#include <glm/gtc/matrix_transform.hpp>

namespace kinematics {

	Options* Options::instance = NULL;

	KinematicDiagram::KinematicDiagram() {
	}

	KinematicDiagram::~KinematicDiagram() {
	}

	KinematicDiagram KinematicDiagram::clone() const {
		KinematicDiagram copied_diagram;

		// copy joints
		for (int i = 0; i < joints.size(); ++i) {
			if (joints[i]->type == Joint::TYPE_PIN) {
				boost::shared_ptr<Joint> joint = boost::shared_ptr<Joint>(new PinJoint(joints[i]->id, joints[i]->ground, joints[i]->pos));
				joint->determined = joints[i]->determined;
				copied_diagram.addJoint(joint);
			}
			else if (joints[i]->type == Joint::TYPE_SLIDER_HINGE) {
				boost::shared_ptr<Joint> joint = boost::shared_ptr<Joint>(new SliderHinge(joints[i]->id, joints[i]->ground, joints[i]->pos));
				joint->determined = joints[i]->determined;
				copied_diagram.addJoint(joint);
			}
			else if (joints[i]->type == Joint::TYPE_GEAR) {
				boost::shared_ptr<Gear> gear = boost::static_pointer_cast<Gear>(joints[i]);
				boost::shared_ptr<Joint> joint = boost::shared_ptr<Joint>(new Gear(gear->id, gear->ground, gear->pos, gear->radius, gear->speed, gear->phase));
				joint->determined = joints[i]->determined;
				copied_diagram.addJoint(joint);
			}
		}

		// copy links
		for (int i = 0; i < links.size(); ++i) {
			std::vector<boost::shared_ptr<Joint>> copied_joints;
			for (int j = 0; j < links[i]->joints.size(); ++j) {
				copied_joints.push_back(copied_diagram.joints[links[i]->joints[j]->id]);
			}

			copied_diagram.addLink(links[i]->driver, copied_joints, links[i]->actual_link);
		}

		// copy the original shape of the links
		for (int i = 0; i < links.size(); ++i) {
			copied_diagram.links[i]->original_shape = links[i]->original_shape;
			copied_diagram.links[i]->angle = links[i]->angle;
		}

		// copy bodis
		for (int i = 0; i < bodies.size(); ++i) {
			int id1 = bodies[i]->pivot1->id;
			int id2 = bodies[i]->pivot2->id;
			boost::shared_ptr<BodyGeometry> body = boost::shared_ptr<BodyGeometry>(new BodyGeometry(copied_diagram.joints[id1], copied_diagram.joints[id2], bodies[i]->polygons));
			/*
			for (int j = 0; j < bodies[i]->polygon.points.size(); ++j) {
			body->polygon.points.push_back(bodies[i]->polygon.points[j]);
			}
			*/

			for (auto it = bodies[i]->neighbors.begin(); it != bodies[i]->neighbors.end(); ++it) {
				body->neighbors[it.key()] = it.value();
			}

			copied_diagram.bodies.push_back(body);
		}

		return copied_diagram;
	}

	void KinematicDiagram::clear() {
		joints.clear();
		links.clear();
		bodies.clear();
	}

	void KinematicDiagram::initialize() {
		// save the initial shape
		// this information is used to obtain the length between joints
		for (int i = 0; i < links.size(); ++i) {
			for (int j = 0; j < links[i]->joints.size(); ++j) {
				links[i]->original_shape[links[i]->joints[j]->id] = links[i]->joints[j]->pos;
			}
			if (links[i]->joints.size() >= 2) {
				links[i]->angle = atan2(links[i]->joints[1]->pos.y - links[i]->joints[0]->pos.y, links[i]->joints[1]->pos.x - links[i]->joints[0]->pos.x);
			}
		}

		updateBodyAdjacency();
	}

	void KinematicDiagram::addJoint(boost::shared_ptr<Joint> joint) {
		int id = joint->id;

		// if id == -1, automatically set the appropriate id
		if (id == -1) {
			id = 0;
			if (!joints.empty()) {
				id = joints.lastKey() + 1;
			}
			joint->id = id;
		}

		joints[id] = joint;
	}

	void KinematicDiagram::setJointToLink(boost::shared_ptr<Joint> joint, boost::shared_ptr<Link> link) {
		link->addJoint(joint);
		joint->links.push_back(link);
	}

	boost::shared_ptr<Link> KinematicDiagram::newLink() {
		return newLink(false);
	}

	boost::shared_ptr<Link> KinematicDiagram::newLink(bool driver, bool actual_link) {
		int id = 0;
		if (!links.empty()) {
			id = links.lastKey() + 1;
		}

		boost::shared_ptr<Link> link = boost::shared_ptr<Link>(new Link(id, driver, actual_link));
		links[id] = link;
		return link;
	}

	boost::shared_ptr<Link> KinematicDiagram::addLink(boost::shared_ptr<Joint> joint1, boost::shared_ptr<Joint> joint2, bool actual_link) {
		return addLink(false, joint1, joint2, actual_link);
	}

	boost::shared_ptr<Link> KinematicDiagram::addLink(bool driver, boost::shared_ptr<Joint> joint1, boost::shared_ptr<Joint> joint2, bool actual_link) {
		int id = 0;
		if (!links.empty()) {
			id = links.lastKey() + 1;
		}

		boost::shared_ptr<Link> link = boost::shared_ptr<Link>(new Link(id, driver, actual_link));
		links[id] = link;

		link->addJoint(joint1);
		link->addJoint(joint2);
		joint1->links.push_back(link);
		joint2->links.push_back(link);

		return link;
	}

	boost::shared_ptr<Link> KinematicDiagram::addLink(std::vector<boost::shared_ptr<Joint>> joints, bool actual_link) {
		return addLink(false, joints, actual_link);
	}

	boost::shared_ptr<Link> KinematicDiagram::addLink(bool driver, std::vector<boost::shared_ptr<Joint>> joints, bool actual_link) {
		int id = 0;
		if (!links.empty()) {
			id = links.lastKey() + 1;
		}

		boost::shared_ptr<Link> link = boost::shared_ptr<Link>(new Link(id, driver, actual_link));
		links[id] = link;

		for (int i = 0; i < joints.size(); ++i) {
			link->addJoint(joints[i]);
			joints[i]->links.push_back(link);
		}

		return link;
	}

	void KinematicDiagram::addPolygonToBody(int body_id, const Polygon25D& polygon) {
		bodies[body_id]->polygons.push_back(polygon);

		// setup rotation matrix
		glm::vec2 dir = bodies[body_id]->pivot2->pos - bodies[body_id]->pivot1->pos;
		double angle = atan2(dir.y, dir.x);

		glm::dvec2 p1 = bodies[body_id]->pivot1->pos;
		glm::dmat4x4 model;
		model = glm::rotate(model, -angle, glm::dvec3(0, 0, 1));

		for (int i = 0; i < polygon.points.size(); i++) {
			// convert the coordinates to the local coordinate system
			glm::dvec2 rotated_p = glm::dvec2(model * glm::dvec4(polygon.points[i].x - p1.x, polygon.points[i].y - p1.y, 0, 1));

			bodies[body_id]->polygons.back().points[i] = rotated_p;
		}
	}

	void KinematicDiagram::addBody(boost::shared_ptr<Joint> joint1, boost::shared_ptr<Joint> joint2, const Object25D& polygons) {
		boost::shared_ptr<BodyGeometry> body = boost::shared_ptr<BodyGeometry>(new BodyGeometry(joint1, joint2, polygons));

		// get the world to local matrix
		glm::dmat3x2 model = body->getWorldToLocalModel();

		for (int i = 0; i < polygons.size(); i++) {
			for (int j = 0; j < polygons[i].points.size(); ++j) {
				// convert the coordinates to the local coordinate system
				body->polygons[i].points[j] = model * glm::dvec3(polygons[i].points[j], 1);
			}
		}

		bodies.push_back(body);

	}

	/**
	* Given the current mechanism, already added moving bodies, and input fixed bodies,
	* this function updates the fixed bodies and moving bodies such that
	* all the joints are properly connected to some rigid bodies.
	*/
	void KinematicDiagram::connectJointsToBodies(std::vector<Object25D>& fixed_bodies, std::vector<glm::dvec2>& connected_pts) {
		int N = fixed_bodies.size();

		// connect fixed joints to a fixed body
		int cnt = 0;
		for (int j = 0; j < joints.size(); j++) {
			if (joints[j]->ground) {
				glm::dvec2 connected_pt = connectFixedJointToBody(joints[j], fixed_bodies);
				connected_pts.push_back(connected_pt);
			}
		}

		// connect moving joints to a moving body
		cnt = 0;
		for (int j = 0; j < bodies.size(); j++) {
			if (bodies[j]->pivot1->ground || bodies[j]->pivot2->ground) continue;

			std::vector<glm::dvec2> body_pts = bodies[j]->getActualPoints()[0];
			glm::dvec2 connected_pt1 = connectMovingJointToBody(bodies[j]->pivot1, j, body_pts);
			connected_pts.push_back(connected_pt1);
			glm::dvec2 connected_pt2 = connectMovingJointToBody(bodies[j]->pivot2, j, body_pts);
			connected_pts.push_back(connected_pt2);
		}
	}

	/**
	* Connect fixed joint to a body by adding a connector
	* Joint has a information of z order that indicates the z order of the connected link.
	* Based on the z orders of the joint and joint connector, the direction of the joint, upward or downward, can be determined.
	*
	* @param joint	joint
	* @param z		z order of the joint connector
	*/
	glm::dvec2 KinematicDiagram::connectFixedJointToBody(boost::shared_ptr<kinematics::Joint> joint, std::vector<Object25D>& fixed_bodies) {
		glm::dvec2 closest_point;
		int fixed_body_id = -1;

		// check if the joint is within the rigid body
		bool is_inside = false;
		for (int k = 0; k < fixed_bodies.size(); k++) {
			if (kinematics::withinPolygon(fixed_bodies[k].polygons[0].points, joint->pos)) {
				is_inside = true;
				fixed_body_id = k;
				break;
			}
		}

		std::vector<glm::dvec2> pts;

		if (is_inside) {
			closest_point = joint->pos;

			pts = generateCirclePolygon(joint->pos, options->link_width / 2);
			fixed_bodies[fixed_body_id].push_back(kinematics::Polygon25D(pts));
			fixed_bodies[fixed_body_id].push_back(kinematics::Polygon25D(pts));
		}
		else {
			// find the closest point of a rigid body
			double min_dist = std::numeric_limits<double>::max();
			for (int k = 0; k < fixed_bodies.size(); k++) {
				glm::dvec2 cp;
				try {
					cp = kinematics::closestOffsetPoint(fixed_bodies[k].polygons[0].points, joint->pos, options->body_margin);
				}
				catch (char* ex) {
					double dist;
					cp = kinematics::closestPoint(fixed_bodies[k].polygons[0].points, joint->pos, dist);
				}

				double dist = glm::length(cp - joint->pos);
				if (dist < min_dist) {
					min_dist = dist;
					closest_point = cp;
					fixed_body_id = k;
				}
			}

			// Create the base of the connecting part
			pts = generateCirclePolygon(closest_point, options->link_width / 2);
			fixed_bodies[fixed_body_id].push_back(kinematics::Polygon25D(pts));
			fixed_bodies[fixed_body_id].push_back(kinematics::Polygon25D(pts));

			// Create a link-like geometry to extend the body to the joint
			pts = generateRoundedBarPolygon(closest_point, joint->pos, options->link_width / 2);
			fixed_bodies[fixed_body_id].push_back(kinematics::Polygon25D(pts));
			fixed_bodies[fixed_body_id].push_back(kinematics::Polygon25D(pts));
		}

		return closest_point;
	}

	glm::dvec2 KinematicDiagram::connectMovingJointToBody(boost::shared_ptr<Joint> joint, int body_id, const std::vector<glm::dvec2>& moving_body) {
		glm::dvec2 closest_point;
		std::vector<glm::dvec2> pts;

		if (kinematics::withinPolygon(moving_body, joint->pos)) {
			closest_point = joint->pos;

			pts = generateCirclePolygon(joint->pos, options->link_width / 2);
			addPolygonToBody(body_id, kinematics::Polygon25D(pts));
			addPolygonToBody(body_id, kinematics::Polygon25D(pts));
		}
		else {
			// find the closest point of a rigid body
			try {
				closest_point = kinematics::closestOffsetPoint(moving_body, joint->pos, options->body_margin);
			}
			catch (char* ex) {
				double dist;
				closest_point = kinematics::closestPoint(moving_body, joint->pos, dist);
			}

			// Create the base of the connecting part
			pts = generateCirclePolygon(closest_point, options->link_width / 2);
			addPolygonToBody(body_id, kinematics::Polygon25D(pts));
			addPolygonToBody(body_id, kinematics::Polygon25D(pts));

			// create a geometry to extend the body to the joint
			pts = generateRoundedBarPolygon(closest_point, joint->pos, options->link_width / 2);
			addPolygonToBody(body_id, kinematics::Polygon25D(pts));
			addPolygonToBody(body_id, kinematics::Polygon25D(pts));
		}

		return closest_point;
	}

	void KinematicDiagram::load(const QString& filename) {
		QFile file(filename);
		if (!file.open(QFile::ReadOnly | QFile::Text)) throw "File cannot open.";

		QDomDocument doc;
		doc.setContent(&file);

		QDomElement root = doc.documentElement();
		if (root.tagName() != "design")	throw "Invalid file format.";

		// clear the data
		clear();

		QDomNode node = root.firstChild();
		while (!node.isNull()) {
			if (node.toElement().tagName() == "joints") {
				QDomNode joint_node = node.firstChild();
				while (!joint_node.isNull()) {
					if (joint_node.toElement().tagName() == "joint") {
						// add a joint
						int id = joint_node.toElement().attribute("id").toInt();
						if (joint_node.toElement().attribute("type") == "pin") {
							addJoint(boost::shared_ptr<Joint>(new PinJoint(joint_node.toElement())));
						}
						else if (joint_node.toElement().attribute("type") == "slider_hinge") {
							addJoint(boost::shared_ptr<Joint>(new SliderHinge(joint_node.toElement())));
						}
						else if (joint_node.toElement().attribute("type") == "gear") {
							addJoint(boost::shared_ptr<Joint>(new Gear(joint_node.toElement())));
						}
					}

					joint_node = joint_node.nextSibling();
				}
			}
			else if (node.toElement().tagName() == "links") {
				QDomNode link_node = node.firstChild();
				while (!link_node.isNull()) {
					if (link_node.toElement().tagName() == "link") {
						// add a link
						bool driver = link_node.toElement().attribute("driver").toLower() == "true" ? true : false;
						std::vector<boost::shared_ptr<Joint>> jts;
						QStringList joint_list = link_node.toElement().attribute("joints").split(",");
						for (int i = 0; i < joint_list.size(); ++i) {
							jts.push_back(joints[joint_list[i].toInt()]);
						}
						addLink(driver, jts);
					}

					link_node = link_node.nextSibling();
				}
			}
			else if (node.toElement().tagName() == "bodies") {
				QDomNode body_node = node.firstChild();
				while (!body_node.isNull()) {
					if (body_node.toElement().tagName() == "body") {
						// add a body
						int id1 = body_node.toElement().attribute("id1").toInt();
						int id2 = body_node.toElement().attribute("id2").toInt();

						std::vector<glm::dvec2> points;
						QDomNode point_node = body_node.firstChild();
						while (!point_node.isNull()) {
							if (point_node.toElement().tagName() == "point") {
								double x = point_node.toElement().attribute("x").toDouble();
								double y = point_node.toElement().attribute("y").toDouble();
								points.push_back(glm::dvec2(x, y));
							}

							point_node = point_node.nextSibling();
						}

						addBody(joints[id1], joints[id2], points);
					}

					body_node = body_node.nextSibling();
				}
			}

			node = node.nextSibling();
		}

		// initialize the adancency between rigid bodies
		initialize();
	}

	void KinematicDiagram::save(const QString& filename) {
		/*
		QFile file(filename);
		if (!file.open(QFile::WriteOnly)) throw "File cannot open.";

		QDomDocument doc;

		// set root node
		QDomElement root = doc.createElement("design");
		root.setAttribute("author", "Gen Nishida");
		root.setAttribute("version", "1.0");
		root.setAttribute("date", QDate::currentDate().toString("MM/dd/yyyy"));
		doc.appendChild(root);

		// write points
		QDomElement points_node = doc.createElement("points");
		root.appendChild(points_node);
		for (auto it = joints.begin(); it != joints.end(); ++it) {
		QDomElement point_node = doc.createElement("point");
		point_node.setAttribute("id", it.key());
		point_node.setAttribute("x", it.value()->pos.x);
		point_node.setAttribute("y", it.value()->pos.y);
		points_node.appendChild(point_node);
		}

		// write links
		QDomElement links_node = doc.createElement("links");
		root.appendChild(links_node);
		for (auto it = joints.begin(); it != joints.end(); ++it) {
		for (int j = 0; j < it.value()->in_links.size(); ++j) {
		QDomElement link_node = doc.createElement("link");
		link_node.setAttribute("order", j);
		link_node.setAttribute("start", it.value()->in_links[j]->start);
		link_node.setAttribute("end", it.value()->in_links[j]->end);
		links_node.appendChild(link_node);
		}
		}

		// write bodies
		QDomElement bodies_node = doc.createElement("bodies");
		root.appendChild(bodies_node);
		for (int i = 0; i < bodies.size(); ++i) {
		QDomElement body_node = doc.createElement("body");
		body_node.setAttribute("id1", bodies[i].pivot1);
		body_node.setAttribute("id2", bodies[i].pivot2);
		bodies_node.appendChild(body_node);

		// setup rotation matrix
		glm::dvec2 dir = joints[bodies[i].pivot2]->pos - joints[bodies[i].pivot1]->pos;
		double angle = atan2(dir.y, dir.x);
		glm::dvec2 p1 = (joints[bodies[i].pivot1]->pos + joints[bodies[i].pivot2]->pos) * 0.5;
		glm::dmat4x4 model;
		model = glm::rotate(model, angle, glm::dvec3(0, 0, 1));

		for (int k = 0; k < bodies[i].points.size(); ++k) {
		// convert the coordinates to the local coordinate system
		glm::dvec2 rotated_p = glm::dvec2(model * glm::vec4(bodies[i].points[k].x, bodies[i].points[k].y, 0, 1)) + p1;
		QDomElement point_node = doc.createElement("point");
		point_node.setAttribute("x", rotated_p.x);
		point_node.setAttribute("y", rotated_p.y);
		body_node.appendChild(point_node);
		}
		}

		QTextStream out(&file);
		doc.save(out, 4);
		*/
	}

	void KinematicDiagram::updateBodyAdjacency() {
		// clear the neighbors
		for (int i = 0; i < bodies.size(); ++i) {
			bodies[i]->neighbors.clear();
		}

		// Check the adjacency
		// The current implementation is not elegant.
		// Adjacent bodies are considered to belong to the same group of a fixed body,
		// and we do not check the collision between them.
		for (int i = 0; i < bodies.size(); ++i) {
			for (int j = i + 1; j < bodies.size(); ++j) {
				if (polygonPolygonIntersection(bodies[i]->getActualPoints()[0], bodies[j]->getActualPoints()[0])) {
					bodies[i]->neighbors[j] = true;
					bodies[j]->neighbors[i] = true;
				}
			}
		}
	}

	bool KinematicDiagram::isCollided(bool main_body_only) const {
		// check the collision between rigid bodies
		for (int i = 0; i < bodies.size(); ++i) {
			for (int j = i + 1; j < bodies.size(); ++j) {
				// skip the neighbors
				if (bodies[i]->neighbors.contains(j)) continue;

				for (int k = 0; k < bodies[i]->polygons.size(); k++) {
					if (main_body_only && k > 0) continue;

					if (!bodies[i]->polygons[k].check_collision) continue;
					std::vector<glm::dvec2> pts1 = bodies[i]->getActualPoints(k);

					for (int l = 0; l < bodies[j]->polygons.size(); l++) {
						if (main_body_only && l > 0) continue;

						if (!bodies[j]->polygons[l].check_collision) continue;
						std::vector<glm::dvec2> pts2 = bodies[j]->getActualPoints(l);

						if (polygonPolygonIntersection(pts1, pts2)) return true;
					}
				}
			}
		}

		return false;
	}

	void KinematicDiagram::draw(QPainter& painter, const QPointF& origin, float scale, bool show_linkage) const {
		for (int i = 0; i < bodies.size(); ++i) {
			bodies[i]->draw(painter, origin, scale, show_linkage);
		}

		if (show_linkage) {
			// draw links
			for (int i = 0; i < links.size(); ++i) {
				links[i]->draw(painter, origin, scale);
			}

			// draw joints
			for (auto it = joints.begin(); it != joints.end(); ++it) {
				joints[it.key()]->draw(painter, origin, scale);
			}
		}
	}

}