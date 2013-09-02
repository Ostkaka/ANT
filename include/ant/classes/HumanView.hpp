#ifndef HUMANVIEW_HPP_
	#define HUMANVIEW_HPP_

#include <ant/core_types.hpp>
#include <ant/interfaces/IGameView.hpp>
#include <ant/classes/BaseGameLogic.hpp>
#include <ant/gui/UserInterface.hpp>
#include <SFML/Graphics.hpp>

namespace ant
{

	class SFMLHumanView : public IGameView
	{
		friend class ISFMLApp;

	public:

		SFMLHumanView(ISFMLRendererStrongPtr renderer);
		
		virtual ~SFMLHumanView();

		bool loadGame(TiXmlElement *levelData);

		// Virtual methods to control the layering of interface elements
		virtual void pushElement(shared_ptr<IScreenElement> pElement);

		virtual void removeElement(shared_ptr<IScreenElement> pElement);

		void togglePause(bool active);

		// TODO Audio

		// Camera 
		virtual void setCameraOffset(sf::Vector2f pos);

		// Controller
		virtual void setControllerActor(ActorId actorId);

		// Event delegates
		void gameStateDelegate(IEventDataStrongPtr pEventData);	

		virtual HRESULT onRestore() ANT_OVERRIDE;

		virtual void onRender() ANT_OVERRIDE;

		virtual HRESULT onLostDevice() ANT_OVERRIDE;

		virtual GameViewType getType() ANT_OVERRIDE;

		virtual GameViewId getId() const ANT_OVERRIDE;

		virtual void onAttach(GameViewId id, ActorId actorid) ANT_OVERRIDE;

		virtual LRESULT CALLBACK onMsgProc(AppMsg msg) ANT_OVERRIDE;

		virtual void onUpdate(ant::DeltaTime dt) ANT_OVERRIDE;

	protected:
		
		virtual void renderNext(){}

		virtual bool loadGameDelegate(TiXmlElement* levelData);

	private:

		void registerAllDelegates(void);

		void removeAllDelegates(void);

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////

	protected:
		GameViewId	m_viewId;
		ActorId			m_actorId;		
		ScreenElementSFMLSceneStrongPtr m_Scene;			
		shared_ptr<SFMLCameraNode> m_Camera;		
		ProcessManager* m_processManager;
		ant::DeltaTime	m_currentTime;
		ant::DeltaTime	m_lastDrawTime;
		bool m_realTime; 
		//shared_ptr<IKeyboardHandler> m_KeyboardHandler;

		BaseGameState m_BaseGameState; // Is this even nessecary?
	};

	// Implementation

	ANT_INLINE GameViewId SFMLHumanView::getId() const { return m_viewId; }	

	ANT_INLINE GameViewType SFMLHumanView::getType() {return GameView_Human; }

	ANT_INLINE bool SFMLHumanView::loadGameDelegate(TiXmlElement* levelData) 
	{
		pushElement(m_Scene);
		return true;
	}

	ANT_INLINE void SFMLHumanView::setControllerActor(ActorId aid) { m_actorId = aid; }

	ANT_INLINE void SFMLHumanView::onAttach(GameViewId id, ActorId actorid) 
	{
		m_viewId = id;
		m_actorId = actorid;
	}


}

#endif