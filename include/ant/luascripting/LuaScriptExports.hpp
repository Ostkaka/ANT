#ifndef LUASCRIPTEXPORTS_HPP_
	#define LUASCRIPTEXPORTS_HPP_

#include <ant/eventsystem/EventManager.hpp>
#include <LuaPlus.h>

namespace ant
{

	namespace LuaScriptExports
	{
		void registerScripts(void);
		void unregisterScripts(void);
	}

	/**
	 * This class handles the internal script exports to lua
	 */
	class InternalLuaScriptExports
	{
	public:

		/// Initialization
		static bool init(void);
		static void destroy(void);

		/// Export to lua
		static bool loadAndExecutreScriptResource(const char* scriptResource);

		/// Process Manager
		static void attachScriptProcess(LuaPlus::LuaObject scriptProcess);
	
		// Events
		static bool queueEvent(EventType eventType, LuaPlus::LuaObject eventData);
		static bool triggerEvent(EventType eventType, LuaPlus::LuaObject eventData);
		static ScriptEventStrongPtr buildEvent(EventType eventType, LuaPlus::LuaObject eventData);

		// lua log
		static void lualog(LuaPlus::LuaObject text);

		/* Add new export functions as the development progresses */

		

	};
}

#endif