/*
* Derived class from IAssetHandler for managing sf:Music
*/

#ifndef   MUSICHANDLER_HPP
	#define   MUSICHANDLER_HPP

#include <SFML/Audio.hpp>
#include <ant/Core_types.hpp>
#include <ant/interfaces/TAssetHandler.hpp>

namespace ant
{
	/// TextureHandler manages the textures used in the game
	class MusicHandler : public TAssetHandler<sf::Music>
	{
	public:
		/*
		* Basic constructor
		*/
		MusicHandler();

		/*
		* Basic deconstructor
		*/
		virtual ~MusicHandler();
	
	protected:

		/*
		* See syntax from derived class
		*/
		virtual bool loadFromFile(const typeAssetID assetID, sf::Music& asset);

	private:

	};
}

#endif /*TEXTUREHANDLER_HPP*/