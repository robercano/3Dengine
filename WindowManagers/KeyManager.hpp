/**
 * @class	KeyManager
 * @brief	Interface for key manager management
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
class KeyManagerListener
{
	public:
		/**
		 * Method called when one of the registered keys is pressed/released
		 *
		 * @param	key			ID of the processed key
		 * @param	modifiers	Flag with the current modifiers that apply to the key
		 */
		virtual void processKey(uint32_t key, uint32_t flags) = 0;
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
		virtual bool registerListener(KeyManagerListener &listener, std::vector<uint32_t> &keys) = 0;
};

#endif
