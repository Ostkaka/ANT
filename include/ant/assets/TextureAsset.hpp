/*
* This class wraps the sf::Texture class for use in the game application
*/

#ifndef  TEXTUREASSET_HPP
	#define   TEXTUREASSET_HPP

#include <SFML/Graphics.hpp>
#include <ant/interfaces/TAsset.hpp>
#include <ant/Core_types.hpp>

namespace ant
{
	/// Asset class for wrapping sf::Texture
	class TextureAsset : public TAsset<sf::Texture>
	{
	public:
		/**
		* See template class description
		*/
		TextureAsset();
		TextureAsset(const typeAssetID assetID);

		/**
		* See template class description
		*/
		virtual ~TextureAsset();
	};
} 
#endif /*TEXTUREASSET_HPP*/