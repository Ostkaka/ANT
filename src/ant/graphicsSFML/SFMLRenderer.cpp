#include <ant/graphicsSFML/SFMLRenderer.hpp>
#include <iostream>

using namespace ant;

ant::SFMLRenderer::SFMLRenderer( sf::RenderWindow *window )
{
	m_window = window;
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

void ant::SFMLRenderer::setView( sf::Vector2f pos, ant::Real angle )
{
	m_centerView.setCenter(pos);
	m_centerView.setRotation(float(angle));

	// Set the view size to window size also	
	m_centerView.setSize(float(m_window->getSize().x),float(m_window->getSize().y));
		
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
