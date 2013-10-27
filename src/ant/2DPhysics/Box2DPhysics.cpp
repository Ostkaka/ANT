#include <ant/2DPhysics/Box2DPhysics.hpp>

using namespace ant;

bool ant::Box2DPhysics::initialize() 
{
	// Create physics world
	m_PhysicsWorld = new b2World(b2Vec2(0,DEFAULT_GRAVITY));

	return true;
}

void ant::Box2DPhysics::syncVisibleScene() 
{

}

void ant::Box2DPhysics::onUpdate( ant::DeltaTime dt ) 
{
	GCC_ASSERT(m_PhysicsWorld);
	if (m_PhysicsWorld)
	{
		m_PhysicsWorld->Step(dt,DEFAULT_VELOCITY_ITERATIONS,DEFAULT_POSITIONS_ITERATIONS);
	}	
}

void ant::Box2DPhysics::addSphere( ant::Real radius, ActorWeakPtr actor, std::string density, std::string material) 
{

}

void ant::Box2DPhysics::removeActor( const ActorId& id ) 
{

}

void ant::Box2DPhysics::applyForce(const sf::Vector2f& force, ActorId id) 
{

}

void ant::Box2DPhysics::translate( const ActorId& id, const sf::Vector2f& pos ) 
{

}

void ant::Box2DPhysics::rotate( const ActorId& id, ant::Real rot ) 
{

}

