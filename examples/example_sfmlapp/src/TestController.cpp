#include "TestController.hpp"
#include <ant/graphicsSFML/SFMLSceneNode.hpp>
#include <ant/eventsystem/Events.hpp>
#include <ant/classes/BaseGameLogic.hpp>
#include <ant/interfaces/ISFMLApp.hpp>
#include <iostream>

using namespace ant;

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
	bool printInformation = false;
	if (m_bKey['W'])
	{
		m_dir.y = -1;
		translating = true;
	}

	if (m_bKey['S'])
	{
		m_dir.y = 1;
		translating = true;
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

	if (m_bKey['I'])
	{
		printInformation = true;
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
			//IEventDataStrongPtr pData(GCC_NEW EvtData_Move_SFMLActor(m_object->getNodeProps()->getActorId(), currentPos, rotation));
			//EventManager::instance()->queueEvent(pData);
			m_object->getNodeProps()->getActorId();
		}
	}

	if (printInformation)
	{
		ActorMap* amap = ISFMLApp::getApp()->getGameLogic()->getActiveActors();
		std::cout << "================ Printing actor information ================" << std::endl;
		for (auto it = amap->begin() ; it != amap->end() ; ++it)
		{
			std::cout << "id:" << it->first << "  name: " << it->second->getType() << std::endl;
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
