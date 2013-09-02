#include <ant/classes/HumanView.hpp>
#include <ant/classes/ProcessManager.hpp>
#include <ant/core_types.hpp>
#include <ant/gui/UserInterface.hpp>

ant::SFMLHumanView::SFMLHumanView( ISFMLRendererStrongPtr renderer )
{
	// Init Audio
	m_processManager = GCC_NEW ProcessManager;

	m_viewId = ant_InvalidGameViewId;

	registerAllDelegates();

	m_BaseGameState = BGS_INIT;

	if (renderer)
	{
		m_Scene.reset(GCC_NEW ScreenElementSFMLScene(renderer));
	}
	m_Camera.reset(GCC_NEW SFMLCameraNode(sf::Vector2f(),0));

	m_Scene->addChild(INVALID_ACTOR_ID, m_Camera);	
	m_Scene->setCamera(m_Camera);
}



