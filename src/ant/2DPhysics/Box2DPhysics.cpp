#include <ant/2DPhysics/Box2DPhysics.hpp>
#include <ant/ant_std.hpp>
#include <ant/gccUtils/templates.hpp>
#include <ant/actors/Actor.hpp>
#include <ant/classes/BaseGameLogic.hpp>
#include <ant/actors/TransformComponent.hpp>
#include <ant/eventsystem/Events.hpp>
#include <SFML/Graphics.hpp>
#include <ant/interfaces/ISFMLApp.hpp>
#include <iostream>

#define M_PI 3.1415926535

using namespace ant;


ant::Real convertRadiansToAngle(const ant::Real radians)
{
	return radians * (180.0 / M_PI);
}

ant::Real convertAngleToRadians(const ant::Real& angle)
{
		return angle * ( M_PI / 180.0 );
}

ant::Box2DPhysics::Box2DPhysics()
{
	m_gameLogic = NULL;
}

ant::Box2DPhysics::~Box2DPhysics()
{

}

bool ant::Box2DPhysics::initialize() 
{
	GCC_INFO("Initializing Game Physics");

	// TODO - Load XML for material
	
	// Create physics world
	m_PhysicsWorld = new b2World(b2Vec2(0,DEFAULT_GRAVITY));
	m_PhysicsWorld->SetGravity(b2Vec2(0,0.02));

	// TODO - initialize the debug drawer

	// Get the logic pointer from the global application
	m_gameLogic = ISFMLApp::getApp()->getGameLogic();

	return true;
}

/// TODO - should this not really be fixed outside the physics stuff?
void ant::Box2DPhysics::syncVisibleScene() 
{
	GCC_ASSERT( m_gameLogic );

	// Keep graphics and physics in synch
	for(auto it = m_actorIDToRigidBody.begin() ; it != m_actorIDToRigidBody.end() ; ++it)
	{
		ActorId const id = it->first;

		// Get the body
		b2Body const * const actorBody = it->second;
		GCC_ASSERT( actorBody );

		// Get the game actor
		ActorStrongPtr pGameActor = MakeStrongPtr(m_gameLogic->getActor(id));

		// Validate and update the transform component from body state
		if (pGameActor && actorBody)
		{			
			TransformComponentStrongPtr transform = MakeStrongPtr(pGameActor->getComponent<TransformComponent>(TransformComponent::g_Name));

			// Converts the position to pixel coordinates
			sf::Vector2f pos = convertBox2DVec2fTosfVector2f(actorBody->GetPosition());
			//std::cout << "Body: " << pGameActor->getId() << "  : " << pos.x  << " " << pos.y << std::endl;

			pos = sf::Vector2f(pos.x * PIXELS_PER_METER, pos.y * PIXELS_PER_METER);
		
			//std::cout << "Body: " << pGameActor->getId() << "  : " << pos.x  << " " << pos.y << std::endl;

			transform->setPosition(pos);
			transform->setRotation(convertRadiansToAngle(actorBody->GetAngle()));
	
			// Send event that the actor have moved
			shared_ptr<EvtData_Move_SFMLActor> pEvent(GCC_NEW EvtData_Move_SFMLActor(id,transform->getPostion(),transform->getRotation()));
			IEventManager::instance()->queueEvent(pEvent);
		}
	}
}

void ant::Box2DPhysics::onUpdate( ant::DeltaTime dt ) 
{
	GCC_ASSERT(m_PhysicsWorld);
	if (m_PhysicsWorld)
	{
		m_PhysicsWorld->Step(dt,DEFAULT_VELOCITY_ITERATIONS,DEFAULT_POSITIONS_ITERATIONS);
	}	
}

void ant::Box2DPhysics::addSphere( ant::Real radius, ActorWeakPtr actor,const std::string& density, const std::string& material, const RigidBodyOptions& options ) 
{
	ActorStrongPtr pActor = MakeStrongPtr(actor);

	if (!pActor)
	{
		GCC_ERROR("Trying to add a shape for an actor is not valid");
	}

	// Create a sphere shape and fixture definition 
	b2CircleShape shape;
	shape.m_radius = radius;

	// Fixture 
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;

	// Add density mass 
	fixtureDef.density = 0.0; // TODO Fix density

	// Add the sphere
	addB2Shape( pActor, fixtureDef, material, options );
}

void ant::Box2DPhysics::addBox( const sf::Vector2f& dimensions, ActorWeakPtr actor, const std::string& density, const std::string& material, const RigidBodyOptions& options ) 
{
	ActorStrongPtr pActor = MakeStrongPtr(actor);

	if (!pActor)
	{
		GCC_ERROR("Trying to add a shape for an actor is not valid");
	}

	// Create a sphere shape and fixture definition 
	b2PolygonShape shape;
	shape.SetAsBox(dimensions.x,dimensions.y);

	// Fixture 
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;

	// Add density mass 
	fixtureDef.density = 5.0; // TODO Fix density

	// Add the sphere
	addB2Shape( pActor, fixtureDef, material , options);
}

void ant::Box2DPhysics::addB2Shape( ActorStrongPtr pActor, b2FixtureDef fixtureDef,const std::string& material, const RigidBodyOptions& options)
{
	GCC_ASSERT( pActor );

	ActorId id = pActor->getId();
	GCC_ASSERT( m_actorIDToRigidBody.find(id) == m_actorIDToRigidBody.end() && "Actor has more than one physics body");

	// Setup material data // TODO
	//MaterialData material();

	// Calculate all the mass data such as inertia and total mass
	b2MassData massData;
	fixtureDef.shape->ComputeMass(&massData,fixtureDef.density);

	// Synch the transform with this body, Is this a hack? This means we MUST include the transform component first!
	sf::Vector2f pos(0,0);
	ant::Real angle = 0;
	TransformComponentStrongPtr pTransform = MakeStrongPtr(pActor->getComponent<TransformComponent>(TransformComponent::g_Name));
	GCC_ASSERT(pTransform);	
	if (pTransform)
	{
		pos = sf::Vector2f(pTransform->getPostion().x / PIXELS_PER_METER , pTransform->getPostion().y / PIXELS_PER_METER);
		angle = convertAngleToRadians(pTransform->getRotation());
	}
	else 
	{
		return;
	}

	// Crete a rigid body from the fixture definition
	b2BodyDef bodyDef;
	if (options.m_motionState == "DYNAMIC")
	{
		bodyDef.type = b2_dynamicBody;
	}
	else if (options.m_motionState == "KINEMATIC")
	{
		bodyDef.type = b2_kinematicBody;
	}

	bodyDef.fixedRotation = options.m_lockRotation;
	
	bodyDef.position = convertsfVector2fToBox2DVec2f(pos);
	bodyDef.angle = angle;
	b2Body * body = m_PhysicsWorld->CreateBody(&bodyDef);	
	body->CreateFixture(&fixtureDef);

	// Insert it into the actor maps
	m_actorIDToRigidBody[id] = body;
	m_rigidBodyToActorID[body] = id;
}

void ant::Box2DPhysics::removeActor( const ActorId& id ) 
{
	if (b2Body * const body = findB2Body(id))
	{
		// Destroy body 
		m_PhysicsWorld->DestroyBody(body);
		m_actorIDToRigidBody.erase(id);
		m_rigidBodyToActorID.erase(body);
	}
}

void ant::Box2DPhysics::applyForce(const sf::Vector2f& force,const ActorId& id) 
{
	// Get body 
	b2Body * body = findB2Body(id);
	GCC_ASSERT(body);

  // Apply force if found
	body->ApplyForceToCenter(convertsfVector2fToBox2DVec2f(force),true);
}

void ant::Box2DPhysics::translate( const ActorId& id, const sf::Vector2f& dx ) 
{
	// Get body 
	b2Body * body = findB2Body(id);
	GCC_ASSERT(body);

	// Translate the object 
	body->SetTransform(body->GetPosition() + convertsfVector2fToBox2DVec2f(dx),body->GetAngle());
}

void ant::Box2DPhysics::rotate( const ActorId& id, ant::Real dr ) 
{
	// Get body 
	b2Body * body = findB2Body(id);
	GCC_ASSERT(body);

	// Rotate the object	
	body->SetTransform(body->GetPosition(),body->GetAngle() + dr);
}

b2Body * ant::Box2DPhysics::findB2Body( const ActorId& id ) const
{
	ActorIdToRigidBody::const_iterator found = m_actorIDToRigidBody.find(id);
	if ( found != m_actorIDToRigidBody.end() )
	{
		return found->second;
	}
	return NULL;
}

ant::ActorId ant::Box2DPhysics::findActorId( b2Body const * const body ) const
{
	RigidBodyToActorId::const_iterator found = m_rigidBodyToActorID.find(body);
	if (found != m_rigidBodyToActorID.end())
	{
		return found->second;
	}
	return NULL;
}

bool ant::Box2DPhysics::kinematicMove( const sf::Vector2f& pos,const ActorId& id ) 
{
	// Get body 
	b2Body * body = findB2Body(id);
	GCC_ASSERT(body);

	// Translate the object 
	body->SetTransform(convertsfVector2fToBox2DVec2f(pos),body->GetAngle());

	return true;
}

void ant::Box2DPhysics::setPosition( const ActorId& id, const sf::Vector2f& pos ) 
{
	kinematicMove(pos, id);
}

void ant::Box2DPhysics::setRotation( const ActorId& id, ant::Real rot ) 
{
	// Get body 
	b2Body * body = findB2Body(id);
	GCC_ASSERT(body);

	// Rotate the object	
	body->SetTransform(body->GetPosition(),rot);
}

void ant::Box2DPhysics::setVelocity( const ActorId& id, const sf::Vector2f& velocity ) 
{
	// Get body 
	b2Body * body = findB2Body(id);
	GCC_ASSERT(body);

	body->SetLinearVelocity(convertsfVector2fToBox2DVec2f(velocity));
}

const sf::Vector2f& ant::Box2DPhysics::getVelocity( const ActorId& id ) 
{
	// Get body 
	b2Body * body = findB2Body(id);
	GCC_ASSERT(body);

	return convertBox2DVec2fTosfVector2f(body->GetLinearVelocity());
}

void ant::Box2DPhysics::setAngularVelocity( const ActorId& id,  ant::Real rotVel ) 
{
	// Get body 
	b2Body * body = findB2Body(id);
	GCC_ASSERT(body);

	body->SetAngularVelocity(rotVel);
}

const ant::Real& ant::Box2DPhysics::getAngularVelocity( const ActorId& id ) 
{
	// Get body 
	b2Body * body = findB2Body(id);
	GCC_ASSERT(body);

	return body->GetAngularVelocity();
}

const sf::Vector2f& ant::Box2DPhysics::getPosition( const ActorId& id ) 
{
	// Get body 
	b2Body * body = findB2Body(id);
	GCC_ASSERT(body);

	return convertBox2DVec2fTosfVector2f(body->GetPosition());
}

const ant::Real& ant::Box2DPhysics::getRotation( const ActorId& id ) 
{
	// Get body 
	b2Body * body = findB2Body(id);
	GCC_ASSERT(body);

	return (body->GetAngle());
}

void ant::Box2DPhysics::stopActor( const ActorId& id ) 
{
	setVelocity(id, sf::Vector2f(0,0));
}



