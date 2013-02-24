#ifndef _CMAP_H_
    #define _CMAP_H_

#include <vector>
#include "CTile.h"
#include <SFML/Graphics.hpp>
#include <ant/core_types.hpp>
#include <ant/assets/TextureAsset.hpp>

class CMap {
    public:
        CMap();
				~CMap();
    public:
      bool onLoad(const std::string & File,int sizeX, int sizeY);

			void onRender(sf::RenderWindow  & window, const sf::Vector2f & mapPos);
		
			void setTexture(ant::TextureAsset& tileSet);

    public:
        CTile* getTile(int X, int Y);

		protected:
			std::vector<CTile> TileList;
			ant::TextureAsset  mapTileset;
			sf::Sprite				 mapSprite;
			sf::Vector2i			 mSize;
};

#endif
