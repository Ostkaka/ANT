#include "TestController.hpp"
#include <ant/graphicsSFML/SFMLSceneNode.hpp>
#include <ant/eventsystem/Events.hpp>
#include <ant/classes/BaseGameLogic.hpp>
#include <ant/interfaces/ISFMLApp.hpp>
#include "TestEvents.hpp"
#include <iostream>

using namespace ant;

#define ACTOR_ACCELERATION 5000
#define ACTOR_JUMP_FORCE 1500

ant::TestController::TestController( SFMLSceneNodeStrongPtr target )
{
	m_object = target;
	// Set memory in the key map controller to zero
	memset(m_bKey, 0x00, sizeof(m_bKey));
}

void ant::TestController::onUpdate( ant::DeltaTime dt )
{
	// Check what keys are down
	m_dir.x = 0;
	m_dir.y = 0;
	bool translating = false;
	bool jumping = false;

	if (m_bKey['W'])
	{
		m_dir.y = -1;
		jumping = true;
	}

	if (m_bKey['D'])
	{
		m_dir.x = 1;
		translating = true;
	}

	if (m_bKey['A'])
	{
		m_dir.x = -1;
		translating = true;
	}

	if (translating)
	{		
		ant::Real speed = 5.0;
		sf::Vector2f currentPos = m_object->getPosition();
		ant::Real rotation = m_object->getRotation();
		currentPos += sf::Vector2f(m_dir.x * speed, m_dir.y * speed);
		// Check if we are just moving a camera
		if (m_object && m_object->getNodeProps()->getActorId() == INVALID_ACTOR_ID )
		{				
			m_object->setPosition(currentPos);
		}		
		else
		{			
			const ActorId actorId = m_object->getNodeProps()->getActorId();			
			shared_ptr<EvtData_StartAccelerating> pEvent(GCC_NEW EvtData_StartAccelerating(actorId, (m_dir.x > 0 ? ACTOR_ACCELERATION : (-ACTOR_ACCELERATION))));
			IEventManager::instance()->queueEvent(pEvent);
		}
	}
	else
	{
		const ActorId actorId = m_object->getNodeProps()->getActorId();
		shared_ptr<EvtData_EndAccelerating> pEvent(GCC_NEW EvtData_EndAccelerating(actorId));
		IEventManager::instance()->queueEvent(pEvent);
	}
	if (jumping)
	{
		if (m_object->getNodeProps()->getActorId() != INVALID_ACTOR_ID)
		{
			const ActorId actorId = m_object->getNodeProps()->getActorId();
			shared_ptr<EvtData_StartJump> pEvent(GCC_NEW EvtData_StartJump(actorId, -ACTOR_JUMP_FORCE));
			IEventManager::instance()->queueEvent(pEvent);
		}
	}
}

bool ant::TestController::onKeyDown( const BYTE c )
{
	m_bKey[c] = true; 
	return true;
}

bool ant::TestController::onKeyUp( const BYTE c )
{
	m_bKey[c] = false; 
	return true;
}

