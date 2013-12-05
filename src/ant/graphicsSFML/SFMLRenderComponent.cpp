#include <ant/graphicsSFML/SFMLRenderComponent.hpp>
#include <ant/graphicsSFML/SFMLSceneNode.hpp>
#include <ant/actors/TransformComponent.hpp>
#include <ant/eventsystem/Events.hpp>

using namespace ant;

const char* SFMLSpriteComponent::g_Name             = "SFMLSpriteComponent";
const char* SFMLBackgroundSpriteComponent::g_Name   = "SFMLBackgroundSpriteComponent";
const char* SFMLRectanglePrimitiveComponent::g_Name = "SFMLRectanglePrimitiveComponent";
const char* SFMLCirclePrimitiveComponent::g_Name    = "SFMLCirclePrimitiveComponent";

//////////////////////////////////////////////////////////////////////////
// Base Render Component
//////////////////////////////////////////////////////////////////////////

bool SFMLBaseRenderComponent::init( TiXmlElement* pData ) 
{
	// color
	TiXmlElement* pColorNode = pData->FirstChildElement("Color");
	if (pColorNode)
		m_color = loadColor(pColorNode);

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

sf::Color ant::SFMLBaseRenderComponent::loadColor( TiXmlElement * pColorData )
{
	double MAX_COLOR = 256;

	double r = 1.0;
	double g = 1.0;
	double b = 1.0;
	double a = 1.0;

	pColorData->Attribute("r", &r);
	pColorData->Attribute("g", &g);
	pColorData->Attribute("b", &b);
	pColorData->Attribute("a", &a);

	sf::Color color((int)(r*255),(int)(g*255),(int)(b*255),(float)(a*255));

	return color;
}

//////////////////////////////////////////////////////////////////////////
// Sprite Render Component
//////////////////////////////////////////////////////////////////////////
ant::SFMLSpriteComponent::SFMLSpriteComponent( void )
{
	m_scale = 1;
}

bool ant::SFMLSpriteComponent::delegateInit( TiXmlElement *data )
{
	GCC_ASSERT(data);

	// Get texture resource path	
	TiXmlElement* texNode = data->FirstChildElement("Texture");
	if (texNode)
	{
		m_textureResource = texNode->FirstChild()->Value();
	}

	TiXmlElement* scaleNode = data->FirstChildElement("Scale");
	if (scaleNode)
	{
		m_scale = atoi(scaleNode->FirstChild()->Value());
	}
	
	TiXmlElement* textRectNode = data->FirstChildElement("TextureRectangleSize");
	if (textRectNode)
	{
		double x = 1.0;
		double y = 1.0;

		textRectNode->Attribute("w", &x);
		textRectNode->Attribute("h", &y);

		m_textureRectangle = sf::Vector2f(x,y);
	}	

	TiXmlElement* textRectPosNode = data->FirstChildElement("TextureRectanglePos");
	if (textRectPosNode)
	{
		double x = 1.0;
		double y = 1.0;

		textRectPosNode->Attribute("x", &x);
		textRectPosNode->Attribute("y", &y);

		m_textureRectanglePos = sf::Vector2f(x,y);
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
		sf::IntRect rect(m_textureRectanglePos.x,m_textureRectanglePos.y,m_textureRectangle.x,m_textureRectangle.y);
		return SFMLSceneNodeStrongPtr(GCC_NEW SFMLSpriteNode(m_pOwner->getId(),weakThis, m_textureResource, rect, m_scale, RenderPass_Actor,pTransformComponent->getPostion(),pTransformComponent->getRotation()));	
	}
	return SFMLSceneNodeStrongPtr();
}

void ant::SFMLSpriteComponent::createInheritedXmlElements( TiXmlElement* pBaseElement )
{
	// TODO create inherited XML stuff in 
}

//////////////////////////////////////////////////////////////////////////
// SFMLBackgroundSpriteComponent
//////////////////////////////////////////////////////////////////////////

ant::SFMLBackgroundSpriteComponent::SFMLBackgroundSpriteComponent( void )
{

}

ant::SFMLSceneNodeStrongPtr ant::SFMLBackgroundSpriteComponent::createSceneNode( void ) 
{
	// Try to get the transform component here. Is this an ugly hack?
	TransformComponentStrongPtr pTransformComponent = MakeStrongPtr(m_pOwner->getComponent<TransformComponent>(TransformComponent::g_Name));

	if (pTransformComponent)
	{
		SFMLBaseRenderComponentWeakPtr weakThis(this);
		return SFMLSceneNodeStrongPtr(GCC_NEW SFMLBackgroundSpriteNode(m_pOwner->getId(),weakThis,m_textureResource,RenderPass_BackGround,pTransformComponent->getPostion(),pTransformComponent->getRotation()));	
	}
	return SFMLSceneNodeStrongPtr();
}

bool ant::SFMLBackgroundSpriteComponent::delegateInit( TiXmlElement *data ) 
{
	GCC_ASSERT(data);

	// Get texture resource path	
	TiXmlElement* texNode = data->FirstChildElement("Texture");
	if (texNode)
	{
		m_textureResource = texNode->FirstChild()->Value();
	}

	TiXmlElement* scaleNode = data->FirstChildElement("Scale");
	if (scaleNode)
	{
		m_scale = atoi(scaleNode->FirstChild()->Value());
	}

	return true;
}

void ant::SFMLBackgroundSpriteComponent::createInheritedXmlElements( TiXmlElement* pBaseElement ) 
{
	// TODO, spawn XML
}

//////////////////////////////////////////////////////////////////////////
// SFMLRectanglePrimitiveComponent
//////////////////////////////////////////////////////////////////////////

ant::SFMLRectanglePrimitiveComponent::SFMLRectanglePrimitiveComponent( void )
{

}

ant::SFMLSceneNodeStrongPtr ant::SFMLRectanglePrimitiveComponent::createSceneNode( void ) 
{
	// Try to get the transform component here. Is this an ugly hack?
	TransformComponentStrongPtr pTransformComponent = MakeStrongPtr(m_pOwner->getComponent<TransformComponent>(TransformComponent::g_Name));

	if (pTransformComponent)
	{
		SFMLBaseRenderComponentWeakPtr weakThis(this);
		return SFMLSceneNodeStrongPtr(GCC_NEW SFMLRectanglePrimitiveNode(m_pOwner->getId(), weakThis, m_size,m_filled, RenderPass_Actor,pTransformComponent->getPostion(),pTransformComponent->getRotation()));	
	}
	return SFMLSceneNodeStrongPtr();
}

bool ant::SFMLRectanglePrimitiveComponent::delegateInit( TiXmlElement *data ) 
{
	GCC_ASSERT(data);

	TiXmlElement* posElement = data->FirstChildElement("Size");
	if (posElement)
	{
		ant::Real x;
		ant::Real y;

		posElement->Attribute("x",&x);
		posElement->Attribute("y",&y);
		m_size.x = float(x);
		m_size.y = float(y);
	}

	TiXmlElement* texNode = data->FirstChildElement("Filled");
	if (texNode)
	{
		m_filled = (bool)(atoi(texNode->FirstChild()->Value()));
	}

	return true;
}

void ant::SFMLRectanglePrimitiveComponent::createInheritedXmlElements( TiXmlElement* pBaseElement ) 
{
	// TODO, spawn XML
}

//////////////////////////////////////////////////////////////////////////
// SFMLCirclePrimitiveComponent
//////////////////////////////////////////////////////////////////////////

ant::SFMLCirclePrimitiveComponent::SFMLCirclePrimitiveComponent( void )
{
	m_radius = 0;
}

ant::SFMLSceneNodeStrongPtr ant::SFMLCirclePrimitiveComponent::createSceneNode( void ) 
{
	// Try to get the transform component here. Is this an ugly hack?
	TransformComponentStrongPtr pTransformComponent = MakeStrongPtr(m_pOwner->getComponent<TransformComponent>(TransformComponent::g_Name));

	if (pTransformComponent)
	{
		SFMLBaseRenderComponentWeakPtr weakThis(this);
		return SFMLSceneNodeStrongPtr(GCC_NEW SFMLCirclePrimitiveNode(m_pOwner->getId(), weakThis, m_radius ,m_filled, RenderPass_Actor,pTransformComponent->getPostion(),pTransformComponent->getRotation()));	
	}
	return SFMLSceneNodeStrongPtr();
}

bool ant::SFMLCirclePrimitiveComponent::delegateInit( TiXmlElement *data ) 
{
	GCC_ASSERT(data);

	TiXmlElement* scaleNode = data->FirstChildElement("Radius");
	if (scaleNode)
	{
		m_radius = atoi(scaleNode->FirstChild()->Value());
	}

	TiXmlElement* texNode = data->FirstChildElement("Filled");
	if (texNode)
	{
		m_filled = (bool)(atoi(texNode->FirstChild()->Value()));
	}

	return true;
}

void ant::SFMLCirclePrimitiveComponent::createInheritedXmlElements( TiXmlElement* pBaseElement ) 
{
	// TODO, spawn XML
}