#include "TestGameView.hpp"

ant::TestGameView::TestGameView( ISFMLRendererStrongPtr renderer )
	:SFMLHumanView(renderer)
{
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
	m_Camera->setCameraOffset(sf::Vector2f(400,200));
}

void ant::TestGameView::onAttach( GameViewId id, ActorId actorid ) 
{
	SFMLHumanView::onAttach(id,actorid);
}

LRESULT CALLBACK ant::TestGameView::onMsgProc( sf::Event theEvent ) 
{
	LRESULT result = 0;
	switch(theEvent.type)
	{	
	case sf::Event::KeyPressed:
		{
			if ( theEvent.key.code == sf::Keyboard::B )
			{				
				this->setControllerActor(2);
				result = true;
			}
			break;
		}
	default:
		break;
	}

	if (SFMLHumanView::onMsgProc(theEvent))
	{
		return 0;
	}		
	// Do nothing here yet
	return result;
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

void ant::TestGameView::registerAllDelegates( void )
{

}

void ant::TestGameView::removeAllDelegates( void )
{

}

