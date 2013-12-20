#ifndef USERINTERFACE_HPP_
	#define USERINTERFACE_HPP_

#include <ant/core_types.hpp>
#include <ant/interfaces/IScreenElement.hpp>
#include <ant/graphicsSFML/SFMLScene.hpp>

namespace ant
{
	/**
	 * Class that handles the drawing of the scene on the human view
	 */
	class ScreenElementSFMLScene : public IScreenElement, public SFMLScene
	{
	public:
		ScreenElementSFMLScene(shared_ptr<ISFMLRenderer> renderer) : SFMLScene(renderer) { }
		virtual ~ScreenElementSFMLScene(void)
		{
			GCC_WARNING("~ScreenElementSFMLScene()");
		}

		// IScreenElement Implementation
		virtual void onUpdate(ant::DeltaTime dt) ANT_OVERRIDE
		{ 
			SFMLScene::onUpdate(dt); 
		}

		virtual bool onRestore() ANT_OVERRIDE  
		{ 
			SFMLScene::onRestore(); 
			return true; 
		}

		virtual bool onRender(ant::DeltaTime fTime, ant::DeltaTime fElapsedTime) ANT_OVERRIDE 
		{ 
			SFMLScene::onRender(); 
			return true; 
		}

		virtual bool onLostDevice() ANT_OVERRIDE 
		{ 
			SFMLScene::onLostDevice(); 
			return true; 
		}

		// Don't handle any messages
		virtual LRESULT CALLBACK onMsgProc( sf::Event theEvent ) ANT_OVERRIDE 
		{ 
			return 0; 
		}

		virtual bool isVisible() const ANT_OVERRIDE 
		{ 
			return true; 
		}

		virtual int getZOrder() const { return 0; }

		virtual void setZOrder(int const zOrder) { }

		virtual void setVisible(bool visible) ANT_OVERRIDE { }

		virtual bool addChild(ActorId id, shared_ptr<ISFMLSceneNode> kid) 
		{ 
			return SFMLScene::addChild(id, kid); 
		}
	};
}
#endif
