#ifndef SFMLAPP_HPP_
	#define SFMLAPP_HPP_

#include <ant/core_types.hpp>
#include <ant/interfaces/ISFMLApp.hpp>

namespace ant
{
	/**
	 * Test app the is used to create an application class
	*/
	class SFMLApp : public ISFMLApp
	{
	protected:
		 
			/**
			 * Registers events for the game
			 */
			virtual void registerGameEvents(void) ANT_OVERRIDE;

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