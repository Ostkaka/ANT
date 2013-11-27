#ifndef IGAMEPHYSICS2D_HPP_
	#define IGAMEPHYSICS2D_HPP_

#include <ant/core_types.hpp>
#include <SFML/Graphics.hpp>

namespace ant
{

	/**
	 * Interface for a a 2d physics engine
	 */
	class IGamePhysics2D 
	{
		public:
		virtual ~IGamePhysics2D() { };

		virtual bool initialize()=0;
		virtual void syncVisibleScene()=0;
		virtual void onUpdate(ant::DeltaTime dt)=0;

		// Create physics object
		virtual	void addSphere(ant::Real radius, ActorWeakPtr actor, std::string density, std::string material)=0;
		virtual	void addBox(const sf::Vector2f& dimensions, ActorWeakPtr actor, std::string density, std::string material)=0;
		virtual void removeActor(const ActorId& id)=0;

		// Physics modifier functions
		virtual void applyForce(const sf::Vector2f& force, const ActorId&)=0;
		virtual bool kinematicMove(const sf::Vector2f& pos, const ActorId&)=0;	

		// Physics actor state
		virtual void setVelocity(const ActorId& id, const sf::Vector2f& vel )=0;
		virtual const sf::Vector2f& getVelocity(const ActorId&)=0;
		virtual void setAngularVelocity(const ActorId& id, ant::Real rotVel)=0;
		virtual const ant::Real& getAngularVelocity(const ActorId&)=0;
		virtual void stopActor( const ActorId& id)=0;				
		virtual void translate(const ActorId& id, const sf::Vector2f& pos)=0;
		virtual void rotate(const ActorId& id, ant::Real rot)=0;		
		virtual void setPosition(const ActorId& id, const sf::Vector2f& pos)=0;
		virtual const sf::Vector2f& getPosition(const ActorId& id)=0;
		virtual void setRotation(const ActorId& id, ant::Real rot)=0;
		virtual const ant::Real& getRotation(const ActorId& id)=0;
	};
}

#endif