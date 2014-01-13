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
	// EvtData_New_Actor - Event that is fired when an actor is created
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

}

#endif