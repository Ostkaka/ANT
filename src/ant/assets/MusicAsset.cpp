#include <assert.h>
#include <stddef.h>
#include <ant/assets/MusicAsset.hpp>

namespace ant
{
	MusicAsset::MusicAsset():
	TAsset<sf::Music>()
	{
	}

	MusicAsset::MusicAsset( const typeAssetID assetID ):
	TAsset<sf::Music>(assetID)
	{
	}

	MusicAsset::~MusicAsset()
	{
	}
}