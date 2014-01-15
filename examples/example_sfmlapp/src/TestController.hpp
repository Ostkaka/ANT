#ifndef TESTCONTROLLER_HPP_ 
	#define TESTCONTROLLER_HPP_

#include <ant/interfaces/InputHandlers.hpp>
#include <ant/core_types.hpp>
#include <SFML/Graphics.hpp>

class ant::SFMLSceneNode;

namespace ant
{
	class TestController : public IKeyboardHandler
	{	
	public:

		TestController(SFMLSceneNodeStrongPtr target);
		
		void onUpdate( ant::DeltaTime dt );
	
		virtual bool onKeyDown(const ant::CharCode c) ANT_OVERRIDE;

		virtual bool onKeyUp(const ant::CharCode c) ANT_OVERRIDE;

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:
		BYTE					          m_bKey[256];			// Which keys are up and down
		SFMLSceneNodeStrongPtr	m_object;
		sf::Vector2f						m_dir;
	};

	ANT_DECLARE_POINTER_TYPES(TestController)
}

#endif