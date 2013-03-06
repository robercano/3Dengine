/**
 * @class	KeyManager
 * @brief	Interface for key management
 *
 * @author	Roberto Sosa Cano
 */
#ifndef __KEYMANAGER_HPP__
#define __KEYMANAGER_HPP__

#include <stdint.h>
#include <vector>


/**
 * Interface to be implemented by listeners of the key manager
 */
class KeyListener
{
	public:
		/**
		 * Method called when one of the registered keys is pressed/released
		 *
		 * @param	key		ID of the processed key
		 * @param	state	State of the key, pressed or released
		 */
		virtual void processKey(uint32_t key, bool state) = 0;
};

class KeyManager
{
	public:
		/**
		 * Registers a listener for the given keys
		 *
		 * @param	listener	Listener that will process the required keys
		 * @param	keys		List of keys IDs to listen on
		 *
		 * @return true if the listener was correctly registered, false otherwise
		 */
		virtual bool registerListener(KeyListener &listener, std::vector<uint32_t> &keys) = 0;
};

#endif
