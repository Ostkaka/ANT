#include <ant/graphicsSFML/SFMLRenderComponent.hpp>
#include <ant/graphicsSFML/SFMLSceneNode.hpp>
#include <ant/actors/TransformComponent.hpp>
#include <ant/eventsystem/Events.hpp>

using namespace ant;

const char* SFMLSpriteComponent::g_Name = "SFMLSpriteComponent";

//////////////////////////////////////////////////////////////////////////
// Base Render Component
//////////////////////////////////////////////////////////////////////////

bool SFMLBaseRenderComponent::init( TiXmlElement* pData ) 
{
	return delegateInit(pData);
}

void SFMLBaseRenderComponent::postInit( void ) 
{
	SFMLSceneNodeStrongPtr sceneNode(getSceneNode());
	shared_ptr<EvtData_New_SFMLRender_Component> pEvent(GCC_NEW EvtData_New_SFMLRender_Component(m_pOwner->getId(),sceneNode));
	IEventManager::instance()->triggerEvent(pEvent);
}

void SFMLBaseRenderComponent::onChanged( void ) 
{
	shared_ptr<EvtData_Modified_SFMLRender_Component> pEvent(GCC_NEW EvtData_Modified_SFMLRender_Component(m_pOwner->getId()));
	IEventManager::instance()->triggerEvent(pEvent);
}

TiXmlElement* SFMLBaseRenderComponent::generateXml( void ) 
{
	// TODO Implement XML export
	return NULL;
}

ant::SFMLSceneNodeStrongPtr ant::SFMLBaseRenderComponent::getSceneNode( void )
{
	if (!m_pSceneNode)
	{
		m_pSceneNode = createSceneNode();
	}
	return m_pSceneNode;
}

//////////////////////////////////////////////////////////////////////////
// Sprite Render Component
//////////////////////////////////////////////////////////////////////////
ant::SFMLSpriteComponent::SFMLSpriteComponent( void )
{
	
}

bool ant::SFMLSpriteComponent::delegateInit( TiXmlElement *data )
{
	// Get texture resource path	
	TiXmlElement* texNode = data->FirstChildElement("Texture");
	if (texNode)
	{
		m_textureResource = texNode->FirstChild()->Value();
	}

	return true;
}

ant::SFMLSceneNodeStrongPtr ant::SFMLSpriteComponent::createSceneNode( void ) 
{
	// Try to get the transform component here. Is this an ugly hack?
	TransformComponentStrongPtr pTransformComponent = MakeStrongPtr(m_pOwner->getComponent<TransformComponent>(TransformComponent::g_Name));

	if (pTransformComponent)
	{
		SFMLBaseRenderComponentWeakPtr weakThis(this);

		return SFMLSceneNodeStrongPtr(GCC_NEW SFMLSpriteNode(m_pOwner->getId(),weakThis,m_textureResource,RenderPass_Static,pTransformComponent->getPostion(),pTransformComponent->getRotation()));
	}
	return SFMLSceneNodeStrongPtr();
}

void ant::SFMLSpriteComponent::createInheritedXmlElements( TiXmlElement* pBaseElement )
{
	// TODO create inherited xml stuff in 
}
