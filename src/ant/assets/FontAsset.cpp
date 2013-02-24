#include <ant/assets/FontAsset.hpp>

namespace ant
{
	FontAsset::FontAsset():
	TAsset<sf::Font>()
	{
	}

	FontAsset::FontAsset(const typeAssetID assetID):
	TAsset<sf::Font>(assetID)
	{
	}

	FontAsset::~FontAsset()
	{
	}
};


