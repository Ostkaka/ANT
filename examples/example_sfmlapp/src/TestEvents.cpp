#include "TestEvents.hpp"

using namespace ant;

const EventType EvtData_Set_Controlled_Actor::sk_EventType(0xbe5e3388);
const EventType EvtData_EndAccelerating::sk_EventType(0xe60f88a4);
const EventType EvtData_StartAccelerating::sk_EventType(0xf0b5b4fd);
const EventType EvtData_StartJump::sk_EventType(0x176645ef);

bool EvtData_Set_Controlled_Actor::buildEventFromScript(void)
{
	if (m_eventData.IsInteger())
	{
		m_id = m_eventData.GetInteger();
		return true;
	}

	return false;
}

bool EvtData_EndAccelerating::buildEventFromScript(void)
{
	if (m_eventData.IsTable())
	{
		// ID
		LuaPlus::LuaObject temp = m_eventData.GetByName("id");
		if (temp.IsInteger())
		{
			m_id = temp.GetInteger();
		}
		else
		{
			GCC_ERROR("Invalid id sent to EvtData_EndAccelerating; type = " + std::string(temp.TypeName()));
			return false;
		}

		return true;
	}

	return false;
}

bool EvtData_StartAccelerating::buildEventFromScript(void)
{
	{
		if (m_eventData.IsTable())
		{
			// ID
			LuaPlus::LuaObject temp = m_eventData.GetByName("id");
			if (temp.IsInteger())
			{
				m_id = temp.GetInteger();
			}
			else
			{
				GCC_ERROR("Invalid id sent to EvtData_StartAccelerating; type = " + std::string(temp.TypeName()));
				return false;
			}

			// acceleration
			temp = m_eventData.GetByName("acceleration");
			if (temp.IsNumber())
				m_acceleration = temp.GetFloat();
			else
				m_acceleration = 5.0f;  // something reasonable

			return true;
		}

		return false;
	}
}

bool EvtData_StartJump::buildEventFromScript(void)
{
	if (m_eventData.IsTable())
	{
		// ID
		LuaPlus::LuaObject temp = m_eventData.GetByName("id");
		if (temp.IsInteger())
		{
			m_id = temp.GetInteger();
		}
		else
		{
			GCC_ERROR("Invalid id sent to EvtData_StartJump; type = " + std::string(temp.TypeName()));
			return false;
		}

		// acceleration
		temp = m_eventData.GetByName("acceleration");
		if (temp.IsNumber())
			m_acceleration = temp.GetFloat();
		else
			m_acceleration = 5.0f;  // something reasonable

		return true;
	}

	return false;
}



void ant::registerTestEvents()
{
	ANT_REGISTER_SCRIPT_EVENT(EvtData_Set_Controlled_Actor, EvtData_Set_Controlled_Actor::sk_EventType);
	ANT_REGISTER_SCRIPT_EVENT(EvtData_StartAccelerating, EvtData_StartAccelerating::sk_EventType);
	ANT_REGISTER_SCRIPT_EVENT(EvtData_EndAccelerating, EvtData_EndAccelerating::sk_EventType);
	ANT_REGISTER_SCRIPT_EVENT(EvtData_StartJump, EvtData_StartJump::sk_EventType);
}
