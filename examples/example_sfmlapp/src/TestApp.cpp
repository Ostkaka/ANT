#include "TestApp.hpp"
#include <ant/classes/BaseGameLogic.hpp>
#include "TestGameLogic.hpp"

using namespace ant;

void ant::SFMLApp::registerGameEvents( void )
{
	// Nothing to do here yet
}

BaseGameLogic* ant::SFMLApp::initGameLogicAndView( void ) 
{
	m_gameLogic = GCC_NEW TestGameLogic();
	m_gameLogic->init();

	//shared_ptr<IGameView> menuView(GCC_NEW MainMenuView());
	//m_pGame->VAddView(menuView);

	return m_gameLogic;
}


