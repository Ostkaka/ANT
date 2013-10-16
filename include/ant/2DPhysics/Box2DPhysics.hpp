#ifndef BOX2DPHYSICS_HPP_
	#define BOX2DPHYSICS_HPP_

#include <ant/core_types.hpp>
#include <ant/2DPhysics/IGamePhysics2D.hpp>
#include <SFML/Graphics.hpp>

namespace ant
{

	/**
	 * Implementation of the 2D physics interface using the box2D engine
	 */
	class Box2DPhysics : public IGamePhysics2D
	{
		Box2DPhysics();
		virtual ~Box2DPhysics();

		virtual bool initialize() ANT_OVERRIDE;
		virtual bool syncVisibleScene() ANT_OVERRIDE;
		virtual bool onUpdate(ant::DeltaTime dt) ANT_OVERRIDE;

		// Create physics object
		virtual	void addSphere(ant::Real radius, ActorWeakPtr actor) ANT_OVERRIDE;
		virtual void removeActor(const ActorId& id) ANT_OVERRIDE;

		// Physics modifier functions
		virtual void applyForce(const sf::Vector2f& force) ANT_OVERRIDE;

		// Physics states
		virtual void translate(const ActorId& id, const sf::Vector2f& pos) ANT_OVERRIDE;
		virtual void rotate(const ActorId& id, ant::Real rot) ANT_OVERRIDE;

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:
	};
}

#endif