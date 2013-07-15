#include <ant/classes/BaseGameLogic.hpp>
#include <ant/actors/ActorFactory.hpp>
#include <ant/interfaces/IGameView.hpp>
#include <ant/actors/Actor.hpp>
#include <ant/gccUtils/Math.hpp>
#include <ant/eventsystem/Events.hpp>

using namespace ant;

//////////////////////////////////////////////////////////////////////////
//
// BaseGameLogic implementation
//
//////////////////////////////////////////////////////////////////////////

ant::BaseGameLogic::BaseGameLogic()
{
	m_lastActorId = 0;
	m_lifetime = 0;
	m_processManager = GCC_NEW ProcessManager;
	m_rng.Randomize();
	m_gameState = BGS_RUNNING;
	m_actorFactory = NULL;

	m_levelManager = GCC_NEW LevelManager;
	GCC_ASSERT(m_processManager && m_levelManager);
	//m_pLevelManager->Initialize(g_pApp->m_ResCache->Match("world\\*.xml")); // This is not good at all

	registerEngineScriptEvents();
}

ant::BaseGameLogic::~BaseGameLogic()
{
	// Remove game views
	while(!m_gameViews.empty())
		m_gameViews.pop_front();

	SAFE_DELETE(m_levelManager);
	SAFE_DELETE(m_processManager);
	SAFE_DELETE(m_actorFactory);

	// Destroy all actors
	for (auto it = m_actors.begin() ; it != m_actors.end() ; ++it)
	{
		it->second->destroy();
	}
	m_actors.clear();

	//THIS event should not really exist
	//IEventManager::instance()->removeListener(MakeDelegate(this,&BaseGameLogic::requestDestroyActorDelegate,EvtData_Request_Destroy_Actor::sk_EventType))
}

bool ant::BaseGameLogic::init( void )
{
	m_actorFactory = createActorFactory();

	return true;
}

void ant::BaseGameLogic::addGameView( IGameViewStrongPtr pView, ActorId actorid )
{
	int viewId = static_cast<int>(m_gameViews.size());
	m_gameViews.push_back(pView);
	pView->onAttach(viewId,actorid);
	pView->onRestore();
}

void ant::BaseGameLogic::removeGameView( IGameViewStrongPtr pView )
{
	m_gameViews.remove(pView);
}

bool ant::BaseGameLogic::loadGameDelegate( TiXmlElement* levelData )
{
	return true;
}

void ant::BaseGameLogic::onUpdate( ant::DeltaTime time, ant::DeltaTime elapsedTime )
{
	ant::DeltaTime dt = elapsedTime;
	m_lifetime += dt;

	switch (m_gameState)
	{
	case BGS_RUNNING:
		m_processManager->updateProcesses(dt);
		if (m_gamePhysics)
		{
			//m_gamePhysics->onUpdate(dt);
			//m_gamePhysics->synchVisibleScene();
		}
		break;

	default:
		GCC_ERROR("Unrecognised state");
	}

	// Update all game views
	for (GameViewList::iterator it = m_gameViews.begin() ; it != m_gameViews.end() ; ++it)
	{
		(*it)->onUpdate(dt);
	}

	// Update game actors
	for (ActorMap::const_iterator it = m_actors.begin() ; it != m_actors.end() ; ++it)
	{
		it->second->update(dt);
	}
}

void ant::BaseGameLogic::changeState( BaseGameState newState )
{
	// Unsure the exact functionality of this function
}

void ant::BaseGameLogic::toggleRenderDiagnostics()
{

}

void ant::BaseGameLogic::renderDiagnostics()
{
	if (m_renderDiagnostics)
	{
		//m_gamePhysics->renderDiagnostics(); Not implemented yet
	}
}

ActorFactory* BaseGameLogic::createActorFactory(void)
{
	return GCC_NEW ActorFactory;
}