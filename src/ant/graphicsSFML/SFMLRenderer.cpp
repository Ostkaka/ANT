#include <ant/graphicsSFML/SFMLRenderer.hpp>

using namespace ant;

ant::SFMLRenderer::SFMLRenderer( sf::RenderWindow *window )
{
	m_window = window;
}

void ant::SFMLRenderer::setBackgroundColor(BYTE bgA, BYTE bgR, BYTE bgG, BYTE bgB )
{
	m_color = sf::Color(bgR,bgG,bgB,bgA);
}

HRESULT ant::SFMLRenderer::onRestore(void)
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
		//m_window->display();
	}
	 
	return true;
}

void ant::SFMLRenderer::setView( sf::Vector2f pos, ant::Real angle )
{
	sf::View view;
	view.setCenter(pos);
	view.setRotation(float(angle));
	m_centerView = view;

	m_window->setView(view);
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


