#include <ant/actors/TransformComponent.hpp>
#include <ant/gccUtils/String.hpp>

using namespace ant;

const char* TransformComponent::g_Name = "TransformComponent";

bool ant::TransformComponent::init( TiXmlElement* data ) 
{
	GCC_ASSERT(data);

	TiXmlElement* posElement = data->FirstChildElement("Position");
	if (posElement)
	{
		ant::Real x;
		ant::Real y;

		posElement->Attribute("x",&x);
		posElement->Attribute("y",&y);
		m_pos.x = float(x);
		m_pos.y = float(y);
	}

	TiXmlElement* rotElement = data->FirstChildElement("Rotation");
	if (rotElement)
	{
		ant::Real angle;
		rotElement->Attribute("angle",&angle);
		m_rot = angle;
	}

	return true;
}

TiXmlElement* ant::TransformComponent::generateXml( void ) 
{
	GCC_ERROR("Cannot generate xml from TransformComponent");
	return NULL;
}

