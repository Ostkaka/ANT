#include <ant/2DPhysics/PhysicsEvents.hpp>
#include <ant/luascripting/ScriptEvent.hpp>

using namespace ant;

const EventType EvtData_PhysCollision::sk_EventType(0x54c58d0d);
const EventType EvtData_PhysSeparation::sk_EventType(0x3dcea6e1);

void ant::EvtData_PhysCollision::buildEventData(void) 
{
	m_eventData.AssignNewTable(LuaStateManager::instance()->getLuaState());
	m_eventData.SetInteger("actorA", m_ActorA);
	m_eventData.SetInteger("actorB", m_ActorB);
}

void ant::registerPhysicsScriptEvents()
{
	//ANT_REGISTER_SCRIPT_EVENT(EvtData_PhysSeparation, EvtData_PhysSeparation::sk_EventType);
	ANT_REGISTER_SCRIPT_EVENT(EvtData_PhysCollision, EvtData_PhysCollision::sk_EventType);
}

