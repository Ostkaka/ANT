#include "TestGameView.hpp"
#include <ant/eventsystem/EventManager.hpp>
#include <ant/eventsystem/Events.hpp>
#include "TestEvents.hpp"
#include <ant/ant_std.hpp>
#include <ant/graphicsSFML/SFMLRenderer.hpp>

ant::TestGameView::TestGameView( ISFMLRendererStrongPtr renderer )
	:SFMLHumanView(renderer)
{
	m_zoomFactor = 1;
	registerAllDelegates();
}

ant::TestGameView::~TestGameView()
{
	removeAllDelegates();
}

void ant::TestGameView::setControllerActor( ActorId actorId )
{
	// Try to get actor from scene
	SFMLSceneNodeStrongPtr node = static_pointer_cast<SFMLSceneNode>(m_Scene->findActor(actorId));
	if (!node)
	{
		GCC_WARNING("Could not find the node for the actor");
	}

	SFMLHumanView::setControllerActor(actorId);

	m_TestController.reset(GCC_NEW TestController(node));
	m_KeyboardHandler = m_TestController;	
	m_Camera->setTarget(node);
}

void ant::TestGameView::onAttach( GameViewId id, ActorId actorid ) 
{
	SFMLHumanView::onAttach(id,actorid);
}

bool ant::TestGameView::onMsgProc( sf::Event theEvent ) 
{
	bool result = false;
	switch(theEvent.type)
	{	
	default:
		break;
	}

	if (SFMLHumanView::onMsgProc(theEvent))
	{
		return true;
	}		

	// Handle view specific stuff
	switch (theEvent.type)
	{
		case sf::Event::KeyPressed:
			handleKeyDown(theEvent.key.code);
			break;
		default:
			break;
	}

	// Do nothing here yet
	return false;
}

void ant::TestGameView::handleKeyDown(sf::Keyboard::Key key)
{
	if (key == sf::Keyboard::Key::N)
	{
		// Send event here perhaps? To reload level
		IEventDataStrongPtr pData(GCC_NEW EvtData_ReloadLevel());
		EventManager::instance()->queueEvent(pData);
	}	
	else if (key == sf::Keyboard::Key::P || key == sf::Keyboard::Key::L)
	{
		ant::Real dz = 0.01;
		m_zoomFactor += dz * ((sf::Keyboard::Key::P == key) ? 1 : -1);
		m_Camera->setCameraZoom(m_zoomFactor);
	}	
}

void ant::TestGameView::onUpdate( ant::DeltaTime dt ) 
{
	if (m_TestController)
	{
		m_TestController->onUpdate(dt);
	}
}

bool ant::TestGameView::loadGameDelegate( TiXmlElement* levelData ) 
{
	if (!SFMLHumanView::loadGameDelegate(levelData))
		return false;

	m_TestController.reset(GCC_NEW TestController(m_Camera));
	m_KeyboardHandler = m_TestController;

	m_Scene->onRestore();

	return true;
}

void ant::TestGameView::setControllerActorDelegate(IEventDataStrongPtr eventPtr)
{
	shared_ptr<EvtData_Set_Controlled_Actor> pCastEventData = static_pointer_cast<EvtData_Set_Controlled_Actor>(eventPtr);
	setControllerActor(pCastEventData->getActorId());
}

void ant::TestGameView::registerAllDelegates( void )
{
	IEventManager* pGlobalEventManager = IEventManager::instance();
	pGlobalEventManager->addListener(MakeDelegate(this, &TestGameView::setControllerActorDelegate), EvtData_Set_Controlled_Actor::sk_EventType);
}

void ant::TestGameView::removeAllDelegates( void )
{
	IEventManager* pGlobalEventManager = IEventManager::instance();
	pGlobalEventManager->removeListener(MakeDelegate(this, &TestGameView::setControllerActorDelegate), EvtData_Set_Controlled_Actor::sk_EventType);
}

void ant::TestGameView::renderText()
{
	SFMLHumanView::renderText();

	GCC_ASSERT(m_renderer->getTextRenderer());

	// Render the current time in the game
	std::string t = ToStr(m_currentTime);
	std::string timeString("Time: " + t + " s");	
	m_renderer->getTextRenderer()->renderText(timeString);
}

