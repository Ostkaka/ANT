#include <ant/luascripting/LuaScriptExports.hpp>
#include <ant/luascripting/LuaStateManager.hpp>
#include <ant/resources/ResourceCacheManager.hpp>
#include <ant/resources/ResourceCache.hpp>
#include <ant/resources/Resource.hpp>
#include <ant/core_types.hpp>
#include <ant/ant_std.hpp>
#include <LuaPlus.h>

using namespace ant;



void LuaScriptExports::registerScripts( void )
{
	LuaPlus::LuaObject globals = LuaStateManager::instance()->getGlobalVars();

	// init
	InternalLuaScriptExports::init();

	// resource loading
	globals.RegisterDirect("loadAndExecuteScriptResource",InternalLuaScriptExports::loadAndExecutreScriptResource);	
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

