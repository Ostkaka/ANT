#include <ant/classes/BaseGameLogic.hpp>
#include <ant/actors/ActorFactory.hpp>
#include <ant/interfaces/IGameView.hpp>
#include <ant/eventsystem/Events.hpp>
#include <ant/resources/ResourceCacheManager.hpp>
#include <ant/resources/ResourceCache.hpp>
#include <ant/resources/XmlResource.hpp>
#include <ant/actors/Actor.hpp>
#include <ant/gccUtils/Math.hpp>
#include <ant/gccUtils/String.hpp>
#include <ant/eventsystem/Events.hpp>
#include <ant/classes/HumanView.hpp>

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

bool ant::BaseGameLogic::loadGame( const char* levelResource ) 
{
	// Grab the root node
	TiXmlElement* root = XmlResourceLoader::loadAndReturnXmlElement(levelResource);
	if (!root)
	{
		GCC_ERROR("Failed to find level resource file: " + std::string(levelResource));
		return false;
	}

	// Fix the pre and load scripts
	const char* preLoadScript = NULL;
	const char* postLoadScript = NULL;

	// pre and post load scripts from the level description
	TiXmlElement * scriptElement = root->FirstChildElement("Script");
	if (scriptElement)
	{
		preLoadScript = scriptElement->Attribute("preLoad");
		postLoadScript = scriptElement->Attribute("postLoad");
	}

	// Load the pre-load script if there is one
	if (preLoadScript)
	{
		Resource resource(preLoadScript);
		ResourceHandleStrongPtr pResourceHandle = ResourceCacheManager::instance()->getResourceCache()->getResourceHandle(&resource);
	}

	// load all initial actors
	TiXmlElement* pActorsNode = root->FirstChildElement("StaticActors");
	if (pActorsNode)
	{
		for (TiXmlElement* pNode = pActorsNode->FirstChildElement() ; pNode ; pNode = pNode->NextSiblingElement())
		{
			const char* actorResource = pNode->Attribute("resource");

			ActorStrongPtr pActor = createActor(actorResource, pNode);
			if (pActor)
			{
				shared_ptr<EvtData_New_Actor> pNewActorEvent(GCC_NEW EvtData_New_Actor(pActor->getId()));
				IEventManager::instance()->queueEvent(pNewActorEvent);
			}
		}
	}

	// init all game views
	for (auto it = m_gameViews.begin() ; it != m_gameViews.end() ; ++it)
	{
		shared_ptr<IGameView> pView = *it;
		if (pView->getType() == GameView_Human)
		{
			shared_ptr<SFMLHumanView> pHumanView = static_pointer_cast<SFMLHumanView, IGameView>(pView);
			pHumanView->loadGame(root);
		}
	}

	if (!loadGameDelegate(root))
	{
		return false;
	}

	if (postLoadScript)
	{
		Resource resource(postLoadScript);
		ResourceHandleStrongPtr pResourceHandle = ResourceCacheManager::instance()->getResourceCache()->getResourceHandle(&resource);
	}

	// trigger the Environment Loaded Game event - only then can player actors and AI be spawned!
	shared_ptr<EvtData_Environment_Loaded> pEvent(GCC_NEW EvtData_Environment_Loaded);
	IEventManager::instance()->triggerEvent(pEvent);

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
			// TODO Physics
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

ant::ActorStrongPtr ant::BaseGameLogic::createActor( const std::string &actorResource, TiXmlElement *overrides, const sf::Vector2f* initialPos/*=NULL*/,const ant::Real* initRot/*=NULL*/ , const ActorId serversActorId/*=INVALID_ACTOR_ID*/ )
{
	GCC_ASSERT(m_actorFactory);
	
	ActorStrongPtr pActor = m_actorFactory->createActor(actorResource.c_str(), overrides, initialPos, initRot , serversActorId);

	if (pActor)
	{
		m_actors.insert(std::make_pair(pActor->getId(), pActor));
		if (m_gameState==BGS_SPAWNINGPLAYERS || BGS_RUNNING)
		{
			// This should be filled when multi player is enabled. Send request new actor event here
		}
		return pActor;
	}
	else
	{
		GCC_WARNING("Could not create actor");
		return ActorStrongPtr();
	}
}

ant::ActorWeakPtr ant::BaseGameLogic::getActor( const ActorId id )
{
	ActorMap::iterator it = m_actors.find(id);
	if (it != m_actors.end())
	{
		return it->second;
	}
	return ActorWeakPtr();
}

void ant::BaseGameLogic::modifyActor( const ActorId id, TiXmlElement* overrides )
{
	GCC_ASSERT(m_actorFactory);
	if (!m_actorFactory)
	{
		return;
	}
	ActorMap::iterator it = m_actors.find(id);
	if (it != m_actors.end())
	{
		m_actorFactory->modifyActor(it->second, overrides);		
	}
}

void ant::BaseGameLogic::destroyActor( const ActorId id )
{
	// We need to trigger a synchronous event to ensure that any systems responding to this event can still access a 
	// valid actor if need be.  The actor will be destroyed after this.
	shared_ptr<EvtData_Destroy_Actor> pEvent(GCC_NEW EvtData_Destroy_Actor(id));
	IEventManager::instance()->triggerEvent(pEvent);

	ActorMap::iterator it = m_actors.find(id);
	if (it != m_actors.end())
	{
		it->second->destroy();
		m_actors.erase(it);
	}
}

std::string ant::BaseGameLogic::getActorXml( const ActorId id )
{
	ActorStrongPtr pActor = MakeStrongPtr(getActor(id));
	if (pActor)
	{
		return pActor->toXML();
	}
	else
	{
		GCC_ERROR("Couldn't find actor: " + ToStr(id));
	}

	return std::string();
}

ActorFactory* BaseGameLogic::createActorFactory(void)
{
	return GCC_NEW ActorFactory;
}