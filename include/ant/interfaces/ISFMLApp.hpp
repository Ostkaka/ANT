/**
* This is the abstract app class in ANT that is used for building applications
*/
#ifndef   ISFMLAPP_HPP
	#define   ISFMLAPP_HPP

#include <fstream>
#include <stddef.h>
#include <string.h>
#include <vector>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <ant/core_types.hpp>
#include <ant_config.hpp>

namespace ant
{

	struct GameSettings
	{
		GameSettings(){m_level="";}
		~GameSettings(){}

		std::string m_level;
	};

	class ISFMLApp 
	{
		public:
			/**
       * IApp deconstructor
       */
      virtual ~ISFMLApp();

			/**
			* Will return the most recent created IApp based class. Used for retrieving 
			* assets among other things
			*/
			static ISFMLApp * getApp();
			
			/*
			* Processes the command line for arguments 
			*/
			virtual void processArguments(int argc, char* argv[]);

			/**
			 * Inits the app instance
			 */
			bool init();

			/**
			* Valued of the exit-code given by the application by using quit.
			*/
			int run();

			/**
			* Used to check in case the application is still running
			*/
			bool isRunning() const;

			/**
       * Returns the current game loop update rate being
       * used in Hz (updates per second)
       */
      ant::Real getUpdateRate() const;

			 /**
       * SetUpdateRate will set the game loop update to rate
       */
      void setUpdateRate(float rate);

			 /**
       * Quit will signal the Application to stop running.
       */
			void quit(int exitCode = 1);

			/**
			 * Get the game logic from the APP
			 */
			IGameLogic * getGameLogic();

		protected:

      /**
       * Basic constructor
       */
      ISFMLApp(const std::string theTitle = "Ant Application");
			
      /**
       * Responsible for monitoring IsRunning and exiting when the
       * Application is done.
       */
      virtual void gameLoop();

			/**
			 * Registers events for the game
			 */
			virtual void registerGameEvents(void){}

			/**
      * ProcessInput is responsible for performing all input processing for
      * the game loop.
      */
      virtual void processInput();

			/**
			 * Initializes the game logic and views. 
			 */
			virtual BaseGameLogic* initGameLogicAndView() = 0;

	private:

		/**
		 * Initializes the resource global resource cache
		 */
		bool initResourceCache();

		/**
		 * Initializes the script engine 
		 */
		bool initScriptEngine();

		/**
		 * Initializes the event system
		 */
		bool initEventSystem();

		/**
		 * Registers engine specific events that consists of the backbone
		 */
		void registerEngineEvents(void);

		/**
     * Initializes the Rendering window that
     * will be used to display the games graphics.
     */
    void initRenderer();

		/**
		 * Renders the frame for the game, given that human views are connected to it
		 */
		void renderFrame(ant::DeltaTime fTime, ant::DeltaTime dt);

		/**
     * Responsible for registering and loading the
     * application configuration file (settings.cfg) 
		 */
    void initConfig();

    /**
      * Cleanup is responsible for performing any last minute Application
      * cleanup steps before exiting the Application.
      */
    void cleanup();

    /**
      * App copy constructor is private because we do not allow copies of
      * singleton class. Intentionally undefined.
      */
    ISFMLApp(const ISFMLApp&);               

    /**
      * Assignment operator is private because copies
      * of our Singleton class are not allowed. Intentionally undefined.
      */
    ISFMLApp& operator=(const ISFMLApp&);    

		public:
		  ///CONSTANTS
			/// Default Video width
			static const unsigned int DEFAULT_VIDEO_WIDTH = 800;
			/// Default Video height 
			static const unsigned int DEFAULT_VIDEO_HEIGHT = 600;
			/// Default video bits per pixel (color depth) 
			static const unsigned int DEFAULT_VIDEO_BPP = 32;
			/// Default application wide settings file string
			static const char* APP_SETTINGS;

			//////////////////////////////////////////////////////////////////////////
			// Variables
			//////////////////////////////////////////////////////////////////////////
		  /// Title to use for Window
		  std::string               m_title;
		  /// Video Mode to use (width, height, bpp)
		  sf::VideoMode             m_videoMode;
		  /// Render window to draw to
		  sf::RenderWindow          m_window;
		  /// Window settings to use when creating Render window
		  sf::ContextSettings       m_contextSettings;
		  /// Window style to use when creating Render window
		  unsigned long             m_windowStyle;

			/// Settings for the application	
			GameSettings                 m_settings;

			/// The game logic
			BaseGameLogic*							m_gameLogic;

			// Renderer
			ISFMLRendererStrongPtr		  m_renderer;

			// EventManager
			EventManager*								m_eventManager;

		private:
			/// Static instance variables assigned at creation
			static ISFMLApp*  g_App;

			/// Exit code that is returned upon existing the application
			int						m_exitCode; 

			/// TRUE if the app is still running
			bool					m_running;

			/// True if the application is initialized
			bool					m_initialized;

			/// Value that holds the update rate in seconds used for a fixed loop time
			ant::DeltaTime m_updateRate;
	};
}

#endif /*CORE_APP_HPP*/