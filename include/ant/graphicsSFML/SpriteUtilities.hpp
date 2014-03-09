#ifndef SPRITEUTILITIES_HPP_
	#define SPRITEUTILITIES_HPP_	

#include <ant/actors/ActorComponent.hpp>
#include <ant/luascripting/ScriptEvent.hpp>
#include <SFML/Graphics.hpp>
#include <map>

namespace ant
{
	typedef std::string AnimationId;

	/**
	* Class that holds information about sprite sheet data
	*/
	class SpriteSheetData
	{
	public:
		typedef std::map<ant::UInt, sf::FloatRect> FrameList;

		SpriteSheetData(FrameList list);

		sf::FloatRect getFrame(ant::UInt frame);

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:
		FrameList m_frameList;
	};

	SpriteSheetData CreateSheetDataFromXML(TiXmlElement * data);
}

#endif /*SPRITEUTILITIES_HPP_*/