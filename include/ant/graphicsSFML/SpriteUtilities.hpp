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
	class SpriteSheetData;
	ANT_DECLARE_POINTER_TYPES(SpriteSheetData);
	class SpriteSheetData
	{
		public:
			typedef std::map<ant::UInt, sf::IntRect> FrameList;

			SpriteSheetData(FrameList list);

			sf::IntRect getFrame(ant::UInt frame);

			static SpriteSheetDataStrongPtr CreateSheetDataFromXML(TiXmlElement * data);

			//////////////////////////////////////////////////////////////////////////
			// Variables
			//////////////////////////////////////////////////////////////////////////
		protected:
			FrameList m_frameList;
	};	
}

#endif /*SPRITEUTILITIES_HPP_*/