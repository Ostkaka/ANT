#ifndef LUASCRIPTEXPORTS_HPP_
	#define LUASCRIPTEXPORTS_HPP_

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

		/* Add new export functions as the development progresses */

	};
}

#endif