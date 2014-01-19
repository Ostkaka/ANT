#include <ant/graphicsSFML/SFMLRenderer.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace ant;

ant::SFMLRenderer::SFMLRenderer( sf::RenderWindow *window )
{
	m_window = window;
	m_textRenderer.reset(GCC_NEW SFMLTextRenderer(this));
}

void ant::SFMLRenderer::setBackgroundColor(BYTE bgA, BYTE bgR, BYTE bgG, BYTE bgB )
{
	m_color = sf::Color(bgR,bgG,bgB,bgA);
}

bool ant::SFMLRenderer::onRestore(void)
{
	return S_OK;
}

bool ant::SFMLRenderer::preRender(void)
{	
	// Clear render target?
	if (m_window)
	{		
		m_window->clear(m_color);
	}
	return true;
}

bool ant::SFMLRenderer::postRender(void)
{
	GCC_ASSERT(m_window);
	if (m_window)
	{
		m_window->display();
	}
	 
	return true;
}

void ant::SFMLRenderer::setView( sf::Vector2f pos, ant::Real angle, ant::Real zoomFactor )
{
	m_centerView.setCenter(pos);
	m_centerView.setRotation(float(angle));

	// Set the view size to window size also	
	m_centerView.setSize(float(m_window->getSize().x * zoomFactor),float(m_window->getSize().y * zoomFactor));
		
	m_window->setView(m_centerView);
}

void ant::SFMLRenderer::shutDown(void)
{
	// Do nothing
}

bool ant::SFMLRenderer::drawSprite( const sf::Sprite& sprite )
{
	GCC_ASSERT(m_window);
	// Draw the sprite in the window
	m_window->draw(sprite);
	return true;
}

bool ant::SFMLRenderer::drawRectangle( const sf::RectangleShape& rectangle )
{
	GCC_ASSERT(m_window);
	m_window->draw(rectangle);
	return true;
}

bool ant::SFMLRenderer::drawCircle( const sf::CircleShape& circle )
{
	GCC_ASSERT(m_window);
	m_window->draw(circle);
	return true;
}

bool ant::SFMLRenderer::drawText(const sf::Text& text) 
{
	GCC_ASSERT(m_window);
	m_window->draw(text);
	return true;
}

//////////////////////////////////////////////////////////////////////////
// SFMLTextRenderer
//////////////////////////////////////////////////////////////////////////
ant::SFMLTextRenderer::SFMLTextRenderer(SFMLRenderer* renderer) : m_renderer(renderer)
{
	// Create initial font
	std::string str = ANT_DATA_PATH"/assets/arial.ttf";
	if (!m_font.loadFromFile(str))
	{
		GCC_ERROR("Could not load font: " + str);
	}	
	m_fontSize = 30;
	m_activeColor = sf::Color(255, 255, 255);
}

ant::SFMLTextRenderer::~SFMLTextRenderer()
{
}

void ant::SFMLTextRenderer::renderText(const std::string& str)
{
	GCC_ASSERT(m_renderer);

	// Create text
	sf::String s(str);
	sf::Text text(s,m_font);
	text.setCharacterSize(m_fontSize);
	text.setPosition(m_textPos);
	text.setStyle(sf::Text::Bold);

	// render it
	m_renderer->drawText(text);
}
