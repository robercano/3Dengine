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
		void normalize()
		{
			/* TODO */
		}

		const glm::vec3 &getPosition() { return _position; }
		const glm::mat4 &getOrientation() { return _orientation; }
		const glm::vec3 &getScaleFactor() { return _scale; }

		const glm::mat4 &getModelMatrix(void)
        {
            if (_modelValid == false) {
				glm::mat4 tmp = glm::scale(glm::mat4(), _scale);
				tmp = _orientation * tmp;
                _model = glm::translate(tmp, _position);
                _modelValid = true;
            }
            return _model;
        }
		virtual const glm::mat4 &getViewMatrix(void)
        {
            if (_viewValid == false) {
                glm::mat4 translation = glm::translate(glm::mat4(), -_position);
                _view = glm::scale( _orientation * translation, _scale );
                _view = _orientation * translation;
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
