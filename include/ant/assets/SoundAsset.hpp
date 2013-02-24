/**
 * Provides the Sound Asset type used by the AssetManager 
 */

#ifndef   SOUNDASSET_HPP
	#define   SOUNDASSET_HPP

#include <SFML/Audio.hpp>
#include <ant/interfaces/TAsset.hpp>
#include <ant/Core_types.hpp>

namespace ant
{
  /// Provides the Sound asset class
  class SoundAsset : public TAsset<sf::SoundBuffer>
  {
    public:
      /**
			 * Default constructor. Se parent class
       */
      SoundAsset();
			
      /**
       * SoundAsset constructor
       */
      SoundAsset(const typeAssetID assetID);

      /**
       * SoundAsset deconstructor
       */
      virtual ~SoundAsset();

  };
} 

#endif /*SOUNDASSET_HPP*/