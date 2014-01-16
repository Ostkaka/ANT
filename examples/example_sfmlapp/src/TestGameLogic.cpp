#include "TestGameLogic.hpp"
#include <ant/eventsystem/Events.hpp>
#include <ant/classes/BaseGameLogic.hpp>
#include <ant/2DPhysics/PhysicsComponent.hpp>
#include <ant/actors/TransformComponent.hpp>
#include <ant/gccUtils/String.hpp>
#include <ant/ant_std.hpp>
#include <ant/gccUtils/templates.hpp>
#include "TestEvents.hpp"

using namespace ant;

ant::TestGameLogic::TestGameLogic()
{
	registerAllDelegates();
	m_HumanPlayersAttached=0;
	m_AIPlayersAttached=0;
}

ant::TestGameLogic::~TestGameLogic()
{
	removeAllDelegates( );
}

void ant::TestGameLogic::changeGameState( BaseGameState newState ) 
{
	BaseGameLogic::changeGameState(newState);	
}

void ant::TestGameLogic::addGameView( IGameViewStrongPtr pView, ActorId actorId/*=INVALID_ACTOR_ID*/ ) 
{
	// Do basic initialization in base class
	BaseGameLogic::addGameView(pView, actorId);
}

void ant::TestGameLogic::TestScriptDelegate( IEventDataStrongPtr pEventData )
{
	shared_ptr<EvtData_TestFromLua> pCastEventData = static_pointer_cast<EvtData_TestFromLua>(pEventData);
	GCC_LOG("Lua", "Event received in C++ from Lua: " + ToStr(pCastEventData->getNumber()));
}

void ant::TestGameLogic::StartAcceleratingDelegate(IEventDataStrongPtr pEventData)
{
	shared_ptr<EvtData_StartAccelerating> pCastEventData = static_pointer_cast<EvtData_StartAccelerating>(pEventData);
	GCC_LOG("GameLogic", "Actor " + ToStr(pCastEventData->getActorId() + "  started accelerating!"));
	
	ActorStrongPtr pActor = MakeStrongPtr(getActor(pCastEventData->getActorId()));
	if (pActor)
	{
		ant::Real acc = pCastEventData->getAcceleration();
		// Change direction of transform
		shared_ptr<TransformComponent> trans = MakeStrongPtr(pActor->getComponent<TransformComponent>(TransformComponent::g_Name));
		trans->setDirection(sf::Vector2f((acc > 0 ? -1.0 : 1.0), 0));
		
		shared_ptr<PhysicsComponent> pPhysicalComponent = MakeStrongPtr(pActor->getComponent<PhysicsComponent>(PhysicsComponent::g_Name));
		if (pPhysicalComponent)
		{
			pPhysicalComponent->applyAcceleration(acc);
		}
	}
}

void ant::TestGameLogic::EndAcceleratingDelegate(IEventDataStrongPtr pEventData)
{
	shared_ptr<EvtData_EndAccelerating> pCastEventData = static_pointer_cast<EvtData_EndAccelerating>(pEventData);
	GCC_LOG("GameLogic", "Actor " + ToStr(pCastEventData->getActorId() + "  ended accelerating!"));

	ActorStrongPtr pActor = MakeStrongPtr(getActor(pCastEventData->getActorId()));
	if (pActor)
	{
		shared_ptr<PhysicsComponent> pPhysicalComponent = MakeStrongPtr(pActor->getComponent<PhysicsComponent>(PhysicsComponent::g_Name));
		if (pPhysicalComponent)
		{
			pPhysicalComponent->removeAcceleration();
		}
	}
}

void ant::TestGameLogic::StartJumpDelegate(IEventDataStrongPtr pEventData)
{
	shared_ptr<EvtData_StartJump> pCastEventData = static_pointer_cast<EvtData_StartJump>(pEventData);
	GCC_LOG("GameLogic", "Actor " + ToStr(pCastEventData->getActorId() + "  jumped!"));

	ActorStrongPtr pActor = MakeStrongPtr(getActor(pCastEventData->getActorId()));
	if (pActor)
	{
		shared_ptr<PhysicsComponent> pPhysicalComponent = MakeStrongPtr(pActor->getComponent<PhysicsComponent>(PhysicsComponent::g_Name));
		if (pPhysicalComponent)
		{
			// Set condition for jump. Should be in contact with ground really?
			if (std::fabs(pPhysicalComponent->getVelocity().y) <= 0.01)
			{
				pPhysicalComponent->applyForce(sf::Vector2f(0, pCastEventData->getAcceleration()));
			}			
		}
	}
}

bool ant::TestGameLogic::loadGameDelegate( TiXmlElement* levelData ) 
{
	registerTestEvents();
	return true;
}

void ant::TestGameLogic::registerAllDelegates( void )
{
	IEventManager* pGlobalEventManager = IEventManager::instance();
	pGlobalEventManager->addListener(MakeDelegate(this, &TestGameLogic::TestScriptDelegate), EvtData_TestFromLua::sk_EventType);
	pGlobalEventManager->addListener(MakeDelegate(this, &TestGameLogic::StartAcceleratingDelegate), EvtData_StartAccelerating::sk_EventType);
	pGlobalEventManager->addListener(MakeDelegate(this, &TestGameLogic::EndAcceleratingDelegate), EvtData_EndAccelerating::sk_EventType);
	pGlobalEventManager->addListener(MakeDelegate(this, &TestGameLogic::StartJumpDelegate), EvtData_StartJump::sk_EventType);
}

void ant::TestGameLogic::removeAllDelegates( void )
{
	IEventManager* pGlobalEventManager = IEventManager::instance();
	pGlobalEventManager->addListener(MakeDelegate(this, &TestGameLogic::TestScriptDelegate), EvtData_TestFromLua::sk_EventType);
	pGlobalEventManager->removeListener(MakeDelegate(this, &TestGameLogic::StartAcceleratingDelegate), EvtData_StartAccelerating::sk_EventType);
	pGlobalEventManager->removeListener(MakeDelegate(this, &TestGameLogic::EndAcceleratingDelegate), EvtData_EndAccelerating::sk_EventType);
	pGlobalEventManager->removeListener(MakeDelegate(this, &TestGameLogic::StartJumpDelegate), EvtData_StartJump::sk_EventType);
}
