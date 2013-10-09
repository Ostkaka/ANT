#include <ant/graphicsSFML/SFMLScene.hpp>
#include <ant/graphicsSFML/SFMLSceneNode.hpp>
#include <ant/eventsystem/Events.hpp>
#include <ant/core_types.hpp>

using namespace ant;

ant::SFMLScene::SFMLScene( ISFMLRendererStrongPtr renderer )
{
	m_root.reset(GCC_NEW SFMLRootNode());
	m_renderer = renderer;

	IEventManager* pEventMgr = IEventManager::instance();
	pEventMgr->addListener(MakeDelegate(this, &SFMLScene::newRenderComponentDelegate), EvtData_New_SFMLRender_Component::sk_EventType);
	pEventMgr->addListener(MakeDelegate(this, &SFMLScene::destroyActorDelegate), EvtData_Destroy_Actor::sk_EventType);
	pEventMgr->addListener(MakeDelegate(this, &SFMLScene::moveActorDelegate), EvtData_Move_SFMLActor::sk_EventType);
	pEventMgr->addListener(MakeDelegate(this, &SFMLScene::modifiedRenderComponentDelegate), EvtData_Modified_SFMLRender_Component::sk_EventType);
}

ant::SFMLScene::~SFMLScene()
{
	IEventManager* pEventMgr = IEventManager::instance();
	pEventMgr->removeListener(MakeDelegate(this, &SFMLScene::newRenderComponentDelegate), EvtData_New_SFMLRender_Component::sk_EventType);
	pEventMgr->removeListener(MakeDelegate(this, &SFMLScene::destroyActorDelegate), EvtData_Destroy_Actor::sk_EventType);
	pEventMgr->removeListener(MakeDelegate(this, &SFMLScene::moveActorDelegate), EvtData_Move_SFMLActor::sk_EventType);
	pEventMgr->removeListener(MakeDelegate(this, &SFMLScene::modifiedRenderComponentDelegate), EvtData_Modified_SFMLRender_Component::sk_EventType);
}

HRESULT ant::SFMLScene::onRender()
{
	if (m_root && m_camera)
	{
		m_camera->setView(this);
	
		if (m_root->preRender(this) == S_OK)
		{
			m_root->render(this);
			m_root->renderChildren(this);
			m_root->postRender(this);
		}
	}
	return S_OK;
}

HRESULT ant::SFMLScene::onRestore()
{
	if (!m_root)
	{
		S_OK;
	}

	HRESULT hr;
	V_RETURN(m_renderer->onRestore());

	return m_root->onRestore(this);
}

HRESULT ant::SFMLScene::onLostDevice()
{
	if (m_root)
	{
		m_root->onLostDevice(this);
	}
	return S_OK;
}

HRESULT ant::SFMLScene::onUpdate( const ant::DeltaTime dt )
{
	if (!m_root)
	{
		return S_OK;
	}

	// Do some function to get time. This is just stupid with ticks
	return m_root->onUpdate(this, 0.016);
}

ant::ISFMLSceneNodeStrongPtr ant::SFMLScene::findActor( ActorId actor )
{
	SFMLSceneActorMap::iterator it = m_actorMap.find(actor);
	if (it==m_actorMap.end())
	{
		return ISFMLSceneNodeStrongPtr();
	}

	return it->second;
}

bool ant::SFMLScene::addChild( ActorId id, ISFMLSceneNodeStrongPtr kid )
{
	if (id != INVALID_ACTOR_ID)
	{
		m_actorMap[id] = kid;
	}

	return m_root->addChild(kid);
}

bool ant::SFMLScene::removeChild( ActorId id )
{
	if (id == INVALID_ACTOR_ID)
	{
		return false;
	}

	m_actorMap.erase(id);
	return m_root->removeChild(id);
}

void ant::SFMLScene::newRenderComponentDelegate( IEventDataStrongPtr eventData )
{
	shared_ptr<EvtData_New_SFMLRender_Component> pEventData = static_pointer_cast<EvtData_New_SFMLRender_Component>(pEventData);

	ActorId actorId = pEventData->getActorId();
	SFMLSceneNodeStrongPtr sceneNode(pEventData->getSceneNode());

	if (FAILED(sceneNode->onRestore(this)))
	{
		std::string error = "Failed to restore scene node to the scene for actorid " + ToStr(actorId);
		GCC_ERROR(error);
		return;
	}

	addChild(actorId,sceneNode);
}

void ant::SFMLScene::modifiedRenderComponentDelegate( IEventDataStrongPtr eventData )
{
	shared_ptr<EvtData_Modified_SFMLRender_Component> pCastEventData = static_pointer_cast<EvtData_Modified_SFMLRender_Component>(eventData);

	ActorId actorId = pCastEventData->getActorId();
	if (actorId == INVALID_ACTOR_ID)
	{
		GCC_ERROR("Scene::ModifiedRenderComponentDelegate - unknown actor id!");
		return;
	}

	//if (g_pApp->GetGameLogic()->GetState()==BGS_LoadingGameEnvironment)
	//	return;

	ISFMLSceneNodeStrongPtr pSceneNode = findActor(actorId);
	// FUTURE WORK: Add better error handling here.		
	if (!pSceneNode  || FAILED(pSceneNode->onRestore(this)))
	{
		GCC_ERROR("Failed to restore scene node to the scene for actorid " + ToStr(actorId));
	}
}

void ant::SFMLScene::destroyActorDelegate( IEventDataStrongPtr eventData )
{
	shared_ptr<EvtData_Destroy_Actor> pCastEventData = static_pointer_cast<EvtData_Destroy_Actor>(eventData);
	removeChild(pCastEventData->getId());
}

void ant::SFMLScene::moveActorDelegate( IEventDataStrongPtr eventData )
{
	shared_ptr<EvtData_Move_SFMLActor> pCastEventData = static_pointer_cast<EvtData_Move_SFMLActor>(eventData);

	ActorId id = pCastEventData->getId();

	sf::Vector2f pos = pCastEventData->getPosition();
	ant::Real rot = pCastEventData->getRotation();

	ISFMLSceneNodeStrongPtr pNode = findActor(id);
	if (pNode)
	{
		pNode->setPosition(pos);
		pNode->setRotation(rot);	
	}
}
