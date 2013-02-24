/*
* This class wraps the sf::Texture class for use in the game application
*/

#include <assert.h>
#include <stddef.h>
#include <ant/assets/TextureAsset.hpp>

namespace ant
{

	ant::TextureAsset::TextureAsset():
	TAsset<sf::Texture>()
	{
	}

	ant::TextureAsset::TextureAsset(const typeAssetID assetID ):
		TAsset<sf::Texture>(assetID)
	{
	}

	ant::TextureAsset::~TextureAsset()
	{
	}

}
