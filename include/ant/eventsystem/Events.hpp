#ifndef EVENTS_HPP_
	#define EVENTS_HPP_

#include <ant/eventsystem/IEventManager.hpp>
#include <ant/luascripting/ScriptEvent.hpp>
#include <ant/luascripting/LuaStateManager.hpp>
#include <ant/core_types.hpp>

namespace ant
{
	
	/// Registers the event types to script
	void registerEngineScriptEvents( void );

	/**
	 * This event is sent by the game logic each tick.
	 */
	class EvtData_Update_Tick : public BaseEventData
	{
		static const EventType sk_EventType;

		explicit EvtData_Update_Tick( const ant::DeltaTime dt)
			:m_deltaTime(dt)
		{
		}

		virtual const EventType& getEventType( void ) const
		{
			return sk_EventType;
		}

		virtual IEventDataStrongPtr copy() const
		{
			return IEventDataStrongPtr (GCC_NEW EvtData_Update_Tick ( m_deltaTime ) );
		}

		virtual void serialize( std::ostrstream & out )
		{
			GCC_ERROR("You should not be serializing update ticks!");
		}

		virtual std::string getName(void) const
		{
			return "EvtData_Update_Tick";
		}

	protected:
		ant::DeltaTime m_deltaTime; 
	};

	/**
	 * This is a test event used by the unit test to verify the integrity of the system.
	 */
	class EvtData_Test : public ant::ScriptEvent
	{

	public:
		static const EventType sk_EventType;

		explicit EvtData_Test()
			: m_executed(false)
		{		
		}

		virtual const EventType& getEventType(void) const ANT_OVERRIDE
		{
			return sk_EventType;
		}

		virtual IEventDataStrongPtr copy(void) const ANT_OVERRIDE
		{
			return IEventDataStrongPtr ( GCC_NEW EvtData_Test ( ) );
		}

		virtual void serialize(std::ostrstream &out) const ANT_OVERRIDE
		{
			out << m_executed;
		}

		virtual void deserialize(std::istrstream& in) ANT_OVERRIDE
		{
			in >> m_executed;
		}

		virtual std::string getName( void ) const ANT_OVERRIDE
		{
			return "EvtData_Destroy_Actor";
		}

		virtual bool buildEventFromScript( void )
		{
			if (m_eventData.IsBoolean())
			{
				m_executed = m_eventData.GetBoolean();
				return true;
			}
			return false;
		}

		virtual void buildEventData( void )
		{
			m_eventData.AssignNewTable(LuaStateManager::instance()->getLuaState());
			m_eventData.SetBoolean("m_executed", m_executed);				
		}

		void execute(){ m_executed = true;}

		bool isExecuted( void ) const { return m_executed; }

		ANT_EXPORT_FOR_SCRIPT_EVENT(EvtData_Test);

	protected:
		bool m_executed;
	};
}

#endif