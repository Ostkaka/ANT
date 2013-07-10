#include <unittests.hpp>
#include <ant/luascripting/LuaStateManager.hpp>
#include <ant/resources/ResourceCacheManager.hpp>
#include <ant/resources/ResourceCache.hpp>
#include <ant/resources/IResourceFile.hpp>
#include <ant/resources/Resource.hpp>
#include <ant/resources/ResourceFiles.hpp>
#include <ant/luascripting/LuaScriptExports.hpp>
#include <ant/luascripting/ScriptProcess.hpp>
#include <ant/classes/ProcessManagerSingleton.hpp>

using namespace ant;

namespace 
{
	
	const char* SCRIPT_PREINIT_FILE = "lua\\PreInit.lua";

	class Test_LuaStateManager : public ::testing::Test
	{

	protected:

		IResourceFile * m_file;

		Test_LuaStateManager()
		{
			std::cout << "Constructor" << std::endl;
		}

		virtual void SetUp() ANT_OVERRIDE
		{
			// Setup logger
			Logger::Init(ANT_DATA_PATH"\\default_logging.xml");

			m_file = GCC_NEW DevelopmentResourceZipFile(L"Assets.zip", DevelopmentResourceZipFile::Editor);

			ResourceCacheManager::create();
			ASSERT_TRUE(ResourceCacheManager::instance()->initResourceCache(50,m_file));		
			
			// init the global process manager
			ProcessManagerSingleton::create();
			ProcessManagerSingleton::instance()->init();

			// Init lua stuff
			ASSERT_TRUE(LuaStateManager::create());
			LuaScriptExports::registerScripts();
			IScriptProcess::registerScriptClass(); // Should this not be in the register scripts function?

			// Install the script loader
			extern IResourceLoaderStrongPtr CreateScriptResourceLoader();
			ResourceCacheManager::instance()->getResourceCache()->registerLoader(CreateScriptResourceLoader());
		}

		virtual ~Test_LuaStateManager() 
		{
			std::cout << "Destructor" << std::endl;
		}

		virtual void TearDown() ANT_OVERRIDE
		{
			ResourceCacheManager::destroy();
			LuaStateManager::destroy();
			LuaScriptExports::unregisterScripts();
			ProcessManagerSingleton::destroy();

			std::cout << "tear down" << std::endl;
		}
	};
}

TEST_F(Test_LuaStateManager, Init) 
{
	// Start by reading the pre-init file	
	{
		Resource resource(SCRIPT_PREINIT_FILE);
		ResourceHandleStrongPtr pResourceHandle = ResourceCacheManager::instance()->getResourceCache()->getResourceHandle(&resource);  
	}
	// Now, try to find the init functions in the lua state
	LuaPlus::LuaObject obj = LuaStateManager::instance()->getGlobalVars().GetByName("class");

	// The class function should have been imported by now
	ASSERT_TRUE(obj.IsFunction());

	// Now, load the test file with the sample test script
	{
		Resource resource("lua\\testScript.lua");
		ResourceHandleStrongPtr pResourceHandle = ResourceCacheManager::instance()->getResourceCache()->getResourceHandle(&resource);  
	}

	// Check so that the TestProcess is loaded
	LuaPlus::LuaObject metaTableObj = LuaStateManager::instance()->getGlobalVars().Lookup("ScriptProcess");
	ASSERT_TRUE(!metaTableObj.IsNil());
}

TEST_F(Test_LuaStateManager, ScriptProcess) 
{
	// Start by reading the pre-init file	
	{
		Resource resource(SCRIPT_PREINIT_FILE);
		ResourceHandleStrongPtr pResourceHandle = ResourceCacheManager::instance()->getResourceCache()->getResourceHandle(&resource);  
	}

	// Now, load the test file with the sample test script
	{
		Resource resource("lua\\testScript.lua");
		ResourceHandleStrongPtr pResourceHandle = ResourceCacheManager::instance()->getResourceCache()->getResourceHandle(&resource);  
	}

	// Test script process
	LuaPlus::LuaObject obj = LuaStateManager::instance()->getGlobalVars().GetByName("TestProcess");

	// See so that the function exists
	ASSERT_TRUE(obj.IsFunction());

	// Do the test function and insert a process in the global process manager
	LuaPlus::LuaFunction<void> func(obj);
	func(LuaStateManager::instance()->getLuaState());

	// We should now have one process in the manager
	ASSERT_TRUE((ProcessManagerSingleton::instance()->getProcessManager()->getNumProcesses() == 1));

	// loop for a while
	ant::DeltaTime dt = 1;
	ant::DeltaTime currT = 0;	
	ant::DeltaTime endT= 5;
	while(currT < endT)
	{
		ProcessManagerSingleton::instance()->getProcessManager()->updateProcesses(dt);
		currT += dt;
	}
	
	// Check so that the process object exists	
	LuaPlus::LuaObject sProcess = LuaStateManager::instance()->getGlobalVars().GetByName("sParent");
	ASSERT_TRUE(!sProcess.IsNil());
	ASSERT_TRUE(sProcess.IsTable());

	//Check that finished is set to true
	LuaPlus::LuaObject finished = sProcess.GetByName("finished");
	ASSERT_TRUE(finished.IsBoolean());

	// It should be true since we looped after the execute
	ASSERT_TRUE(finished.GetBoolean());

	// Now, loop a little more 
	endT = 10;
	dt = 0.1;
	while(currT < endT)
	{
		ProcessManagerSingleton::instance()->getProcessManager()->updateProcesses(dt);
		currT += dt;
	}

	// Get the process child
	LuaPlus::LuaObject cProcess = LuaStateManager::instance()->getGlobalVars().GetByName("sChild");
	ASSERT_TRUE(!cProcess.IsNil());
	ASSERT_TRUE(cProcess.IsTable());

	//Check that finished is set to true
	LuaPlus::LuaObject cfinished = cProcess.GetByName("finished");
	ASSERT_TRUE(cfinished.IsBoolean());

	// It should be true since we looped after the execute
	ASSERT_TRUE(cfinished.GetBoolean());
}

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}