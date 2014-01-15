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

		void setBackgroundColor(BYTE bgA, BYTE bgR, BYTE bgG, BYTE bgB ) ANT_OVERRIDE;
		
		bool onRestore(void) ANT_OVERRIDE;
		
		void shutDown(void) ANT_OVERRIDE;
		
		bool preRender(void) ANT_OVERRIDE;
		
		bool postRender(void) ANT_OVERRIDE;

		void setView(sf::Vector2f pos, ant::Real angle, ant::Real zoomFactor=1.0) ANT_OVERRIDE;

		virtual const sf::View& getView() ANT_OVERRIDE;

		/// Functions used to draw primitives on the active render target
		virtual bool drawSprite(const sf::Sprite& sprite) ANT_OVERRIDE;

		virtual bool drawRectangle(const sf::RectangleShape& rectangle) ANT_OVERRIDE;

		virtual bool drawCircle(const sf::CircleShape& circle) ANT_OVERRIDE;

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:
		sf::View		      m_centerView;
		sf::Color		      m_color;
		sf::RenderWindow *m_window;	
	};

	ANT_INLINE const sf::View& SFMLRenderer::getView()
	{
		GCC_ASSERT(m_window);
		return m_window->getView();
	}
}

#endif