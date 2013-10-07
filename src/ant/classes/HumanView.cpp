#include <ant/classes/HumanView.hpp>
#include <ant/classes/ProcessManager.hpp>
#include <ant/eventsystem/IEventManager.hpp>
#include <ant/core_types.hpp>
#include <ant/gui/UserInterface.hpp>
#include <ant/interfaces/GenericObjectFactory.hpp>

#define SCREEN_REFRESH_RATE (1.0/60.0)

using namespace ant;

ant::SFMLHumanView::SFMLHumanView( ISFMLRendererStrongPtr renderer )
{
	// TODO - Init Audio

	m_processManager = GCC_NEW ProcessManager;

	m_viewId = ant_InvalidGameViewId;

	registerAllDelegates();

	m_BaseGameState = BGS_INIT;

	// Init the scene here
	m_renderer = renderer;
	if(renderer)
	{
		m_Camera.reset(GCC_NEW SFMLCameraNode(sf::Vector2f(),0));

		m_Scene.reset(GCC_NEW ScreenElementSFMLScene(renderer));
		m_Scene->addChild(INVALID_ACTOR_ID, m_Camera);	
		m_Scene->setCamera(m_Camera);
	}
}

ant::SFMLHumanView::~SFMLHumanView()
{
	// Remove events 
	removeAllDelegates();

	while(!m_ScreenElements.empty())
	{
		m_ScreenElements.pop_front();
	}

	SAFE_DELETE(m_processManager);

	// TODO - delete audio here
}

bool ant::SFMLHumanView::loadGame( TiXmlElement *levelData )
{
	return loadGameDelegate(levelData);
}

void ant::SFMLHumanView::togglePause( bool active )
{
	// TODO - audio
}

void ant::SFMLHumanView::setCameraOffset(const sf::Vector2f& pos )
{
	GCC_ASSERT(m_Camera);
	if (m_Camera)
	{
		m_Camera->setCameraOffset(pos);
	}
}

void ant::SFMLHumanView::gameStateDelegate( IEventDataStrongPtr pEventData )
{
	// TODO - this is not event used. What does this mean?
}

void ant::SFMLHumanView::onRender(ant::DeltaTime fTime, ant::DeltaTime fElapsedTime) 
{
	// TODO - add clock here! WTF?!? Why used milliseconds here? Why not use fTime and fElasped time?
	m_currentTime = 0;

	// Render if we have a valid renderer
	GCC_ASSERT(m_renderer);
	if ( m_runfullSpeed || (m_currentTime - m_lastDrawTime) > SCREEN_REFRESH_RATE )
	{
		if (m_renderer->preRender())
		{
			m_ScreenElements.sort(SortBy_SharedPtr_Content<IScreenElement>());

			for (auto it=m_ScreenElements.begin() ; it != m_ScreenElements.end() ; ++it)
			{
				if ((*it)->isVisible())
				{
					(*it)->onRender(fTime, fElapsedTime);
				}
			}

			// TODO - render text

			// TODO - render console

			// Record last successfully timestamp
		}
		m_renderer->postRender();
	}
}

HRESULT ant::SFMLHumanView::onRestore() 
{
	HRESULT hr = S_OK;
	for(ScreenElementList::iterator i=m_ScreenElements.begin(); i!=m_ScreenElements.end(); ++i)
	{
		V_RETURN ( (*i)->onRestore() );
	}

	return hr;
}

HRESULT ant::SFMLHumanView::onLostDevice() 
{
	HRESULT hr = S_OK;
	for(ScreenElementList::iterator i=m_ScreenElements.begin(); i!=m_ScreenElements.end(); ++i)
	{
		V_RETURN ( (*i)->onLostDevice() );
	}

	return S_OK;
}

void ant::SFMLHumanView::onUpdate( ant::DeltaTime dt ) 
{
	// Update processes
	m_processManager->updateProcesses(dt);

	// Update all the screen elements 
	for (auto it = m_ScreenElements.begin() ; it != m_ScreenElements.end() ; ++it)
	{
		(*it)->onUpdate(dt);
	}
}

LRESULT CALLBACK ant::SFMLHumanView::onMsgProc( sf::Event theEvent ) 
{
	// Go through the layers in reverse order
	for (auto it = m_ScreenElements.rbegin(); it != m_ScreenElements.rend() ; ++it)
	{
		if ((*it)->isVisible())
		{
			if ((*it)->onMsgProc(theEvent))
			{
				return 1;
			}
		}
	}

	LRESULT result = 0;
	switch(theEvent.type)
	{
		// TODO - add controller here for handling stuff
		// TODO - add console
		default:
			return 0;
	}

	return 0;
}

void ant::SFMLHumanView::registerAllDelegates( void )
{
	IEventManager* pGlobalEventManager = IEventManager::instance();	
	// TODO - audio
	//pGlobalEventManager->VAddListener(MakeDelegate(this, &HumanView::PlaySoundDelegate), EvtData_PlaySound::sk_EventType);
}

void ant::SFMLHumanView::removeAllDelegates( void )
{
	IEventManager* pGlobalEventManager = IEventManager::instance();	
	// TODO - audio
	//pGlobalEventManager->VRemoveListener(MakeDelegate(this, &HumanView::PlaySoundDelegate), EvtData_PlaySound::sk_EventType);
}

