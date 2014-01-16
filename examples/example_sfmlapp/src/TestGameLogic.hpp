#ifndef TESTGAMELOGIC_HPP_
	#define TESTGAMELOGIC_HPP_

#include <ant/core_types.hpp>
#include <ant/classes/BaseGameLogic.hpp>

namespace ant 
{

	class TestGameLogic : public BaseGameLogic
	{
	public:

		TestGameLogic();

		virtual ~TestGameLogic();

		// Overloads
		virtual void changeGameState(BaseGameState newState) ANT_OVERRIDE; 

		virtual void addGameView(IGameViewStrongPtr pView, ActorId actorId=INVALID_ACTOR_ID) ANT_OVERRIDE;
	
		void TestScriptDelegate(IEventDataStrongPtr pEventData);

		void StartAcceleratingDelegate(IEventDataStrongPtr pEventData);

		void EndAcceleratingDelegate(IEventDataStrongPtr pEventData);

		void StartJumpDelegate(IEventDataStrongPtr pEventData);

	protected:

		virtual bool loadGameDelegate(TiXmlElement* levelData) ANT_OVERRIDE;

		
	private:
		
		void registerAllDelegates(void);
		
		void removeAllDelegates(void);

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:
		ant::UInt m_HumanPlayersAttached;
		ant::UInt m_AIPlayersAttached;
	};
}

#endif /*TESTGAMELOGIC_HPP_*/