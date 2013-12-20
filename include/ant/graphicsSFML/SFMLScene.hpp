#ifndef SFMLSCENE_HPP_
	#define SFMLSCENE_HPP_

#include <ant/core_types.hpp>
#include <ant/graphicsSFML/SFMLSceneNode.hpp>

namespace ant
{

	typedef std::map<ActorId, ISFMLSceneNodeStrongPtr> SFMLSceneActorMap;

	/**
	* This class contains and render SFML based scenes
	*/
	class SFMLScene 
	{
	public:
		/// Default constructor
		SFMLScene(ISFMLRendererStrongPtr renderer);

		/// Default destructor
		virtual ~SFMLScene();

		bool onRender(void);

		bool onRestore(void);
		
		bool onLostDevice(void);
		
		bool onUpdate(const ant::DeltaTime dt);

		ISFMLSceneNodeStrongPtr findActor(ActorId actor);

		bool addChild(ActorId id, ISFMLSceneNodeStrongPtr kid);
		
		bool removeChild(ActorId id);

		ISFMLRendererStrongPtr getRenderer() { return m_renderer; }

		// Event delegates
		void newRenderComponentDelegate(IEventDataStrongPtr eventData);
		void modifiedRenderComponentDelegate(IEventDataStrongPtr eventData);
		void destroyActorDelegate(IEventDataStrongPtr eventData);
		void moveActorDelegate(IEventDataStrongPtr eventData);

		void setCamera(SFMLCameraNodeStrongPtr camera);
		const SFMLCameraNodeStrongPtr getCamera(void) const;


		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:	
		SceneNodeList															m_sceneNodes;
		SFMLCameraNodeStrongPtr										m_camera;
		ISFMLRendererStrongPtr										m_renderer;
		SFMLSceneActorMap													m_actorMap;
		SFMLSceneNodeStrongPtr										m_root;
	};

	/// Implementation

	ANT_INLINE void SFMLScene::setCamera(SFMLCameraNodeStrongPtr camera) { m_camera = camera; }

	ANT_INLINE const SFMLCameraNodeStrongPtr SFMLScene::getCamera() const { return m_camera; }
}

#endif