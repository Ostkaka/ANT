#ifndef TRANSFORMCOMPONENT_HPP_
	#define TRANSFORMCOMPONENT_HPP_

#include <ant/core_types.hpp>
#include <ant/actors/ActorComponent.hpp>
#include <SFML/Graphics.hpp>

namespace ant
{

	class TransformComponent : public ActorComponent
	{
	public:
		TransformComponent(void):
				m_pos(0,0),
				m_rot(0),
				m_direction(1,0)
				{
				}

		virtual const char* getName(void) const; 

		virtual bool init(TiXmlElement* element) ANT_OVERRIDE;

		virtual TiXmlElement* generateXml(void) ANT_OVERRIDE;

		// Transform functions
		void setRotation(const ant::Real rot);
		void setPosition(const sf::Vector2f& pos);
		void setDirection(const sf::Vector2f& dir);
		const sf::Vector2f& getDirection() const;
		const sf::Vector2f& getPostion(void) const;
		ant::Real getRotation(void) const;		

	protected:
		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
		sf::Vector2f  m_pos;
		ant::Real     m_rot;
		sf::Vector2f  m_direction;
	public:
		static const char* g_Name;		
	};

	/// Implementation

	ANT_INLINE const char* TransformComponent::getName() const { return g_Name; }

	ANT_INLINE void TransformComponent::setRotation(ant::Real angle) { m_rot = angle; }

	ANT_INLINE void ant::TransformComponent::setPosition( const sf::Vector2f& pos )	{	m_pos = pos; }

	ANT_INLINE const sf::Vector2f& ant::TransformComponent::getPostion() const	{	return m_pos;	}

	ANT_INLINE ant::Real ant::TransformComponent::getRotation() const	{	return m_rot;	}

	ANT_INLINE const sf::Vector2f& ant::TransformComponent::getDirection() const	{	return m_direction;	}

	ANT_INLINE void ant::TransformComponent::setDirection( const sf::Vector2f& direction ) {	m_direction=direction;	}
}

#endif