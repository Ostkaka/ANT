#ifndef TESTGAMEVIEW_HPP_
	#define TESTGAMEVIEW_HPP_

#include <ant/classes/HumanView.hpp>
#include <ant/core_types.hpp>
#include "TestController.hpp"

namespace ant
{
	/**
	* Test Game view 
	*/
	class TestGameView : public SFMLHumanView
	{
	public:
		TestGameView(ISFMLRendererStrongPtr renderer);
		
		virtual ~TestGameView();

		// Controller
		virtual void setControllerActor(ActorId actorId);

		virtual void onAttach(GameViewId id, ActorId actorid) ANT_OVERRIDE;

		virtual LRESULT CALLBACK onMsgProc(sf::Event theEvent) ANT_OVERRIDE;

		virtual void onUpdate(ant::DeltaTime dt) ANT_OVERRIDE;

		virtual bool loadGameDelegate(TiXmlElement* levelData) ANT_OVERRIDE;

	private:

		void registerAllDelegates(void);

		void removeAllDelegates(void);

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:
		TestControllerStrongPtr m_TestController;
	};
}

#endif