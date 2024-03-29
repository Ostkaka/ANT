#ifndef SFMLAPP_HPP_
	#define SFMLAPP_HPP_

#include <ant/ant_std.hpp>
#include <ant/core_types.hpp>
#include <ant/interfaces/ISFMLApp.hpp>

namespace ant
{
	/**
	 * Test app the is used to create an application class
	*/
	class SFMLApp : public ISFMLApp
	{
	public:
		    /**
			 * Load the game
			 */
			virtual void loadGameDelegate(IEventDataStrongPtr eventData);

	protected:
		 
			/**
			* Registers events for the game
			*/
			virtual void registerGameEvents(void) ANT_OVERRIDE;

			/**
			 * Registers events for the game
			 */
			virtual void registerGameDelegates(void);

			/**
			 * Initializes the game logic and views. 
			 */
			virtual BaseGameLogic* initGameLogicAndView(void) ANT_OVERRIDE;

	private:
		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////

	};
}

#endif