#ifndef SCRIPTEVENTLISTENER_HPP_
	#define SCRIPTEVENTLISTENER_HPP_

#include <ant/core_types.hpp>
#include <ant/eventsystem/IEventManager.hpp>
#include <set>
#include <LuaPlus.h>

namespace ant
{
	/**
	* This is a C++ listener proxy for script event listeners. Pairs with a single type of event with a
	* lua callback function.
	*/
	class ScriptEventListener
	{
	public:
		explicit ScriptEventListener(const EventType& eventType, const LuaPlus::LuaObject& scriptCallbackFunction);
		~ScriptEventListener();
		
		/// Returns the delegate of the listener
		EventListenerDelegate getDelegate(void);

		void scriptEventDelegate(IEventDataStrongPtr pEvent);

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:
		LuaPlus::LuaObject m_scriptCallbackFunction;
		EventType m_eventType;
	};

	/**
	 * This class manages C++ event listener proxies.
	 */
	class ScriptEventListenerManager
	{
		typedef std::set<ScriptEventListener*> ScriptEventListenerSet;
		
	public:
		/// Default destructor
		~ScriptEventListenerManager(void);

		/// Add a proxy eventListener to the manager
		void addListener(ScriptEventListener* pListener);

		/// Remove a proxy eventListener to the manager
		void destroyListener(ScriptEventListener* pListener);

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:
		ScriptEventListenerSet m_listener;
	};
}


#endif