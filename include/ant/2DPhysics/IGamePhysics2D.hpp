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
		virtual ~IGamePhysics2D() { };

		virtual bool initialize()=0;
		virtual bool syncVisibleScene()=0;
		virtual bool onUpdate(ant::DeltaTime dt)=0;

		// Create physics object
		virtual	void addSphere(ant::Real radius, ActorWeakPtr actor)=0;
		virtual void removeActor(const ActorId& id)=0;

		// Physics modifier functions
		virtual void applyForce(const sf::Vector2f& force)=0;

		// Physics states
		virtual void translate(const ActorId& id, const sf::Vector2f& pos)=0;
		virtual void rotate(const ActorId& id, ant::Real rot)=0;		
	};
}

#endif