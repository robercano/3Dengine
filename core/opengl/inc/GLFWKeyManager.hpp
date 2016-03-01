/**
 * @class	GLFWKeyManager
 * @brief	GLFW based key manager
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <stdint.h>
#include <map>
#include <vector>
#include "KeyManager.hpp"

class GLFWKeyManager: public KeyManager
{
	public:
		/**
		 * Singleton pattern
		 */
		static GLFWKeyManager *GetKeyManager(void);

		/**
		 * Disposes the singleton
		 */
		static void DisposeKeyManager(void);

		/**
		 * Constructor of the class
		 */
		GLFWKeyManager(void);

		/**
		 * Destructor of the class
		 */
		~GLFWKeyManager(void);

		/**
		 * Registers a listener for the given keys
		 *
		 * @param	listener	Listener that will process the required keys
		 * @param	keys		List of keys IDs to listen on
		 *
		 * @return true if the listener was correctly registered, false otherwise
		 */
		bool registerListener(KeyListener &listener, std::vector<uint32_t> &keys);

	private:
		/**
		 * Static callback for key processing
		 *
		 * @param	key		Key being pressed or release
		 * @param	state	Either GLFW_PRESS or GLFW_RELEASE
		 */
		static void keyCallback(int key, int state);

		/**
		 * Object wrapper callback
		 *
		 * @param	key		Key being pressed
		 * @param	state	State of the key, pressed or released
		 */
		 void processKey(uint32_t key, bool state);

		/**
		 * Static GLFW key manager
		 */
		static GLFWKeyManager *_keyManager;

		/**
		 * Map for key listeners
		 */
		std::map<uint32_t, std::vector<KeyListener *> > _listeners;
};
