#ifndef IRENDERER_HPP_
	#define IRENDERER_HPP_

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

	class I2DRenderer
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