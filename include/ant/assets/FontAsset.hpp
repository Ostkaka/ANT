/**
 * Provides the Font Asset 
 *
 */
#ifndef   FONTASSET_HPP
	#define   FONTASSET_HPP

#include <SFML/Graphics.hpp>
#include <ant/interfaces/TAsset.hpp>
#include <ant/Core_types.hpp>

namespace ant
{
  /// Provides the Font asset class
  class FontAsset : public TAsset<sf::Font>
  {
    public:
      /**
       * FontAsset default constructor
       */
      FontAsset();

      /**
       * See parent class
       */
      FontAsset(const typeAssetID assetID);
			
			/**
       * FontAsset deconstructor
       */
      virtual ~FontAsset();

    protected:

    private:
  }; 
} 

#endif