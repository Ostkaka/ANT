#include "TestApp.hpp"
#include <ant/classes/BaseGameLogic.hpp>
#include "TestGameLogic.hpp"
#include "TestGameView.hpp"
#include <ant/eventsystem/EventManager.hpp>
#include <ant/eventsystem/Events.hpp>
#include <ant/ant_std.hpp>
#include <ant/core_types.hpp>

using namespace ant;

void ant::SFMLApp::registerGameDelegates(void)
{
	// Nothing to do here yet
	IEventManager* pGlobalEventManager = IEventManager::instance();	
	pGlobalEventManager->addListener(MakeDelegate(this, &SFMLApp::loadGameDelegate), EvtData_ReloadLevel::sk_EventType);
}

void ant::SFMLApp::registerGameEvents(void)
{
	// Do nothing?	
	registerGameDelegates();
}

BaseGameLogic* ant::SFMLApp::initGameLogicAndView( void ) 
{
	m_gameLogic = GCC_NEW TestGameLogic();
	m_gameLogic->init();
	m_gameLogic->setActiveLevel(m_settings.m_level.c_str());
	
	GCC_ASSERT(m_renderer);
	shared_ptr<TestGameView> playaView(GCC_NEW TestGameView(m_renderer));

	m_gameLogic->addGameView(playaView);

	// Should add a game view to the logic?

	return m_gameLogic;
}

void ant::SFMLApp::loadGameDelegate(IEventDataStrongPtr eventData)
{	
	initGameLogicAndView();
}


