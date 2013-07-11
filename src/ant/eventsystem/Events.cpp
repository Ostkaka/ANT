#include <ant/eventsystem/Events.hpp>

using namespace ant;

/**
 * This is the declaration of the unique GUID:s of the event
 */
const EventType EvtData_Update_Tick::sk_EventType(0xf0f5d183);
const EventType EvtData_Test::sk_EventType(0xa3814acd);

void ant::registerEngineScriptEvents( void )
{
	ANT_REGISTER_SCRIPT_EVENT(EvtData_Test, EvtData_Test::sk_EventType);
}

