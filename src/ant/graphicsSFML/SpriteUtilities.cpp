#include <ant/graphicsSFML/SpriteUtilities.hpp>

using namespace ant;

ant::SpriteSheetData::SpriteSheetData(FrameList list)
{
	m_frameList = list;
}

sf::FloatRect SpriteSheetData::getFrame(ant::UInt frame)
{
	if (m_frameList.find(frame) != m_frameList.end())
	{
		return m_frameList[frame];
	}
	else
	{
		GCC_WARNING("Could not find frame: " + ToStr(frame) + " in framelist");
	}
}

SpriteSheetData CreateSheetDataFromXML(TiXmlElement * data)
{
	// Try to parse the rectangles in the file

	// Each frame has an attribute "id"
		// Childs are parsed as x,y,x2,y2

	SpriteSheetData::FrameList frameList;

	return SpriteSheetData(frameList);
}