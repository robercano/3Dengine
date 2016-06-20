/**
 * @class	BoundingBox
 * @brief	Bounding box for 3D objects. It contains information about
 *          the BoundingBox that contains all object vertices. It can be used
 *          to represent an Axis-aligned bounding box or an Object-oriented
 *          bounding box
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <glm/glm.hpp>

class BoundingBox
{
  public:
    /**
     * Constructor
     */
	BoundingBox() :
		_min(glm::vec3(-1.0f, -1.0f, -1.0f)),
		_max(glm::vec3(1.0f, 1.0f, 1.0f)) {}
    BoundingBox(const glm::vec3 &min, const glm::vec3 &max) :
     _min(min),
	 _max(max) {}

	const glm::vec3 &getMin() { return _min; }
	const glm::vec3 &getMax() { return _max; }
	void setMin(const glm::vec3 &min) { _min = min; }
	void setMax(const glm::vec3 &max) { _max = max; }

  private:
	glm::vec3 _min; /**< Minimum values of the bounding box */
	glm::vec3 _max; /**< Maximum values of the bounding box */
};
