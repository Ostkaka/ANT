#ifndef _CAREA_H_
    #define _CAREA_H_

#include <SFML/Graphics.hpp>
#include "antTile/CMap.h"
#include <ant/core_types.hpp>
#include <ant/assets/TextureAsset.hpp>

class CArea {
		public:
        std::vector<CMap> mapList;

    public:
        CArea();
				~CArea();
        bool    CArea::onLoad(const std::string & file, int sizeX, int sizeY);
				void    onRender(sf::RenderWindow & window, const sf::Vector2f & cameraPos);
        void    onCleanup();

    public:
			CMap * getMap(int X, int Y);
			CTile *	getTile(int X, int Y);
			static CArea *			areaControl;
			sf::Vector2i				mMapSize;

		protected:
			int									areaSize;
			ant::TextureAsset		mTilesetTexture;

};

#endif
