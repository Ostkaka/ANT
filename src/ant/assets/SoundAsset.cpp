/**
 * Provides the Sound Asset type used by the AssetManager 
 */

#include <assert.h>
#include <stddef.h>
#include <ant/assets/SoundAsset.hpp>
#include <ant/loggers/Log.hpp>

namespace ant
{

	SoundAsset::SoundAsset()
	{

	}

	SoundAsset::SoundAsset( const typeAssetID assetID ):
	TAsset<sf::SoundBuffer>(assetID)
	{
	}

	SoundAsset::~SoundAsset()
	{
	}
}