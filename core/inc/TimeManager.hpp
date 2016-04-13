/**
 * @class	TimeManager
 * @brief	Interface for time management
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <string>
#include <stdint.h>

class TimeManager
{
	public:
		/**
		 * Time Manager factory
		 *
		 * @return Pointer to a the requested time manager or NULL
		 */
		static TimeManager *GetInstance(void);

		/**
		 * Time Manager disposal
		 */
		static void DisposeInstance();

        virtual ~TimeManager() {}

		/**
		 * Retrieves the current elapsed time in milliseconds
		 *
		 * @return The elapsed time in milliseconds
		 */
		virtual double getElapsedMs() = 0;

	private:
		/**
		 * Current time manager
		 */
		static TimeManager *_timeManager;
};
