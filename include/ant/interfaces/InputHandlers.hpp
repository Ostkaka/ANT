#ifndef INPUTHANDLERS_HPP_
	#define INPUTHANDLERS_HPP_

#include <ant/core_types.hpp>
#include <SFML/Window.hpp>

namespace ant	
{
	/**
	* Handler for capturing user input from devices
	*/ 
	class IKeyboardHandler
	{
	public:	

		virtual bool onKeyDown(const ant::CharCode c)=0;

		virtual bool onKeyUp(const ant::CharCode c)=0;
	};

	/**
	 * Converts an sf enumerate keycode to an ant::CharCode
	 */
	static ant::CharCode convertSFMLKeyCodeToCharCode(int keyCode)
	{
		switch(keyCode)
		{
		case sf::Keyboard::A: 
			return 'A';
			break;
		case sf::Keyboard::B:
			return 'B';
			break;
		case sf::Keyboard::C:
			return 'C';
			break;
		case sf::Keyboard::D:
			return 'D';
			break;
		case sf::Keyboard::E:
			return 'E';
			break;
		case sf::Keyboard::F:
			return 'F';
			break;
		case sf::Keyboard::G:
			return 'G';
			break;
		case sf::Keyboard::H:
			return 'H';
			break;
		case sf::Keyboard::I:
			return 'I';
			break;
		case sf::Keyboard::J:
			return 'J';
			break;
		case sf::Keyboard::K:
			return 'K';
			break;
		case sf::Keyboard::L:
			return 'L';
			break;
		case sf::Keyboard::M:
			return 'M';
			break;
		case sf::Keyboard::N:
			return 'N';
			break;
		case sf::Keyboard::O:
			return 'O';
			break;
		case sf::Keyboard::P:
			return 'P';
			break;
		case sf::Keyboard::Q:
			return 'Q';
			break;
		case sf::Keyboard::R:
			return 'R';
			break;
		case sf::Keyboard::S:
			return 'S';
			break;
		case sf::Keyboard::T:
			return 'T';
			break;
		case sf::Keyboard::U:
			return 'U';
			break;
		case sf::Keyboard::V:
			return 'V';
			break;
		case sf::Keyboard::W:
			return 'W';
			break;
		case sf::Keyboard::X:
			return 'X';
			break;
		case sf::Keyboard::Y:
			return 'Y';
			break;
		case sf::Keyboard::Z:
			return 'Z';
			break;
		default:
			return '0';
			break;
		}
	}
}

#endif