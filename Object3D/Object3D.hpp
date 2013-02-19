/**
 * @class	Object3D
 * @brief	Basic object 3D representation
 *
 * @author	Roberto Sosa Cano
 */
#ifndef __OBJECT3D_HPP
#define __OBJECT3D_HPP

class Object3D
{
	public:
		/**
		 * Destructor
		 */
		virtual ~Object3D() {}

		/**
		 * Renders the object by using graphic API commands
		 *
		 * @returns	true if the object was rendered or false otherwise
		 */
		virtual bool render() = 0;
};

#endif
