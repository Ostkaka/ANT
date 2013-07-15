#ifndef BASEGAMELOGIC_HPP_
	#define BASEGAMELOGIC_HPP_

#include <ant/core_types.hpp>
#include <ant/classes/ProcessManager.hpp>
#include <ant/interfaces/IGameLogic.hpp>

namespace ant
{
	/// Just some temporary enums till a better solution comes around
	enum BaseGameState
	{
		BGS_INVALID,
		BGS_INIT,
		BGS_RUNNING
	};

	class LevelManager;
	class ActorFactory;

	typedef std::map<ActorId, StrongActorPtr> ActorMap;
	typedef std::string Level;

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

		ActorId getNewActorId(void);

		/// Returns the random number generator inside the game logic
		GCCRandom& getRNG(void);

		/// Adds a game view to the game logic
		virtual void addGameView(IGameViewStrongPtr pView);

		/// Removes a game view to the game logic
		virtual void removeGameView(IGameViewStrongPtr pView);

		virtual ActorStrongPtr createActor(const std::string &actorResource, TiXmlElement *overrides, 
			const Mat4x4* initialTransform=NULL, const ActorId serversActorId=INVALID_ACTOR_ID);

		virtual void destroyActor(const ActorId id);

		virtual ActorWeakPtr getActor(const ActorId id);

		virtual ActorWeakPtr modifyActor(const ActorId id, TiXmlElement* overrides);

		/// Loads a game
		virtual bool loadGame(const char* levelResource) ANT_OVERRIDE;

		/// Gets the level manager of the game logic
		const LevelManager* getLevelManager() { return m_pLevelManager; }

		/// Logic update function
		virtual void onUpdate(ant::DeltaTime time, ant::DeltaTime elapsedTime);

		/// Change Game logic state 
		virtual void changeState(BaseGameState newState);
		const BaseGameState getState() const;

	protected:

	private:

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:
		/// The process manager of the game logic
		ProcessManager* m_ProcessManager;
		/// Total lifetime of the game logic
		ant::DeltaTime m_lifetime;
		/// Random number generator
		GCCRandom m_rng;
		/// Map that holds all the actors in the game logic
		ActorMap m_actors;
		/// Last id used for a new actor in the logic
		ActorId m_lastActord;
		/// Factory responsible for creating actors
		ActorFactory m_ActorFactory;
		/// Lists of different game views connected to the logic
		GameViewList m_gameViews;
		
		/// Game physics
		bool m_RenderDiagnostics;
		IGamePhysicsStrongPtr m_gamePhysics;

		/// The level manager of the game
		LevelManager* m_pLevelManager;
	};

}

#endif