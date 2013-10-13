#ifndef SFMLRENDERER_HPP_
	#define SFMLRENDERER_HPP_

#include <ant/interfaces/IRenderer.hpp>

namespace ant
{

	/**
	 * SFML Implementation of the I2DRenderer
	 */
	class SFMLRenderer : public ISFMLRenderer
	{
	public:

		SFMLRenderer(sf::RenderWindow *window);

		void setBackgroundColor(BYTE bgA, BYTE bgR, BYTE bgG, BYTE bgB );
		
		HRESULT onRestore(void);
		
		void shutDown(void);
		
		bool preRender(void);
		
		bool postRender(void);

		void setView(sf::Vector2f pos, ant::Real angle);

		/// Functions used to draw primitives on the active render target
		virtual bool drawSprite(const sf::Sprite& sprite);

		virtual bool drawRectangle(const sf::RectangleShape& rectangle);

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:
		sf::View		m_centerView;
		sf::Color		m_color;
		sf::RenderWindow *m_window;
	};
}

#endif