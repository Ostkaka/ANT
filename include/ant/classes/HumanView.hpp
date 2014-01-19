#ifndef HUMANVIEW_HPP_
	#define HUMANVIEW_HPP_

#include <ant/core_types.hpp>
#include <ant/interfaces/IGameView.hpp>
#include <ant/classes/BaseGameLogic.hpp>
#include <ant/gui/UserInterface.hpp>
#include <ant/interfaces/InputHandlers.hpp>
#include <SFML/Graphics.hpp>

namespace ant
{
	/**
	 * Basic human view for displaying a game logic 
	 */
	class SFMLHumanView : public IGameView
	{
		friend class ISFMLApp;

	public:

		SFMLHumanView(ISFMLRendererStrongPtr renderer);
		
		virtual ~SFMLHumanView();

		bool loadGame(TiXmlElement *levelData);

		// Virtual methods to control the layering of interface elements
		virtual void pushElement(IScreenElementStrongPtr pElement);

		virtual void removeElement(IScreenElementStrongPtr pElement);

		void togglePause(bool active);

		// Camera 
		virtual void setCameraOffset(const sf::Vector2f& pos);

		// Controller
		virtual void setControllerActor(ActorId actorId);

		// Event delegates
		void gameStateDelegate(IEventDataStrongPtr pEventData);	

		virtual bool onRestore() ANT_OVERRIDE;

		virtual void onRender(ant::DeltaTime fTime, ant::DeltaTime fElapsedTime) ANT_OVERRIDE;

		virtual bool onLostDevice() ANT_OVERRIDE;

		virtual void onAttach(GameViewId id, ActorId actorid) ANT_OVERRIDE;

		virtual bool onMsgProc(sf::Event theEvent) ANT_OVERRIDE;

		virtual void onUpdate(ant::DeltaTime dt) ANT_OVERRIDE;

		virtual GameViewType getType() ANT_OVERRIDE;

		virtual GameViewId getId() const ANT_OVERRIDE;

		void toggleDebugRender();

	protected:
		
		virtual void renderText(){}

		virtual bool loadGameDelegate(TiXmlElement* levelData);

	private:

		void registerAllDelegates(void);

		void removeAllDelegates(void);

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////

	protected:
		GameViewId	                    m_viewId;
		ActorId			                m_actorId;		
		ScreenElementSFMLSceneStrongPtr m_Scene;			
		shared_ptr<SFMLCameraNode>      m_Camera;		
		ProcessManager*                 m_processManager;
		ant::TimeStamp	                m_currentTime;
		ant::TimeStamp                  m_lastDrawTime;
		bool                            m_runfullSpeed;
		bool                            m_realTime; 
		ISFMLRendererStrongPtr          m_renderer;
		ScreenElementList               m_ScreenElements;						// a game screen entity
		IKeyboardHandlerStrongPtr       m_KeyboardHandler;
		BaseGameState                   m_BaseGameState;

		// Debug stuff
		bool							m_renderDebug;
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

	ANT_INLINE void SFMLHumanView::pushElement(IScreenElementStrongPtr pElement){ m_ScreenElements.push_front(pElement); }

	ANT_INLINE void SFMLHumanView::removeElement(IScreenElementStrongPtr pElement){ m_ScreenElements.remove(pElement); }

	ANT_INLINE void SFMLHumanView::toggleDebugRender() { m_renderDebug = !m_renderDebug; }
}

#endif