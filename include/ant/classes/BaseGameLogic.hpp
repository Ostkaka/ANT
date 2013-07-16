#ifndef BASEGAMELOGIC_HPP_
	#define BASEGAMELOGIC_HPP_

#include <ant/core_types.hpp>
#include <ant/classes/ProcessManager.hpp>
#include <ant/interfaces/IGameLogic.hpp>
#include <ant/gccUtils/Math.hpp>

class GCCRandom;

namespace ant
{
	/// Just some temporary enums till a better solution comes around
	enum BaseGameState
	{
		BGS_INVALID,
		BGS_INIT,
		BGS_SPAWNINGPLAYERS,
		BGS_RUNNING
	};

	class LevelManager;
	class ActorFactory;

	typedef std::map<ActorId, ActorStrongPtr> ActorMap;
	typedef std::string Level;

	/// How to use this?
	class LevelManager
	{
	public:
		const std::vector<Level> &getLevels() const { return m_Levels; }
		const int getCurrentLevel() const { return m_CurrentLevel; }
		bool initialize(std::vector<std::string> &levels);

	protected:
		std::vector<Level> m_Levels;
		int m_CurrentLevel;
	};

	/**
	* Basic implementation of the basic game logic implementation
	*/
	class BaseGameLogic : public IGameLogic
	{

		friend class ISFMLApp; // Opps this should be abstracted to base App class for SFML, DirectX, etc...

	public:

		BaseGameLogic();

		virtual ~BaseGameLogic();

		bool init(void);

		/// TODO - make Proxy functions

		/// Generate a new unique actor id
		ActorId getNewActorId(void);

		/// Returns the random number generator inside the game logic
		GCCRandom& getRNG(void);

		/// Adds a game view to the game logic
		virtual void addGameView(IGameViewStrongPtr pView, ActorId actorid);

		/// Removes a game view to the game logic
		virtual void removeGameView(IGameViewStrongPtr pView);

		/// Create an actor
		virtual ActorStrongPtr createActor(const std::string &actorResource, TiXmlElement *overrides=NULL, 
			const Mat4x4* initialTransform=NULL, const ActorId serversActorId=INVALID_ACTOR_ID);

		/// Destroy an actor given an id
		virtual void destroyActor(const ActorId id);

		/// Get an actor given an id
		virtual ActorWeakPtr getActor(const ActorId id);

		/// Modify an actor given an id and override it with given XML data
		virtual void modifyActor(const ActorId id, TiXmlElement* overrides);

		/// Loads a game
		virtual bool loadGame(const char* levelResource) ANT_OVERRIDE;

		/// Gets the level manager of the game logic
		const LevelManager* getLevelManager();

		/// Logic update function
		virtual void onUpdate(ant::DeltaTime time, ant::DeltaTime elapsedTime);

		/// Change Game logic state 
		virtual void changeState(BaseGameState newState);
		const BaseGameState getState() const;

		/// Game physics
		void toggleRenderDiagnostics();  /// Should this even be here?
		virtual void renderDiagnostics();
		virtual IGamePhysicsStrongPtr getGamePhysics(void); 

		/// Attach a process into the process manager inside the game logic
		void attachProcesses(IProcessStrongPtr process);

		// editor functions
		std::string getActorXml(const ActorId id);

	protected:
		/// Create an actor factory
		virtual ActorFactory* createActorFactory(void);

		/// Delegate function to load a game given XML data
		virtual bool loadGameDelegate(TiXmlElement* levelData);

	private:

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:
		/// The process manager of the game logic
		ProcessManager* m_processManager;
		/// Total lifetime of the game logic
		ant::DeltaTime m_lifetime;
		/// Random number generator
		GCCRandom m_rng;
		/// Map that holds all the actors in the game logic
		ActorMap m_actors;
		/// Last id used for a new actor in the logic
		ActorId m_lastActorId;
		/// Factory responsible for creating actors
		ActorFactory* m_actorFactory;
		/// Lists of different game views connected to the logic
		GameViewList m_gameViews;
		/// Game state
		BaseGameState m_gameState;
		/// Game physics
		bool m_renderDiagnostics;
		IGamePhysicsStrongPtr m_gamePhysics;

		/// The level manager of the game
		LevelManager* m_levelManager;
	};

	/// Implementation
	ANT_INLINE GCCRandom& BaseGameLogic::getRNG(void) { return m_rng; }

	ANT_INLINE const BaseGameState BaseGameLogic::getState(void) const { return m_gameState; }

	ANT_INLINE ActorId BaseGameLogic::getNewActorId(void) { return m_lastActorId++; }

	ANT_INLINE IGamePhysicsStrongPtr BaseGameLogic::getGamePhysics(void) { return m_gamePhysics; }

	ANT_INLINE const LevelManager* BaseGameLogic::getLevelManager(void) { return m_levelManager; }

	ANT_INLINE void BaseGameLogic::attachProcesses(IProcessStrongPtr process) 
	{
		if (m_processManager)
		{
			m_processManager->attachProcess(process);
		}
	}
}

#endif