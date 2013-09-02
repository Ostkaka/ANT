/*
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


		protected:

      /**
       * Basic constructor
       * @param[in] Title of the window
       */
      ISFMLApp(const std::string theTitle = "MGE Application");
			
      /**
       * Responsible for monitoring IsRunning and exiting when the
       * Application is done.
       */
      virtual void gameLoop();

			/**
			 * Registers events for the game
			 */
			virtual void RegisterGameEvents(void){};

	private:

		/**
		 * Registers engine specific events that consists of the backbone
		 */
		void RegisterEngineEvents(void);

		/**
     * Initializes the Rendering window that
     * will be used to display the games graphics.
     */
    void initRenderer();

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
		  std::string               mTitle;
		  /// Video Mode to use (width, height, bpp)
		  sf::VideoMode             mVideoMode;
		  /// Render window to draw to
		  sf::RenderWindow          mWindow;
		  /// Window settings to use when creating Render window
		  sf::ContextSettings       mContextSettings;
		  /// Window style to use when creating Render window
		  unsigned long             mWindowStyle;

			/// The game logic
			BaseGameLogic*							m_gameLogic;

			// Renderer
			ISFMLRenderer*							m_renderer;

			// Process manager
			ProcessManager*							m_manager;

			// EventManager
			EventManager*								m_eventManager;

		private:
			/// Static instance variables assigned at creation
			static ISFMLApp*  gApp;

			/// Exit code that is returned upon existing the application
			int						mExitCode; 

			/// TRUE if the app is still running
			bool					mRunning;

			/// Value that holds the update rate in milliseconds used for a fixed loop time
			float					mUpdateRate;
	};
}

#endif /*CORE_APP_HPP*/