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
#include <ant/gccUtils/templates.hpp>
#include <iostream>
#include <ant\resources\XmlResource.hpp>

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
	m_baseRenderComponent=renderComponent;
	setPosition(pos);
	setRotation(rot);
	setDirection(sf::Vector2f(1,0));
}

ant::SFMLSceneNode::~SFMLSceneNode()
{

}

bool ant::SFMLSceneNode::onUpdate( SFMLScene *scene, ant::DeltaTime dt )
{
	for (auto i=m_Children.begin() ; i != m_Children.end() ; ++i)
		(*i)->onUpdate(scene,dt);

	return true;
}

bool ant::SFMLSceneNode::onRestore( SFMLScene *scene )
{	
	for (auto i=m_Children.begin() ; i != m_Children.end() ; ++i)
		(*i)->onRestore(scene);

	return true;
}

bool ant::SFMLSceneNode::preRender( SFMLScene *scene )
{
	// What about the move character delegate in the scene?
	if (m_baseRenderComponent)
	{
		ant::ActorStrongPtr p = MakeStrongPtr(m_baseRenderComponent->getOwner());
		if (p)
		{
			TransformComponentStrongPtr pt = MakeStrongPtr(p->getComponent<TransformComponent>(TransformComponent::g_Name));
			if (pt)
			{
				setPosition(pt->getPostion());
				setRotation(pt->getRotation());
				setDirection(pt->getDirection());
			}	
		}
	}
	return true;
}

bool ant::SFMLSceneNode::render( SFMLScene *scene )
{
	return true;
}

bool ant::SFMLSceneNode::postRender( SFMLScene *scene )
{
	return true;
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

bool ant::SFMLSceneNode::onLostDevice( SFMLScene *scene )
{
	for (SceneNodeList::iterator i=m_Children.begin() ; i != m_Children.end() ; ++i)
		(*i)->onLostDevice(scene);

	return true;
}

bool ant::SFMLSceneNode::renderChildren( SFMLScene *scene )
{
	SFMLSceneNodeList::iterator i = m_Children.begin();
	SFMLSceneNodeList::iterator end = m_Children.end();

	while(i != end)
	{
		if ((*i)->preRender(scene)==true)
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

	return true;
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

	shared_ptr<SFMLSceneNode> skyGroup(GCC_NEW SFMLSceneNode(INVALID_ACTOR_ID,  NULL,  RenderPass_BackGround, sf::Vector2f(),ant::Real(0)));
	m_Children.push_back(skyGroup);	// RenderPass_Sky = 2

	shared_ptr<SFMLSceneNode> actorGroup(GCC_NEW SFMLSceneNode(INVALID_ACTOR_ID,  NULL,  RenderPass_Actor, sf::Vector2f(),ant::Real(0)));
	m_Children.push_back(actorGroup);	// RenderPass_Actor = 1

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

bool ant::SFMLRootNode::renderChildren( SFMLScene *scene )
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

	return true;
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

bool ant::SFMLCameraNode::render(SFMLScene *scene)
{
	return true;
}

bool ant::SFMLCameraNode::onRestore(SFMLScene *scene)
{
	return true;
}

bool ant::SFMLCameraNode::setView( SFMLScene * pScene )
{
	if (m_target)
	{
		sf::Vector2f pos = m_target->getNodeProps()->getPosition();
		//ant::Real rot = m_target->getNodeProps()->getRotation();

		// Add the camera offset also
		pos += m_cameraOffset;

		setPosition(pos);
		//setRotation(rot);
	}
	// Get the offset to the middle
	sf::Vector2f center = pScene->getRenderer()->getView().getCenter();

	// Set view to scene
	pScene->getRenderer()->setView(getPosition(), getRotation(), m_zoomFactor);
	return true;
}

////////////////////////////////////////////////////
// SFMLSpriteNode Implementation
////////////////////////////////////////////////////
ant::SFMLSpriteNode::SFMLSpriteNode( ActorId actorId, 
	SFMLBaseRenderComponent* renderComponent, 
	const std::string& textureName,
	const sf::IntRect& textureRect,
	const ant::Real& scale,
	SFMLRenderPass renderPass, 
	const sf::Vector2f& pos, 
	const ant::Real& rot )
	:SFMLSceneNode(actorId,renderComponent,renderPass,pos,rot),
	m_textureName(textureName),
	m_scale(scale)	
{	
	// Now, get the buffer from the resource cache
	Resource r(m_textureName);
	ResourceHandleStrongPtr handle = ResourceCacheManager::instance()->getResourceCache()->getResourceHandle(&r);
	
	// Is the buffer OK?
	if (!handle->getBuffer())
	{
		GCC_ERROR("Buffer from texture resource is bad");
	}

	if (!m_texture.loadFromMemory(handle->getBuffer(),handle->getSize()))
	{		
		GCC_ERROR("sf::Texture could not load buffer from memory: " + m_textureName);
	}

	m_SFMLSprite.setTexture(m_texture);
	if (textureRect.width != 0 && textureRect.height != 0)
	{
		m_SFMLSprite.setTextureRect(textureRect);
	}	
	m_SFMLSprite.setOrigin(m_SFMLSprite.getLocalBounds().width/2,m_SFMLSprite.getLocalBounds().height/2);
}

bool ant::SFMLSpriteNode::render( SFMLScene *scene ) 
{	
	// First, set proper transformation
	m_SFMLSprite.setPosition(getPosition());
	m_SFMLSprite.setRotation(float(getRotation()));	
	
	// Flip the sprite depending on direction in X-axis
	int w = m_SFMLSprite.getTextureRect().width;
	int h = m_SFMLSprite.getTextureRect().height;

	
	if (getDirection().x == -1)
	{
		m_SFMLSprite.setScale(-1.0 * (float)m_scale, 1.0* (float)m_scale);
	}
	else if (getDirection().x == 1)
	{
		m_SFMLSprite.setScale(1 * (float)m_scale, 1 * (float)m_scale);
	}
	
	// Tell the renderer to draw the sprite
	return scene->getRenderer()->drawSprite(m_SFMLSprite);
}

bool ant::SFMLSpriteNode::onRestore( SFMLScene *scene ) 
{
	return SFMLSceneNode::onRestore(scene);
	// TODO - do nothing until I get to know what this is
}

////////////////////////////////////////////////////
// SFMLAnimatedSpriteNode
////////////////////////////////////////////////////
SFMLAnimatedSpriteNode::SFMLAnimatedSpriteNode(ActorId actorId, 
	SFMLBaseRenderComponentWeakPtr renderComponent, 
	const std::string& textureName,
	const std::string& spriteDataName,
	SFMLRenderPass renderPass, 
	sf::Vector2f const& pos, 
	Real const& rot) : SFMLSceneNode(actorId, renderComponent, renderPass, pos, rot), m_scale(1.0), m_textureName(textureName)
{	
	// Now, get the buffer from the resource cache
	Resource r(m_textureName);
	ResourceHandleStrongPtr h = ResourceCacheManager::instance()->getResourceCache()->getResourceHandle(&r);

	// Is the buffer OK?
	if (!h->getBuffer())
	{
		GCC_ERROR("Buffer from texture resource is bad");
	}

	if (!m_texture.loadFromMemory(h->getBuffer(), h->getSize()))
	{
		GCC_ERROR("sf::Texture could not load buffer from memory: " + m_textureName);
	}

	// Load sprite sheet data
	TiXmlElement* pRoot = XmlResourceLoader::loadAndReturnXmlElement(spriteDataName.c_str());
	SpriteSheetDataStrongPtr sheetData = SpriteSheetData::CreateSheetDataFromXML(pRoot);
	if (sheetData)
	{
		m_spriteSheetData = sheetData;
	}
	else
	{
		GCC_WARNING("Could not load sprite data");
	}

	m_SFMLSprite.setTexture(m_texture);
}

bool SFMLAnimatedSpriteNode::onRestore(SFMLScene* scene)
{
	return SFMLSceneNode::onRestore(scene);
}

bool SFMLAnimatedSpriteNode::render(SFMLScene* scene)
{
	// Get Animation components from the actor 
	ActorStrongPtr p = MakeStrongPtr(m_baseRenderComponent->getOwner());
	AnimationComponentStrongPtr animComp = MakeStrongPtr(p->getComponent<AnimationComponent>(AnimationComponent::g_Name));

	// Get the current animation frame
	ant::UInt frame = 0;
	if (animComp->hasActiveAnimation())
		frame = animComp->getFrameIndex();
	
	// Get the frame from the sprite data
	sf::IntRect rect = m_spriteSheetData->getFrame(frame);

	// Set the texture rectangle of the sprite
	m_SFMLSprite.setTextureRect(rect);

	// Flip according to scale
	if (getDirection().x == -1)
	{
		m_SFMLSprite.setScale(-1.0 * (float)m_scale, 1.0* (float)m_scale);
	}
	else if (getDirection().x == 1)
	{
		m_SFMLSprite.setScale(1 * (float)m_scale, 1 * (float)m_scale);
	}

	// First, set proper transformation
	m_SFMLSprite.setOrigin(m_SFMLSprite.getLocalBounds().width / 2, m_SFMLSprite.getLocalBounds().height / 2);
	m_SFMLSprite.setPosition(getPosition());
	m_SFMLSprite.setRotation(float(getRotation()));

	// Tell the renderer to draw the sprite
	return scene->getRenderer()->drawSprite(m_SFMLSprite);

	return true;
}

////////////////////////////////////////////////////
// SFMLBackgroundSpriteNode
////////////////////////////////////////////////////

ant::SFMLBackgroundSpriteNode::SFMLBackgroundSpriteNode( ActorId actorId, 
	SFMLBaseRenderComponentWeakPtr renderComponent, 
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

	// Is the buffer OK?
	if (!h->getBuffer())
	{
		GCC_ERROR("Buffer from texture resource is bad");
	}

	if (!m_texture.loadFromMemory(h->getBuffer(),h->getSize()))
	{		
		GCC_ERROR("sf::Texture could not load buffer from memory: " + m_textureName);
	}

	m_SFMLSprite.setTexture(m_texture);
}

bool ant::SFMLBackgroundSpriteNode::render( SFMLScene *scene ) 
{
	// First, set proper transformation
	m_SFMLSprite.setPosition(getPosition());
	m_SFMLSprite.setRotation(float(getRotation()));	

	// Tell the renderer to draw the sprite
	return scene->getRenderer()->drawSprite(m_SFMLSprite);
}

bool ant::SFMLBackgroundSpriteNode::onRestore( SFMLScene *scene ) 
{
	return SFMLSceneNode::onRestore(scene);
	// TODO - do nothing until I get to know what this is

	// TODO, find the rendering dimension for the window to scale the sprite to it
}

//////////////////////////////////////////////////////////////////////////
// SFMLBackgroundSpriteNode
//////////////////////////////////////////////////////////////////////////

ant::SFMLRectanglePrimitiveNode::SFMLRectanglePrimitiveNode( ActorId actorId, 
	SFMLBaseRenderComponentWeakPtr renderComponent, 
	const sf::Vector2f& size,
	const bool& filled,
	SFMLRenderPass renderPass, 
	const sf::Vector2f& pos, 
	const ant::Real& rot )
	:SFMLSceneNode(actorId,renderComponent,renderPass,pos,rot),
	m_size(size),
	m_filled(filled)
{
		sf::Color color = sf::Color::White;
		if (renderComponent)
		{
			color = renderComponent->getColor(); 
		}
		
		m_rectangleShape.setSize(m_size);
		if (m_filled)
		{
			m_rectangleShape.setFillColor(color);	
			m_rectangleShape.setOutlineThickness(0);	
		}	
		else
		{
			m_rectangleShape.setFillColor(sf::Color::Transparent);
			m_rectangleShape.setOutlineColor(color);
			m_rectangleShape.setOutlineThickness(1);
		}	
		
}

bool ant::SFMLRectanglePrimitiveNode::render( SFMLScene *scene ) 
{
	m_rectangleShape.setPosition(getPosition());
	m_rectangleShape.setRotation(float(getRotation()));	
	m_rectangleShape.setOrigin(m_rectangleShape.getSize().x/2.0, m_rectangleShape.getSize().y/2.0);

	scene->getRenderer()->drawRectangle(m_rectangleShape);
	return true;
}

bool ant::SFMLRectanglePrimitiveNode::onRestore( SFMLScene *scene ) 
{
	return SFMLSceneNode::onRestore(scene);
}

//////////////////////////////////////////////////////////////////////////
// SFMLCirclePrimitiveNode
//////////////////////////////////////////////////////////////////////////

ant::SFMLCirclePrimitiveNode::SFMLCirclePrimitiveNode( ActorId actorId, 
	SFMLBaseRenderComponentWeakPtr renderComponent, 
	const ant::Real& radius, 
	const bool& filled, 
	SFMLRenderPass renderPass, 
	const sf::Vector2f& pos, 
	const ant::Real& rot )
	:SFMLSceneNode(actorId,renderComponent,renderPass,pos,rot),
	m_radius(radius),
	m_filled(filled)
{
	sf::Color color = sf::Color::White;
	if (renderComponent)
	{
		color = renderComponent->getColor(); 
	}
	m_circleShape.setRadius((float)radius);
	if (m_filled)
	{
		m_circleShape.setFillColor(color);	
		m_circleShape.setOutlineThickness(0);	
	}	
	else
	{
		m_circleShape.setFillColor(sf::Color::Transparent);
		m_circleShape.setOutlineColor(color);
		m_circleShape.setOutlineThickness(1);
	}	
}

bool ant::SFMLCirclePrimitiveNode::render( SFMLScene *scene ) 
{
	m_circleShape.setPosition(getPosition());
	m_circleShape.setRotation(float(getRotation()));	
	m_circleShape.setOrigin((float)m_radius, (float)m_radius);

	scene->getRenderer()->drawCircle(m_circleShape);

	return true;
}

bool ant::SFMLCirclePrimitiveNode::onRestore( SFMLScene *scene ) 
{
	return SFMLSceneNode::onRestore(scene);
}
