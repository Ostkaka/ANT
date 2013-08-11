#ifndef IRENDERER_HPP_
	#define IRENDERER_HPP_

#include <ant/core_types.hpp>

namespace ant
{

	class IRenderState
	{
	public:
		virtual std::string toString()=0;
	};

	/**
	 * Abstract interface for declaring a renderer
	 */
	class I3DRenderer
	{
	public:
		virtual void setBackgroundColor(BYTE bgA, BYTE bgR, BYTE bgG, BYTE bgB )=0;
		virtual HRESULT onRestore()=0;
		virtual void shutDown()=0;
		virtual bool preRender()=0;
		virtual bool postRender()=0;
		virtual void calcLightning()=0;
		virtual void setWorldTransform(const Mat4x4 *m)=0;
		virtual void setViewTransform(const Mat4x4 *m)=0;
		virtual void setProjectionTransform(const Mat4x4 *m)=0;
		virtual shared_ptr<IRenderState> prepareAlphaPass()=0;
		virtual shared_ptr<IRenderState> prepareSkyBoxPass()=0;
		virtual void drawLine(const Vec3& from,const Vec3& to,const Color& color)=0;
	};

	/////////////////////////////////////////////////////////////////////////////
	//   SFML scenes are drawn in passes - this enum defines the render passes
	//   supported by the 3D scene graph created by class Scene.
	/////////////////////////////////////////////////////////////////////////////

	enum SFMLRenderPass
	{
		RenderPass_0,
		RenderPass_Static = RenderPass_0,
		RenderPass_Actor,
		RenderPass_BackGround,
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
		virtual HRESULT onRestore()=0;
		virtual void shutDown()=0;
		virtual bool preRender()=0;
		virtual bool postRender()=0;
		/*virtual void calcLightning()=0;
		virtual void setWorldTransform(const Mat4x4 *m)=0;
		virtual void setViewTransform(const Mat4x4 *m)=0;
		virtual void setProjectionTransform(const Mat4x4 *m)=0;
		virtual shared_ptr<IRenderState> prepareAlphaPass()=0;
		virtual shared_ptr<IRenderState> prepareSkyBoxPass()=0;
		virtual void drawLine(const Vec3& from,const Vec3& to,const Color& color)=0;*/
	};
}

#endif