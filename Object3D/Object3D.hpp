/**
 * @class	Object3D
 * @brief	Basic object 3D representation
 *
 * @author	Roberto Sosa Cano
 */
#ifndef __OBJECT3D_HPP__
#define __OBJECT3D_HPP__

class Object3D
{
	public:
		/**
		 * Destructor
		 */
		virtual ~Object3D() {}

		/**
		 * Initialises the 3D object
		 *
		 * @returns	true if the object was initialised or false otherwise
		 */
		virtual bool init() = 0;

		/**
		 * Renders the object by using graphic API commands
		 *
		 * @returns	true if the object was rendered or false otherwise
		 */
		virtual bool render() = 0;

		/**
		 * Destroys the object by deinitilising it
		 *
		 * @returns	true if the object was destroyed or false otherwise
		 */
		virtual bool destroy() = 0;
};

#endif
