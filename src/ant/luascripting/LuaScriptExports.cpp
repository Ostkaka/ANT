#include <ant/luascripting/LuaScriptExports.hpp>
#include <ant/luascripting/LuaStateManager.hpp>
#include <ant/resources/ResourceCacheManager.hpp>
#include <ant/classes/ProcessManagerSingleton.hpp>
#include <ant/classes/ProcessManager.hpp>
#include <ant/interfaces/IProcess.hpp>
#include <ant/resources/ResourceCache.hpp>
#include <ant/resources/Resource.hpp>
#include <ant/core_types.hpp>
#include <ant/ant_std.hpp>
#include <LuaPlus.h>

using namespace ant;

void LuaScriptExports::registerScripts( void )
{
	LuaPlus::LuaObject globals = LuaStateManager::instance()->getGlobalVars();

	// Init
	InternalLuaScriptExports::init();

	// Resource loading
	globals.RegisterDirect("loadAndExecuteScriptResource",InternalLuaScriptExports::loadAndExecutreScriptResource);	

	// Process manager
	globals.RegisterDirect("attachProcess",&InternalLuaScriptExports::attachScriptProcess);

	// lua log
	globals.RegisterDirect("log",&InternalLuaScriptExports::lualog);
}

void LuaScriptExports::unregisterScripts( void )
{
	InternalLuaScriptExports::destroy();
}


//////////////////////////////////////////////////////////////////////////
// InternalLuaScriptExports
//////////////////////////////////////////////////////////////////////////

bool ant::InternalLuaScriptExports::init( void )
{
	return true;
}

void ant::InternalLuaScriptExports::destroy( void )
{

}

bool ant::InternalLuaScriptExports::loadAndExecutreScriptResource( const char* scriptResource )
{
	// Check if cache is loaded
	if (!ResourceCacheManager::instance()->getResourceCache())
	{
		GCC_WARNING("Tried to access cache in CacheManager, but it was a bad pointer");
		return false;
	}
	
	if (!ResourceCacheManager::instance()->getResourceCache()->isUsingDevelopmentDirectories())
	{
		Resource resource(scriptResource);
		// this actually loads the Lua file from the zip file
		ResourceHandleStrongPtr pResourceHandle = ResourceCacheManager::instance()->getResourceCache()->getResourceHandle(&resource);  
		if (pResourceHandle)
		{
			return true;
		}
		return false;
	}
	else
	{
		std::string path(ANT_DATA_PATH"\\"); // TODO
		path += scriptResource;
		LuaStateManager::instance()->executeFile(path.c_str());
		return true;
	}
	
	return true;
}

void ant::InternalLuaScriptExports::attachScriptProcess( LuaPlus::LuaObject scriptProcess )
{
	LuaPlus::LuaObject temp = scriptProcess.Lookup("__object");
	if (!temp.IsNil())
	{
		IProcessStrongPtr pProcess(static_cast<IProcess*>(temp.GetLightUserData()));
		ProcessManagerSingleton::instance()->getProcessManager()->attachProcess(pProcess);
	}
	else
	{
		GCC_ERROR("Could not find __object in script proces");
	}
}

void ant::InternalLuaScriptExports::lualog( LuaPlus::LuaObject text )
{
	if (text.IsConvertibleToString())
	{
		GCC_LOG("Lua",text.ToString());
	}
	else
	{
		GCC_LOG("Lua","<" + std::string(text.TypeName()) + ">");
	}

}

