#include <ant/graphicsSFML/SFMLSceneNode.hpp>
#include <ant/graphicsSFML/SFMLRenderComponent.hpp>
#include <ant/actors/TransformComponent.hpp>
#include <ant/interfaces/IRenderer.hpp>
#include <ant/resources/ResourceCacheManager.hpp>
#include <ant/resources/ResourceHandle.hpp>
#include <ant/resources/ResourceCache.hpp>
#include <ant/resources/Resource.hpp>
#include <ant/graphicsSFML/SFMLRenderer.hpp>
#include <ant/graphicsSFML/SFMLScene.hpp>

using namespace ant;

SFMLSceneNodeProperties::SFMLSceneNodeProperties( void )
{
	m_actorId = INVALID_ACTOR_ID;
	m_renderPass = RenderPass_0;
}

////////////////////////////////////////////////////
// SFMLSceneNode Implementation
////////////////////////////////////////////////////

ant::SFMLSceneNode::SFMLSceneNode( ActorId actorId, SFMLBaseRenderComponentWeakPtr renderComponent, SFMLRenderPass renderPass, const sf::Vector2f& pos, const ant::Real& rot )
{
	m_pParent = NULL;
	m_props.m_actorId = actorId;
	m_props.m_name = (renderComponent) ? renderComponent->getName() : "SceneNode";
	m_props.m_renderPass = renderPass;
	setPosition(pos);
	setRotation(rot);
}

ant::SFMLSceneNode::~SFMLSceneNode()
{

}

HRESULT ant::SFMLSceneNode::onUpdate( SFMLScene *scene, ant::DeltaTime dt )
{
	for (auto i=m_Children.begin() ; i != m_Children.end() ; ++i)
		(*i)->onUpdate(scene,dt);

	return S_OK;
}

HRESULT ant::SFMLSceneNode::onRestore( SFMLScene *scene )
{	
	for (auto i=m_Children.begin() ; i != m_Children.end() ; ++i)
		(*i)->onRestore(scene);

	return S_OK;
}

HRESULT ant::SFMLSceneNode::preRender( SFMLScene *scene )
{
	ant::ActorStrongPtr p = MakeStrongPtr(m_baseRenderComponent->getOwner());
	if (p)
	{
		TransformComponentStrongPtr pt = MakeStrongPtr(p->getComponent<TransformComponent>(TransformComponent::g_Name));
		if (pt)
		{
			setPosition(pt->getPostion());
			setRotation(pt->getRotation());
		}	
	}

	return S_OK;
}

HRESULT ant::SFMLSceneNode::render( SFMLScene *scene )
{
	return S_OK;
}

HRESULT ant::SFMLSceneNode::postRender( SFMLScene *scene )
{
	return S_OK;
}

// TODO add culling!!!
bool ant::SFMLSceneNode::isVisible( SFMLScene *scene ) const
{
	// We need to cull check here
	//SFMLCameraNodeStrongPtr camera = scene->getCamera();

	//sf::Vector2f camPos = camera->getPosition();

	// Hum, we need to get the view port of the view too
	/*	
	sf::View view;

	sf::Rect rect(;

	rect.contains(getPosition());
	*/

	// Is always visible for now
	return true;
}

HRESULT ant::SFMLSceneNode::onLostDevice( SFMLScene *scene )
{
	for (SceneNodeList::iterator i=m_Children.begin() ; i != m_Children.end() ; ++i)
		(*i)->onLostDevice(scene);

	return S_OK;
}

HRESULT ant::SFMLSceneNode::renderChildren( SFMLScene *scene )
{
	SFMLSceneNodeList::iterator i = m_Children.begin();
	SFMLSceneNodeList::iterator end = m_Children.end();

	while(i != end)
	{
		if ((*i)->preRender(scene)==S_OK)
		{
			if ((*i)->isVisible(scene))
			{
				(*i)->render(scene);

				(*i)->renderChildren(scene);
			}
		}
		(*i)->postRender(scene);
		++i;
	}

	return S_OK;
}

bool ant::SFMLSceneNode::addChild(ISFMLSceneNodeStrongPtr ikid)
{
	m_Children.push_back(ikid);

	SFMLSceneNodeStrongPtr kid = static_pointer_cast<SFMLSceneNode>(ikid);

	// Set the parent 
	kid->m_pParent = this;

	return true;
}

bool ant::SFMLSceneNode::removeChild(ActorId id)
{
	for (SceneNodeList::iterator i=m_Children.begin() ; i != m_Children.end() ; ++i)
	{
		const SFMLSceneNodeProperties* props = (*i)->getNodeProps();
		if (props->getActorId() != INVALID_ACTOR_ID && id == props->getActorId())
		{
			i = m_Children.erase(i);
			return true;
		}
	}
	return false;
}

////////////////////////////////////////////////////
// SFMLRootNode Implementation
////////////////////////////////////////////////////

ant::SFMLRootNode::SFMLRootNode()
	:SFMLSceneNode(INVALID_ACTOR_ID,NULL,RenderPass_0,sf::Vector2f(),ant::Real())
{
	m_Children.reserve(RenderPass_Last);

	shared_ptr<SFMLSceneNode> staticGroup(GCC_NEW SFMLSceneNode(INVALID_ACTOR_ID,  NULL,  RenderPass_Static, sf::Vector2f(),ant::Real(0)));
	m_Children.push_back(staticGroup); // RenderPass_Static = 0

	shared_ptr<SFMLSceneNode> actorGroup(GCC_NEW SFMLSceneNode(INVALID_ACTOR_ID,  NULL,  RenderPass_Actor, sf::Vector2f(),ant::Real(0)));
	m_Children.push_back(actorGroup);	// RenderPass_Actor = 1

	shared_ptr<SFMLSceneNode> skyGroup(GCC_NEW SFMLSceneNode(INVALID_ACTOR_ID,  NULL,  RenderPass_BackGround, sf::Vector2f(),ant::Real(0)));
	m_Children.push_back(skyGroup);	// RenderPass_Sky = 2

	shared_ptr<SFMLSceneNode> invisibleGroup(GCC_NEW SFMLSceneNode(INVALID_ACTOR_ID,  NULL,  RenderPass_NotRendered, sf::Vector2f(),ant::Real(0)));
	m_Children.push_back(invisibleGroup);	// RenderPass_NotRendered = 3
}

bool ant::SFMLRootNode::addChild( ISFMLSceneNodeStrongPtr kid )
{
	SFMLRenderPass pass = kid->getNodeProps()->renderPass();
	if (ant::UInt(pass) >= m_Children.size() || !m_Children[pass])
	{
		GCC_ASSERT(0 && _T("There is no such render pass"));
		return false;
	}
	
	return m_Children[pass]->addChild(kid);
}

HRESULT ant::SFMLRootNode::renderChildren( SFMLScene *scene )
{
	// This code creates fine control of the render passes.
	for (int pass = RenderPass_0; pass < RenderPass_Last; ++pass)
	{
		switch(pass)
		{
		case RenderPass_Static:
		case RenderPass_Actor:
			m_Children[pass]->renderChildren(scene);
			break;

		case RenderPass_BackGround:
			{
				// TODO - background pass renderer stuff
				//shared_ptr<IRenderState> skyPass = scene->get()->VPrepareSkyBoxPass();
				m_Children[pass]->renderChildren((scene));
				break;
			}
		}
	}

	return S_OK;
}

bool ant::SFMLRootNode::removeChild( ActorId id )
{
	bool removed = false;
	for (int i=RenderPass_0 ; i < RenderPass_Last; ++i)
	{
		if (m_Children[i]->removeChild(id))
		{
			removed=true;
		}
	}
	return removed;
}

////////////////////////////////////////////////////
// SFMLCameraNode Implementation
////////////////////////////////////////////////////

HRESULT ant::SFMLCameraNode::render(SFMLScene *scene)
{
	return S_OK;
}

HRESULT ant::SFMLCameraNode::onRestore(SFMLScene *scene)
{
	return S_OK;
}

HRESULT ant::SFMLCameraNode::setView( SFMLScene * pScene )
{
	if (m_target)
	{
		sf::Vector2f pos = m_target->getNodeProps()->getPosition();
		ant::Real rot = m_target->getNodeProps()->getRotation();

		setPosition(pos);
		setRotation(rot);
	}

	// TODO set position and such for renderer
	pScene->getRenderer()->setView(getPosition(), getRotation());
	return S_OK;
}

////////////////////////////////////////////////////
// SFMLSpriteNode Implementation
////////////////////////////////////////////////////
ant::SFMLSpriteNode::SFMLSpriteNode( ActorId actorId, 
	SFMLBaseRenderComponent* renderComponent, 
	const std::string& textureName,
	SFMLRenderPass renderPass, 
	const sf::Vector2f& pos, 
	const ant::Real& rot )
	:SFMLSceneNode(actorId,renderComponent,renderPass,pos,rot),
	m_textureName(textureName)
{
	// Now, get the buffer from the resource cache
	Resource r(m_textureName);
	ResourceHandleStrongPtr h = ResourceCacheManager::instance()->getResourceCache()->getResourceHandle(&r);
	
	if (m_texture.loadFromMemory(h->getBuffer(),h->getSize()))
	{		
		GCC_ERROR("sf::Texture could not load buffer from memory: " + m_textureName);
	}

	m_SFMLSprite.setTexture(m_texture);
}

HRESULT ant::SFMLSpriteNode::render( SFMLScene *scene ) 
{
	// First, set proper transformation
	m_SFMLSprite.setPosition(getPosition());
	m_SFMLSprite.setRotation(float(getRotation()));

	// Tell the renderer to draw the sprite
	return scene->getRenderer()->drawSprite(m_SFMLSprite);
}

HRESULT ant::SFMLSpriteNode::onRestore( SFMLScene *scene ) 
{
	return SFMLSceneNode::onRestore(scene);
	// TODO - do nothing until I get to know what this is
}
