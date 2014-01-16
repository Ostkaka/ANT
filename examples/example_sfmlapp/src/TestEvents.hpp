#ifndef TESTEVENTS_HPP_
	#define TESTEVENTS_HPP_

#include <ant/eventsystem/IEventManager.hpp>
#include <ant/eventsystem/Events.hpp>
#include <ant/luascripting/LuaStateManager.hpp>
#include <ant/core_types.hpp>
#include <sfml/Graphics.hpp>

namespace ant
{

	void registerTestEvents();

	//////////////////////////////////////////////////////////////////////////
	// EvtData_Set_Controlled_Actor - Event that is fired to set a controller to an actor
	//////////////////////////////////////////////////////////////////////////
	class EvtData_Set_Controlled_Actor : public ScriptEvent
	{
		ActorId m_id;

	public:
		static const EventType sk_EventType;

		EvtData_Set_Controlled_Actor(void) { }
		EvtData_Set_Controlled_Actor(ActorId actorId)
			: m_id(actorId)
		{
		}

		virtual const EventType& getEventType(void) const ANT_OVERRIDE
		{
			return sk_EventType;
		}

		virtual IEventDataStrongPtr copy() const ANT_OVERRIDE
		{
			return IEventDataStrongPtr(GCC_NEW EvtData_Set_Controlled_Actor(m_id));
		}

		virtual void serialize(std::ostrstream& out) const ANT_OVERRIDE
		{
			out << m_id;
		}

		virtual void deserialize(std::istrstream& in) ANT_OVERRIDE
		{
			in >> m_id;
		}

		const ActorId& getActorId(void) const
		{
			return m_id;
		}

		virtual std::string getName(void) const ANT_OVERRIDE
		{
			return "EvtData_Set_Controlled_Actor";
		}

		virtual bool buildEventFromScript(void) ANT_OVERRIDE;

		ANT_EXPORT_FOR_SCRIPT_EVENT(EvtData_Set_Controlled_Actor);
	};

	//////////////////////////////////////////////////////////////////////////
	// EvtData_StartAccelerating - Event that is fired when an actor should start to accelerate
	//////////////////////////////////////////////////////////////////////////
	class EvtData_StartAccelerating : public ScriptEvent
	{
		ActorId m_id;
		ant::Real m_acceleration;

	public:
		static const EventType sk_EventType;

		EvtData_StartAccelerating(void) : 
			m_id(INVALID_ACTOR_ID),
			m_acceleration(0) { }
		EvtData_StartAccelerating(ActorId actorId, ant::Real acceleration): 
			m_id(actorId),
			m_acceleration(acceleration)
		{
		}

		virtual const EventType& getEventType(void) const ANT_OVERRIDE
		{
			return sk_EventType;
		}

		virtual IEventDataStrongPtr copy() const ANT_OVERRIDE
		{
			return IEventDataStrongPtr(GCC_NEW EvtData_StartAccelerating(m_id,m_acceleration));
		}

		virtual void serialize(std::ostrstream& out) const ANT_OVERRIDE
		{
			out << m_id;
		}

		virtual void deserialize(std::istrstream& in) ANT_OVERRIDE
		{
			in >> m_id;
		}

		const ActorId& getActorId(void) const
		{
			return m_id;
		}

		void setActorId(const ActorId& id)
		{
			m_id = id;
		}

		const ant::Real& getAcceleration(void) const
		{
			return m_acceleration;
		}

		virtual std::string getName(void) const ANT_OVERRIDE
		{
			return "EvtData_StartAccelerate";
		}

		virtual bool buildEventFromScript(void) ANT_OVERRIDE;


		ANT_EXPORT_FOR_SCRIPT_EVENT(EvtData_StartAccelerating);
	};

	//////////////////////////////////////////////////////////////////////////
	// EvtData_EndAccelerating - Event that is fired when an actor should end accelerating
	//////////////////////////////////////////////////////////////////////////
	class EvtData_EndAccelerating : public ScriptEvent
	{
		ActorId m_id;

	public:
		static const EventType sk_EventType;

		EvtData_EndAccelerating(void)
			: m_id(INVALID_ACTOR_ID)
		{
			//
		}

		EvtData_EndAccelerating(ActorId actorId)
			: m_id(actorId)
		{
			//
		}

		virtual const EventType& getEventType(void) const ANT_OVERRIDE
		{
			return sk_EventType;
		}

		virtual IEventDataStrongPtr copy() const ANT_OVERRIDE
		{
			return IEventDataStrongPtr(GCC_NEW EvtData_EndAccelerating(m_id));
		}

		virtual void serialize(std::ostrstream& out) const ANT_OVERRIDE
		{
			out << m_id;
		}

		virtual void deserialize(std::istrstream& in) ANT_OVERRIDE
		{
			in >> m_id;
		}

		const ActorId& getActorId(void) const
		{
			return m_id;
		}

		void setActorId(const ActorId& id)
		{
			m_id = id;
		}

		virtual std::string getName(void) const ANT_OVERRIDE
		{
			return "EvtData_EndAccelerating";
		}

		virtual bool buildEventFromScript(void) ANT_OVERRIDE;

		ANT_EXPORT_FOR_SCRIPT_EVENT(EvtData_EndAccelerating);
	};

	//////////////////////////////////////////////////////////////////////////
	// EvtData_StartJump - Event that is fired when an actor should jump
	//////////////////////////////////////////////////////////////////////////
	class EvtData_StartJump : public ScriptEvent
	{
		ActorId m_id;
		ant::Real m_acceleration;

	public:
		static const EventType sk_EventType;

		EvtData_StartJump(void):
			m_id(INVALID_ACTOR_ID), 
			m_acceleration(0)
		{
		}

		EvtData_StartJump(ActorId actorId, ant::Real acceleration): 
			m_id(actorId),
			m_acceleration(acceleration)
		{
		}

		virtual const EventType& getEventType(void) const ANT_OVERRIDE
		{
			return sk_EventType;
		}

		virtual IEventDataStrongPtr copy() const ANT_OVERRIDE
		{
			return IEventDataStrongPtr(GCC_NEW EvtData_StartJump(m_id,m_acceleration));
		}

		virtual void serialize(std::ostrstream& out) const ANT_OVERRIDE
		{
			out << m_id;
		}

		virtual void deserialize(std::istrstream& in) ANT_OVERRIDE
		{
			in >> m_id;
		}

		const ActorId& getActorId(void) const
		{
			return m_id;
		}

		void setActorId(const ActorId& id)
		{
			m_id = id;
		}

		const ant::Real& getAcceleration()
		{
			return m_acceleration;
		}

		virtual std::string getName(void) const ANT_OVERRIDE
		{
			return "EvtData_StartJump";
		}

		virtual bool buildEventFromScript(void) ANT_OVERRIDE;

		ANT_EXPORT_FOR_SCRIPT_EVENT(EvtData_StartJump);
	};
}

#endif