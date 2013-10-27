#ifndef BOX2DPHYSICS_HPP_
	#define BOX2DPHYSICS_HPP_

#include <ant/core_types.hpp>
#include <ant/2DPhysics/IGamePhysics2D.hpp>
#include <SFML/Graphics.hpp>
#include <box2d/Box2D.h>
#include <box2d/common/b2Math.h>

#define DEFAULT_GRAVITY 9.82
#define DEFAULT_VELOCITY_ITERATIONS 6 
#define DEFAULT_POSITIONS_ITERATIONS 4

namespace ant
{
	
	sf::Vector2f convertBox2DVec2fTosfVector2f(const b2Vec2& vec)
	{
		return sf::Vector2f(vec.x,vec.y);
	}

	b2Vec2 convertsfVector2fToBox2DVec2f(const sf::Vector2f& vec)
	{
		return b2Vec2(vec.x,vec.y);
	}

	/**
	 * Implementation of the 2D physics interface using the box2D engine
	 */
	class Box2DPhysics : public IGamePhysics2D
	{
	public:
		Box2DPhysics();
		virtual ~Box2DPhysics();

		virtual bool initialize() ANT_OVERRIDE;
		virtual void syncVisibleScene() ANT_OVERRIDE;
		virtual void onUpdate(ant::DeltaTime dt) ANT_OVERRIDE;

		// Create physics object
		virtual	void addSphere(ant::Real radius, ActorWeakPtr actor, std::string density, std::string material) ANT_OVERRIDE;
		virtual void removeActor(const ActorId& id) ANT_OVERRIDE;

		// Physics modifier functions
		virtual void applyForce(const sf::Vector2f& force, ActorId id) ANT_OVERRIDE;
		virtual void kinematicMove(const sf::Vector2f& pos, ActorId id) ANT_OVERRIDE;

		virtual void setVelocity(const sf::Vector2f& pos, ActorId id) ANT_OVERRIDE;
		virtual const sf::Vector2f& getVelocity(ActorId id) ANT_OVERRIDE;
		virtual void setAngularVelocity(ant::Real rotVel, ActorId id) ANT_OVERRIDE;
		virtual const sf::Vector2f& getAngularVelocity(ant::Real rotVel) ANT_OVERRIDE;

		// Physics states
		virtual void translate(const ActorId& id, const sf::Vector2f& pos) ANT_OVERRIDE;
		virtual void rotate(const ActorId& id, ant::Real rot) ANT_OVERRIDE;

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:
		typedef std::map<ActorId, b2Body*> ActorIdToRigidBody;
		// TODO material database, density database (Why not in material database?)
		// The box2D world 
		b2World* m_PhysicsWorld;
	};
}

#endif