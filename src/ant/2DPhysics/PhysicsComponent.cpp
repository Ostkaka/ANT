#include <ant/2DPhysics/PhysicsComponent.hpp>
#include <ant/2DPhysics/Box2DPhysics.hpp>
#include <ant/actors/TransformComponent.hpp>
#include <ant/interfaces/ISFMLApp.hpp>
#include <ant/gccUtils/templates.hpp>

using namespace ant;

const char* PhysicsComponent::g_Name = "PhysicsComponent";

const std::string DEFAULT_MATERIAL = "Default";
const std::string DEFAULT_DENSITY = "water";

// units per second
const ant::Real DEFAULT_MAX_VELOCITY = 1.0f;
const ant::Real DEFAULT_MAX_ANGULAR_VELOCITY = 1.0f;

ant::PhysicsComponent::PhysicsComponent( void )
{
	m_lockRotation = false;
	m_isSensor = false;
	m_acceleration        = 0;
	m_angularAcceleration = 0;
	m_maxAngularVelocity  = DEFAULT_MAX_ANGULAR_VELOCITY; 
	m_maxVelocity         = DEFAULT_MAX_VELOCITY;
	m_motionState = "DYNAMIC";
	m_material = DEFAULT_MATERIAL;
	m_density = DEFAULT_DENSITY;
	m_angularDamping = 0;
	m_linearDamping = 0;
}

ant::PhysicsComponent::~PhysicsComponent( void )
{
	m_pPhysics->removeActor(m_pOwner->getId());
}

bool ant::PhysicsComponent::init( TiXmlElement* pData ) 
{
	// Try to find the pointer to the game physics from the active app
	m_pPhysics = ISFMLApp::getApp()->getGameLogic()->getGamePhysics();

	// Check so that we have a game physics
	GCC_ASSERT(m_pPhysics);

	// Shape
	TiXmlElement* pShape = pData->FirstChildElement("Shape");
	if (pShape)
	{
		m_shape = pShape->FirstChild()->Value();
	}

	// Density
	TiXmlElement *pDensity = pData->FirstChildElement("Density");
	if (pDensity)
	{
		m_density = pDensity->FirstChild()->Value();
	}

	// Material
	TiXmlElement *pMaterial = pData->FirstChildElement("PhysicsMaterial");
	if (pMaterial)
	{
		m_material = pMaterial->FirstChild()->Value();
	}

	TiXmlElement *pMotionState = pData->FirstChildElement("MotionState");
	if (pMotionState)
	{
		m_motionState = pMotionState->FirstChild()->Value();
	}

	TiXmlElement *pSensorState = pData->FirstChildElement("IsSensor");
	if (pSensorState)
	{
		m_isSensor = (bool)(atoi(pSensorState->FirstChild()->Value()));
	}

	TiXmlElement *pLockRot = pData->FirstChildElement("LockRotation");
	if (pLockRot)
	{
		m_lockRotation = (bool)(atoi(pLockRot->FirstChild()->Value()));
	}
	
	// Damping 
	TiXmlElement *adampNode = pData->FirstChildElement("AngularDamping");
	if (adampNode)
	{
		m_lockRotation = (bool)(atoi(pLockRot->FirstChild()->Value()));
	}

	// Damping 
	TiXmlElement *ldampNode = pData->FirstChildElement("LinearDamping");
	if (ldampNode)
	{
		m_lockRotation = (bool)(atoi(pLockRot->FirstChild()->Value()));
	}

	// Get transformation properties such as scale, position and stuff
	TiXmlElement *pTranform = pData->FirstChildElement("RigidBodyTransform");
	if (pTranform)
	{
		handleRigidBodyTransform(pTranform);
	}

	return true;
}

void ant::PhysicsComponent::postInit() 
{
	if (m_pOwner)
	{
		// Create options struct
		RigidBodyOptions options;
		options.m_motionState = m_motionState;
		options.m_lockRotation = m_lockRotation;
		options.m_angularDamping = m_angularDamping;
		options.m_linearDamping = m_linearDamping;
		options.m_isSensor = m_isSensor;

		if (m_shape == "Circle")
		{
			m_pPhysics->addSphere(m_rigidBodyScale.x,m_pOwner,m_density,m_material,options);
		}
		else if(m_shape == "Box")
		{
			m_pPhysics->addBox(sf::Vector2f(m_rigidBodyScale.x,m_rigidBodyScale.y),m_pOwner,m_density,m_material,options);
		}
	}
}

void ant::PhysicsComponent::update( ant::DeltaTime dt ) 
{
	// Get the transform component
	TransformComponentStrongPtr pTransform = MakeStrongPtr(m_pOwner->getComponent<TransformComponent>(TransformComponent::g_Name));

	if (!pTransform)
	{
		GCC_ERROR("Actor does not have a transform");
		return;
	}

	// Handle character acceleration
	if (m_acceleration != 0)
	{
		// Calculate the acceleration affect on the actor. This is really shitty for now. minus acceleration does nothing!
		ant::Real dForce = m_acceleration * dt;

		// Get the current velocity of the objects
		sf::Vector2f velocity = m_pPhysics->getVelocity(m_pOwner->getId());

		/// TODO get the direction of the ACTOR!!! This is probably the most reasonable way for now to do it
		sf::Vector2f force = sf::Vector2f(std::fabs(pTransform->getDirection().x) * dForce, std::fabs(pTransform->getDirection().y) * dForce);

		// Apply force to actor
		m_pPhysics->applyForce(force,m_pOwner->getId());
	}

	if (m_angularAcceleration != 0)
	{
		ant::Real angularVelocity = m_angularAcceleration * dt;
	}
}

void ant::PhysicsComponent::applyForce( const sf::Vector2f& force )
{
	m_pPhysics->applyForce(force, m_pOwner->getId());
}

void ant::PhysicsComponent::applyRotation( const ant::Real force )
{
	// TODO
	GCC_ASSERT(false);
}

void ant::PhysicsComponent::applyAcceleration( float acceleration )
{
	m_acceleration = acceleration;
}

void ant::PhysicsComponent::removeAcceleration( void )
{
	m_acceleration = 0;
}

const sf::Vector2f& ant::PhysicsComponent::getVelocity()
{
	return m_pPhysics->getVelocity(m_pOwner->getId());
}

void ant::PhysicsComponent::setVelocity( const sf::Vector2f velocity )
{
	m_pPhysics->setVelocity(m_pOwner->getId(), velocity);
}

void ant::PhysicsComponent::setPosition( const sf::Vector2f position )
{
	// NEED TO UPDATE TRANSFORM COMPOENNT ALSO
	TransformComponentStrongPtr pTransform = MakeStrongPtr(m_pOwner->getComponent<TransformComponent>(TransformComponent::g_Name));
	GCC_ASSERT(pTransform);

	pTransform->setPosition(position);
	kinematicMove(position);

	m_pPhysics->setPosition(m_pOwner->getId(),position);
}

const sf::Vector2f& ant::PhysicsComponent::getPosition()
{
	return m_pPhysics->getPosition(m_pOwner->getId());
}

void ant::PhysicsComponent::stop( void )
{
	m_pPhysics->stopActor(m_pOwner->getId());
}

bool ant::PhysicsComponent::kinematicMove( const sf::Vector2f& pos )
{
	return m_pPhysics->kinematicMove(pos,m_pOwner->getId());
}

TiXmlElement* ant::PhysicsComponent::generateXml( void ) 
{
	return NULL;
}

void ant::PhysicsComponent::handleRigidBodyTransform( TiXmlElement* pData )
{
	TiXmlElement* pScaleElement = pData->FirstChildElement("Scale");
	if (pScaleElement)
	{
		ant::Real x  = 0;
		ant::Real y  = 0;
		ant::Real z  = 0;
		pScaleElement->Attribute("x", &x);
		pScaleElement->Attribute("y", &y);
		pScaleElement->Attribute("z", &z);
		m_rigidBodyScale = sf::Vector3f(x / PIXELS_PER_METER, y / PIXELS_PER_METER, z / PIXELS_PER_METER);
	}
}
