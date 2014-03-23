#include <ant/graphicsSFML/SpriteUtilities.hpp>

using namespace ant;

bool parseXMLReal(TiXmlElement * node, const char * name, Real& val)
{
	TiXmlElement * child = nullptr;
	if ((child = node->FirstChildElement(name)) != nullptr)
	{
		val = Real(std::stof(child->FirstChild()->Value()));
	}
	return child;
}

ant::SpriteSheetData::SpriteSheetData(FrameList list)
{
	m_frameList = list;
}

SpriteSheetDataStrongPtr SpriteSheetData::CreateSheetDataFromXML(TiXmlElement* data)
{
	SpriteSheetData::FrameList frameList;

	// Try to parse the rectangles in the file
	if (data)
	{
		// Loop over every child in the root
		for (TiXmlElement * node = data->FirstChildElement(); node; node = node->NextSiblingElement())
		{
			// Each frame has an attribute "id"
			// Childs are parsed as x,y,x2,y	

			// Is this child a frame
			if (node->Value() == std::string("Frame"))
			{
				// Get id attribute
				int id = -1;
				if (node->Attribute("id"))
				{
					id = std::atoi(node->Attribute("id"));
				}
				else
				{
					GCC_WARNING("Could not parse id from frame.");
				}

				bool succ = false;
				// Get childs 
				Real x = 0, y = 0, x1 = 0, y1 = 0;
				succ = parseXMLReal(node, "x1", x);
				succ = parseXMLReal(node, "y1", y);
				succ = parseXMLReal(node, "x2", x1);
				succ = parseXMLReal(node, "y2", y1);

				if (succ)
				{
					sf::IntRect rect((int)x, (int)y, (int)(x1 - x), (int)(y1 - y));
					frameList[id] = rect;
				}
				else
				{
					GCC_WARNING("Failed to parse frame in Spritesheetdata");
				}
			}
			else
			{
				GCC_WARNING("Parsing a child in sprite sheet that is not a frame");
			}
		}

		return SpriteSheetDataStrongPtr(GCC_NEW SpriteSheetData(frameList));
	}
	return nullptr;
}

sf::IntRect SpriteSheetData::getFrame(ant::UInt frame)
{
	if (m_frameList.find(frame) != m_frameList.end())
	{
		return m_frameList[frame];
	}
	else
	{
		GCC_WARNING("Could not find frame: " + ToStr(frame) + " in framelist");
		return sf::IntRect();
	}
}

