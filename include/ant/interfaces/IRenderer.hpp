#ifndef IRENDERER_HPP_
	#define IRENDERER_HPP_

#include <ant/core_types.hpp>
#include <SFML/Graphics.hpp>

namespace ant
{

	class IRenderState
	{
	public:
		virtual std::string toString()=0;
	};

	/////////////////////////////////////////////////////////////////////////////
	//   SFML scenes are drawn in passes - this enum defines the render passes
	//   supported by the 2D scene graph created by class Scene.
	/////////////////////////////////////////////////////////////////////////////

	enum SFMLRenderPass
	{
		RenderPass_0,
		RenderPass_Static = RenderPass_0,
		RenderPass_BackGround,
		RenderPass_Actor,
		RenderPass_NotRendered,
		RenderPass_Last
	};

	//////////////////////////////////////////////////////////////////////////
	// SFML interface for a sfml based renderer
	//////////////////////////////////////////////////////////////////////////
	class ISFMLRenderer
	{
	public:		
		virtual void setBackgroundColor(BYTE bgA, BYTE bgR, BYTE bgG, BYTE bgB )=0;
		virtual bool onRestore()=0;
		virtual void shutDown()=0;
		virtual bool preRender()=0;
		virtual bool postRender()=0;
		virtual void setView(sf::Vector2f pos, ant::Real angle, ant::Real zoomFactor)=0;
		// Functions for drawing SFML primitives on the active window
		virtual bool drawSprite(const sf::Sprite& sprite)=0;
		virtual bool drawRectangle(const sf::RectangleShape& rectangle)=0;
		virtual bool drawCircle(const sf::CircleShape& circle)=0;
		virtual const sf::View& getView() = 0;
	};
}

#endif