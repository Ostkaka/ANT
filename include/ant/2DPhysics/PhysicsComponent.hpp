#ifndef PHYSICSCOMPONENT_HPP_
	#define PHYSICSCOMPONENT_HPP_	

#include <ant/actors/ActorComponent.hpp>
#include <SFML/Graphics.hpp>

namespace ant
{
	/**
	* This is a physics component
	*/ 
	class Physics2DComponent : public ActorComponent
	{
	public:
		Physics2DComponent(void);
		virtual ~Physics2DComponent(void);

		/// Actor component interface
		virtual bool init(TiXmlElement* pData) ANT_OVERRIDE;
		virtual void postInit() ANT_OVERRIDE;
		virtual void update(ant::DeltaTime dt) ANT_OVERRIDE;
		virtual TiXmlElement* generateXml(void) ANT_OVERRIDE;

		/// Physics functions
		void applyForce(const sf::Vector2f& force);
		void applyRotation(const ant::Real force);
		bool kinematicMove(const sf::Vector2f& pos); /// ???
		void applyAcceleration(float acceleration);
		void removeAcceleration(void);

		const sf::Vector2f& getVelocity();		
		void setVelocity(const sf::Vector2f velocity);
		void setPosition(const sf::Vector2f position);
		const sf::Vector2f& getPosition();
		void stop(void);

		const static char *g_Name;
		virtual const char *getName() const ANT_OVERRIDE { return Physics2DComponent::g_Name; }

	protected:
		void createShape();

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:
		std::string             m_shape;
		std::string             m_density;
		std::string             m_material;

		ant::Real								m_acceleration;		
		ant::Real								m_maxVelocity;		

		ant::Real               m_angularAcceleration;
		ant::Real               m_maxAngularVelocity;
				
		IGamePhysics2DStrongPtr m_pPhysics;
	};

}


#endif