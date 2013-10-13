#include "TestApp.hpp"
#include <ant/classes/BaseGameLogic.hpp>
#include "TestGameLogic.hpp"
#include "TestGameView.hpp"

using namespace ant;

void ant::SFMLApp::registerGameEvents( void )
{
	// Nothing to do here yet
}

BaseGameLogic* ant::SFMLApp::initGameLogicAndView( void ) 
{
	m_gameLogic = GCC_NEW TestGameLogic();
	m_gameLogic->init();
	
	GCC_ASSERT(m_renderer);
	shared_ptr<TestGameView> playaView(GCC_NEW TestGameView(m_renderer));

	m_gameLogic->addGameView(playaView);

	// Should add a game view to the logic?

	return m_gameLogic;
}


