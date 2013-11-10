#include <ant/2DPhysics/PhysicsComponent.hpp>
#include <ant/2DPhysics/Box2DPhysics.hpp>
#include <ant/actors/TransformComponent.hpp>
#include <ant/interfaces/ISFMLApp.hpp>

using namespace ant;

const char* Physics2DComponent::g_Name = "Physics2DComponent";

// units per second
const ant::Real DEFAULT_MAX_VELOCITY = 1.0f;
const ant::Real DEFAULT_MAX_ANGULAR_VELOCITY = 1.0f;

ant::Physics2DComponent::Physics2DComponent( void )
{
	m_acceleration        = 0;
	m_angularAcceleration = 0;
	m_maxAngularVelocity  = DEFAULT_MAX_ANGULAR_VELOCITY; 
	m_maxVelocity         = DEFAULT_MAX_VELOCITY;
}

ant::Physics2DComponent::~Physics2DComponent( void )
{
	m_pPhysics->removeActor(m_pOwner->getId());
}

bool ant::Physics2DComponent::init( TiXmlElement* pData ) 
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
		m_density = pMaterial->FirstChild()->Value();
	}

	return true;
}

void ant::Physics2DComponent::postInit() 
{

	if (m_pOwner)
	{
		if (m_shape == "Circle")
		{
			m_pPhysics->addSphere(1.0,m_pOwner,m_density,m_material);
		}
	}
}

void ant::Physics2DComponent::update( ant::DeltaTime dt ) 
{
	// Get the transform component
	TransformComponentStrongPtr pTransform = MakeStrongPtr(m_pOwner->getComponent<TransformComponent>(TransformComponent::g_Name));

	if (pTransform)
	{
		GCC_ERROR("Actor does not have a transform");
		return;
	}

	// Handle character acceleration
	if (m_acceleration != 0)
	{
		// Calculate the acceleration affect on the actor
		ant::Real dForce = m_acceleration * dt;

		// Get the current velocity of the objects
		sf::Vector2f velocity = m_pPhysics->getVelocity(m_pOwner->getId());

		/// TODO get the direction of the ACTOR!!! This is probably the most reasonable way for now to do it
		sf::Vector2f force = sf::Vector2f(pTransform->getDirection().x * dForce,pTransform->getDirection().y * dForce);

		// Apply force to actor
		m_pPhysics->applyForce(force,m_pOwner->getId());
	}

	if (m_angularAcceleration != 0)
	{
		ant::Real angularVelocity = m_angularAcceleration * dt;
	}
}

void ant::Physics2DComponent::applyForce( const sf::Vector2f& force )
{
	m_pPhysics->applyForce(force, m_pOwner->getId());
}

void ant::Physics2DComponent::applyRotation( const ant::Real force )
{
	// TODO
}

void ant::Physics2DComponent::applyAcceleration( float acceleration )
{
	m_acceleration = acceleration;
}

void ant::Physics2DComponent::removeAcceleration( void )
{
	m_acceleration = 0;
}

const sf::Vector2f& ant::Physics2DComponent::getVelocity()
{
	return m_pPhysics->getVelocity(m_pOwner->getId());
}

void ant::Physics2DComponent::setVelocity( const sf::Vector2f velocity )
{
	m_pPhysics->setVelocity(velocity, m_pOwner->getId());
}

void ant::Physics2DComponent::setPosition( const sf::Vector2f position )
{
	// NEED TO UPDATE TRANSFORM COMPOENNT ALSO
	TransformComponentStrongPtr pTransform = MakeStrongPtr(m_pOwner->getComponent<TransformComponent>(TransformComponent::g_Name));
	GCC_ASSERT(pTransform);

	pTransform->setPosition(position);
	kinematicMove(position);

	m_pPhysics->setPosition(m_pOwner->getId(),position);
}

const sf::Vector2f& ant::Physics2DComponent::getPosition()
{
	return m_pPhysics->getPosition(m_pOwner->getId());
}

void ant::Physics2DComponent::stop( void )
{
	m_pPhysics->stopActor(m_pOwner->getId());
}

bool ant::Physics2DComponent::kinematicMove( const sf::Vector2f& pos )
{
	return m_pPhysics->kinematicMove(pos,m_pOwner->getId());
}

TiXmlElement* ant::Physics2DComponent::generateXml( void ) 
{
	return NULL;
}

