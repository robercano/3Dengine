/**
 * @class	Object3D
 * @brief	Meta-class that represents any object in the 3D
 *          world that has a position and orientation
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>

class Object3D
{
	public:
		Object3D() : _position(0.0f, 0.0f, 0.0f),
                     _orientation(glm::mat4(1.0f)),
					 _scale(glm::vec3(1.0)),
                     _model(glm::mat4(1.0f)),
                     _modelValid(true),
                     _viewValid(true) {}

		void setPosition(const glm::vec3 &position)
        {
            _position = position;
            _modelValid = false;
            _viewValid = false;
        }
		void setOrientation(const glm::mat4 &orientation)
        {
            _orientation = orientation;
            _modelValid = false;
            _viewValid = false;
        }
		void setScaleFactor(const glm::vec3 &factor)
		{
			_scale = factor;
			_modelValid = false;
			_viewValid = false;
		}
        void move(const glm::vec3 &amount)
        {
            _position += amount;
            _modelValid = false;
            _viewValid = false;
        }
        void rotate(const glm::mat4 &rotation)
        {
            _orientation = rotation * _orientation;
            _modelValid = false;
            _viewValid = false;
        }
		void scale(const glm::vec3 &factor)
		{
			_scale *= factor;
            _modelValid = false;
            _viewValid = false;
		}
		void lookAt(const glm::vec3 &at)
		{
			glm::vec3 up(0.0f, 1.0f, 0.0f);

			_view = glm::lookAt(_position, at, up);
            _modelValid = false;
            _viewValid = true;
		}
		const glm::vec3 &getPosition() { return _position; }
		const glm::mat4 &getOrientation() { return _orientation; }
		const glm::vec3 &getScaleFactor() { return _scale; }
		glm::vec3 getDirection()   { return glm::vec3(glm::column(_orientation, 2)); }

		const glm::mat4 &getModelMatrix(void)
        {
            if (_modelValid == false) {
				_model = glm::scale(glm::translate(glm::mat4(), _position) * _orientation, _scale);
                _modelValid = true;
            }
            return _model;
        }
		const glm::mat4 &getViewMatrix(void)
        {
            if (_viewValid == false) {
				_view = glm::translate(glm::scale(glm::mat4(), _scale) * _orientation, -_position);
                _viewValid = true;
            }
            return _view;
        }

	protected:
		glm::vec3 _position;
        glm::mat4 _orientation;
		glm::vec3 _scale;
		glm::mat4 _model;
		glm::mat4 _view;
		bool _modelValid;
		bool _viewValid;
};
