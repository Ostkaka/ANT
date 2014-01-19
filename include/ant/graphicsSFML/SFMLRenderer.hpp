#ifndef SFMLRENDERER_HPP_
	#define SFMLRENDERER_HPP_

#include <ant/interfaces/IRenderer.hpp>
#include <SFML/Graphics.hpp>

namespace ant
{
	class SFMLRenderer;

	/**
	* Test renderer helper class for the SFML renderer
	*/
	class SFMLTextRenderer
	{

	public:
		SFMLTextRenderer(SFMLRenderer* renderer);

		virtual ~SFMLTextRenderer();

		void setTextPos(const sf::Vector2f& pos);

		void renderText(const std::string& str);

		void setTextSize(int size);

		void setTextColor(const sf::Color &color);

	protected:

	private:
		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:
		sf::Vector2f  m_textPos;
		sf::Font      m_font;
		sf::Color     m_activeColor;
		int           m_fontSize;
		SFMLRenderer* m_renderer;
	};

	ANT_INLINE void SFMLTextRenderer::setTextSize(int size){ m_fontSize = size; }

	ANT_INLINE void SFMLTextRenderer::setTextColor(const sf::Color& color){ m_activeColor = color; }

	ANT_INLINE void SFMLTextRenderer::setTextPos(const sf::Vector2f& pos){ m_textPos = pos; }

	ANT_DECLARE_POINTER_TYPES(SFMLTextRenderer)

	/**
	 * SFML Implementation of the I2DRenderer
	 */
	class SFMLRenderer : public ISFMLRenderer
	{
	public:

		SFMLRenderer(sf::RenderWindow *window);

		void setBackgroundColor(BYTE bgA, BYTE bgR, BYTE bgG, BYTE bgB) ANT_OVERRIDE;

		bool onRestore(void) ANT_OVERRIDE;

		void shutDown(void) ANT_OVERRIDE;

		bool preRender(void) ANT_OVERRIDE;

		bool postRender(void) ANT_OVERRIDE;

		void setView(sf::Vector2f pos, ant::Real angle, ant::Real zoomFactor = 1.0) ANT_OVERRIDE;

		virtual const sf::View& getView() ANT_OVERRIDE;

		/// Functions used to draw primitives on the active render target
		virtual bool drawSprite(const sf::Sprite& sprite) ANT_OVERRIDE;

		virtual bool drawRectangle(const sf::RectangleShape& rectangle) ANT_OVERRIDE;

		virtual bool drawCircle(const sf::CircleShape& circle) ANT_OVERRIDE;

		virtual bool drawText(const sf::Text& text) ANT_OVERRIDE;

		SFMLTextRenderer* getTextRenderer();

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:
		sf::View		          m_centerView;
		sf::Color		          m_color;
		sf::RenderWindow*         m_window;
		SFMLTextRendererStrongPtr m_textRenderer;
	};

	ANT_INLINE const sf::View& SFMLRenderer::getView()
	{
		GCC_ASSERT(m_window);
		return m_window->getView();
	}

	ANT_INLINE SFMLTextRenderer* SFMLRenderer::getTextRenderer() { return m_textRenderer.get(); }
}

#endif