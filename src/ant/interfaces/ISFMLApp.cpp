#include <ant/interfaces/ISFMLApp.hpp>
#include <ant/graphicsSFML/SFMLRenderer.hpp>
#include <ant/classes/BaseGameLogic.hpp>
#include <ant/classes/HumanView.hpp>
#include <ant/eventsystem/IEventManager.hpp>
#include <ant/eventsystem/Events.hpp>
#include <ant/resources/ResourceCacheManager.hpp>
#include <ant/resources/ResourceCache.hpp>
#include <ant/resources/ResourceFiles.hpp>
#include <ant/resources/TextureResource.hpp>
#include <ant/resources/Resource.hpp>
#include <ant/luascripting/LuaStateManager.hpp>
#include <ant/luascripting/ScriptProcess.hpp>
#include <ant/luascripting/LuaScriptExports.hpp>
#include <ant/actors/BaseScriptComponent.hpp>
#include <ant/interfaces/GenericObjectFactory.hpp>

using namespace ant;

const char* SCRIPT_PREINIT_FILE = "lua\\PreInit.lua";

/// Single instance of the most recently created ISFMLApp class
ISFMLApp* ISFMLApp::g_App = NULL;

ant::ISFMLApp::ISFMLApp( const std::string theTitle /*= "Ant Application"*/ )
	:m_contextSettings(),
	m_videoMode(DEFAULT_VIDEO_WIDTH, DEFAULT_VIDEO_HEIGHT, DEFAULT_VIDEO_BPP),
	m_windowStyle(sf::Style::Close | sf::Style::Resize),
	m_gameLogic(NULL),
	m_renderer(NULL),
	m_eventManager(NULL),
	m_running(false),
	m_updateRate(0.016),
	m_initialized(false),
	m_title(theTitle)
{
	g_App = this;
}

ant::ISFMLApp::~ISFMLApp()
{

}

void ant::ISFMLApp::processArguments( int argc, char* argv[] )
{
	// TODO
}

bool ant::ISFMLApp::initResourceCache()
{
	bool resCacheInitialized = false;

	// Init the resource cache in the resource handler singleton
	std::string zipPath = ANT_DATA_PATH"\\unittestdata\\zipdata.zip";
	IResourceFile * resFile = GCC_NEW DevelopmentResourceZipFile(L"Assets.zip", DevelopmentResourceZipFile::Editor);
	
	ResourceCacheManager::create();
	resCacheInitialized = ResourceCacheManager::instance()->initResourceCache(50,resFile);

	// Declare and find the resource loaders
	extern IResourceLoaderStrongPtr CreateXmlResourceLoader();	
	extern IResourceLoaderStrongPtr CreateScriptResourceLoader();
	extern IResourceLoaderStrongPtr CreatePNGResourceLoader();
	extern IResourceLoaderStrongPtr CreateJPGResourceLoader();

	// Register the loaders 
	ResourceCacheManager::instance()->getResourceCache()->registerLoader(CreateXmlResourceLoader());
	ResourceCacheManager::instance()->getResourceCache()->registerLoader(CreateScriptResourceLoader());
	ResourceCacheManager::instance()->getResourceCache()->registerLoader(CreatePNGResourceLoader());
	ResourceCacheManager::instance()->getResourceCache()->registerLoader(CreateJPGResourceLoader());

	return resCacheInitialized;
}

bool ant::ISFMLApp::init()
{
	// Init Logger
	Logger::Init(ANT_DATA_PATH"\\default_logging.xml");

	GCC_LOG("SFMLApplication","Init application");
	
	// Register events
	registerEngineEvents();
	registerGameEvents();

	// Init resource cache
	if(!initResourceCache())
	{
		GCC_ERROR("Failed to initialize resource cache");
	}

	// init Lua state manager
	if (!initScriptEngine())
	{
		GCC_ERROR("Failed to initialize LuaStateManager");
	}

	// init the event system
	if (!initEventSystem())
	{
		GCC_ERROR("Failed to create EventManager.");
	}

	// Init renderer and application window
	initRenderer();

	// Init the game logic and the views from a virtual function create by child class
	m_gameLogic = initGameLogicAndView();
	if (!m_gameLogic)
	{
		return false;
	}

	m_initialized=true;

	return true;
}

int ant::ISFMLApp::run()
{
	GCC_LOG("SFMLApplication","Running application");
	GCC_ASSERT(m_initialized);

	// Init the application
	m_running = true;

	// Start the main loop of the application
	gameLoop();

	// Clean up all the stuff in the application
	cleanup();

	m_running = false;

	return 1;
}

void ant::ISFMLApp::gameLoop()
{
	GCC_LOG("SFMLApplication","Starting gameloop");

	// Init the update clock
	sf::Clock updateClock;

	// Init the frame clock
	sf::Clock frameClock;

	// Start up the clock
	updateClock.restart();

	// When do we update next?
	int updateNext = updateClock.getElapsedTime().asMilliseconds();

	// Begin the main loop
	while (isRunning() && m_window.isOpen())
	{
		// Get the current time
		ant::DeltaTime updateTime = updateClock.getElapsedTime().asSeconds();

		// Is it time to update yet?
		ant::DeltaTime dt = frameClock.getElapsedTime().asSeconds();
		if (dt > m_updateRate)
		{
			// Process input and get it to the game logic and it's views
			processInput();

			// Process events
			IEventManager::instance()->update(); // allow event queue to process for up to 20 ms

			// Update game logic
			m_gameLogic->onUpdate(updateTime,dt);

			// Update renderer
			renderFrame(updateTime,dt);

			// Display contents in the window
			m_window.display();		
		}
	}
}

bool ant::ISFMLApp::isRunning() const
{
	return m_running;
}

ant::Real ant::ISFMLApp::getUpdateRate() const
{
	return m_updateRate;
}

void ant::ISFMLApp::setUpdateRate( float rate )
{
	m_updateRate = rate;
}

void ant::ISFMLApp::quit( int exitCode /*= 1*/ )
{
	m_exitCode = exitCode;
	m_running = false;
}

void ant::ISFMLApp::initRenderer()
{
	GCC_LOG("ISFMLApp", "Init Renderer");

	m_videoMode.width = DEFAULT_VIDEO_WIDTH;
	m_videoMode.height = DEFAULT_VIDEO_HEIGHT;
	m_videoMode.bitsPerPixel = DEFAULT_VIDEO_BPP;

	// Create a RenderWindow object using VideoMode object above
	m_window.create(m_videoMode, m_title, m_windowStyle, m_contextSettings);

	// Create SFML render and connect the window to it
	m_renderer.reset(new SFMLRenderer(&m_window));
}

void ant::ISFMLApp::processInput( )
{
	// Variable for storing the current input event to be processed
	sf::Event tEvent;

	while(m_window.pollEvent(tEvent))
	{
		LRESULT result = 0;

		// Handle some input events and let the current state handle the rest
		// Switch on Event Type
		switch(tEvent.type)
		{
		case sf::Event::Closed:       // Window closed
			quit(StatusAppOK);
			break;
		case sf::Event::GainedFocus:  // Window gained focus			
			// 
			break;
		case sf::Event::LostFocus:    // Window lost focus
			//
			break;
		case sf::Event::Resized:      // Window resized
			break;
		case sf::Event::KeyPressed:
			if (tEvent.key.code == sf::Keyboard::Escape)
			{
				quit(StatusAppOK);
				break;
			}
		default:                      // Current active state will handle
			{				
				if (m_gameLogic)
				{
					// Note the reverse order! User input is grabbed first from the view that is on top, 
					// which is the last one in the list.
					for ( auto it = m_gameLogic->m_gameViews.rbegin(); it!=m_gameLogic->m_gameViews.rend(); ++it )
					{
						if ( (*it)->onMsgProc( tEvent ) )
						{
							result = true;
							break;				// WARNING! This breaks out of the for loop.
						}
					}
				}
			}
		} 
	} 
}

void ant::ISFMLApp::renderFrame(ant::DeltaTime fTime, ant::DeltaTime dt)
{
	if (m_gameLogic)
	{
		for(GameViewList::iterator it=m_gameLogic->m_gameViews.begin(),
			end=m_gameLogic->m_gameViews.end(); it!=end; ++it)
		{
			(*it)->onRender(fTime, dt);
		}
	}

	// TODO - g_pApp->m_pGame->VRenderDiagnostics();
}

bool ant::ISFMLApp::initScriptEngine()
{
	bool luaInitialized = false;

	// First, create the Lua state manager
	luaInitialized = LuaStateManager::create();

	// Load the pre-init file
	{
		Resource resource(SCRIPT_PREINIT_FILE);
		ResourceHandleStrongPtr pPreInitResource = ResourceCacheManager::instance()->getResourceCache()->getResourceHandle(&resource);
	}

	// Register all functions exported from c++
	LuaScriptExports::registerScripts();
	IScriptProcess::registerScriptClass();
	BaseScriptComponent::registerScriptFunctions();

	return luaInitialized;
}

void ant::ISFMLApp::cleanup()
{
	// Clean up the following

	// Resource manager
	ResourceCacheManager::destroy();

	// Lua
	LuaScriptExports::unregisterScripts();
	BaseScriptComponent::unregisterScriptFunctions();
}

bool ant::ISFMLApp::initEventSystem()
{
	m_eventManager = GCC_NEW EventManager("SFMLApp Event Mgr", true );
	return m_eventManager;	
}

void ant::ISFMLApp::registerEngineEvents( void )
{
	//REGISTER_EVENT(EvtData_Environment_Loaded);
	REGISTER_EVENT(EvtData_New_Actor);
	REGISTER_EVENT(EvtData_Move_SFMLActor);
	REGISTER_EVENT(EvtData_Destroy_Actor);
	//REGISTER_EVENT(EvtData_Request_New_Actor);
	//REGISTER_EVENT(EvtData_Network_Player_Actor_Assignment);
}

ISFMLApp * ant::ISFMLApp::getApp()
{
	return g_App;
}

IGameLogic * ant::ISFMLApp::getGameLogic()
{
	return m_gameLogic;
}


