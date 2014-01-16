#ifndef EVENTS_HPP_
#define EVENTS_HPP_

#include <ant/eventsystem/IEventManager.hpp>
#include <ant/luascripting/ScriptEvent.hpp>
#include <ant/luascripting/LuaStateManager.hpp>
#include <ant/core_types.hpp>
#include <sfml/Graphics.hpp>

namespace ant
{
	//////////////////////////////////////////////////////////////////////////
	// EvtData_PhysCollision - Event that is sent when a bodies collide
	//////////////////////////////////////////////////////////////////////////
	class EvtData_PhysCollision : public ScriptEvent
	{
		ActorId m_ActorA;
		ActorId m_ActorB;

	public:
		static const EventType sk_EventType;

		virtual const EventType & getEventType(void) const ANT_OVERRIDE
		{
			return sk_EventType;
		}

		EvtData_PhysCollision()
		{
			m_ActorA = INVALID_ACTOR_ID;
			m_ActorB = INVALID_ACTOR_ID;
		}

		explicit EvtData_PhysCollision(ActorId actorA,
			ActorId actorB) :
			m_ActorA(actorA),
			m_ActorB(actorB)
		{}

		virtual IEventDataStrongPtr copy() const ANT_OVERRIDE
		{
			return IEventDataStrongPtr(GCC_NEW EvtData_PhysCollision(m_ActorA, m_ActorB));
		}

		virtual std::string getName(void) const ANT_OVERRIDE
		{
			return "EvtData_PhysCollision";
		}

		ActorId getActorA(void) const
		{
			return m_ActorA;
		}

		ActorId getActorB(void) const
		{
			return m_ActorB;
		}

		virtual void buildEventData(void) ANT_OVERRIDE;

		EXPORT_FOR_SCRIPT_EVENT(EvtData_PhysCollision);
	};

	//////////////////////////////////////////////////////////////////////////
	// EvtData_PhysCollision - Event that is sent when a bodies separate
	//////////////////////////////////////////////////////////////////////////
	class EvtData_PhysSeparation : public BaseEventData
	{
		ActorId m_ActorA;
		ActorId m_ActorB;

	public:
		static const EventType sk_EventType;

		virtual const EventType & getEventType(void) const ANT_OVERRIDE
		{
			return sk_EventType;
		}

		EvtData_PhysSeparation()
		{
			m_ActorA = INVALID_ACTOR_ID;
			m_ActorB = INVALID_ACTOR_ID;
		}

		explicit EvtData_PhysSeparation(ActorId actorA, ActorId actorB)
			: m_ActorA(actorA)
			, m_ActorB(actorB)
		{}

		virtual IEventDataStrongPtr copy() const ANT_OVERRIDE
		{
			return IEventDataStrongPtr(GCC_NEW EvtData_PhysSeparation(m_ActorA, m_ActorB));
		}

		virtual std::string getName(void) const ANT_OVERRIDE
		{
			return "EvtData_PhysSeparation";
		}

		ActorId getActorA(void) const
		{
			return m_ActorA;
		}

		ActorId getActorB(void) const
		{
			return m_ActorB;
		}
	};
}

#endif