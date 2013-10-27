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
		virtual void removeActor(const ActorId& id)=0;

		// Physics modifier functions
		virtual void applyForce(const sf::Vector2f& force, ActorId id)=0;
		virtual void kinematicMove(const sf::Vector2f& pos, ActorId id)=0;	
		virtual void setVelocity(const sf::Vector2f& pos, ActorId id)=0;
		virtual const sf::Vector2f& getVelocity(ActorId id)=0;
		virtual void setAngularVelocity(ant::Real rotVel, ActorId id)=0;
		virtual const sf::Vector2f& getAngularVelocity(ant::Real rotVel)=0;

		// Physics states
		virtual void translate(const ActorId& id, const sf::Vector2f& pos)=0;
		virtual void rotate(const ActorId& id, ant::Real rot)=0;		
	};
}

#endif