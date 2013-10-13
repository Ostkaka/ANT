#ifndef SFMLSCENENODE_HPP_
	#define SFMLSCENENODE_HPP_

#include <ant/core_types.hpp>
#include <ant/graphicsSFML/ISFMLSceneNode.hpp>
#include <ant/interfaces/IRenderer.hpp>
#include <SFML/Graphics.hpp>
#include <ant/graphicsSFML/SFMLRenderComponent.hpp>

namespace ant
{

	typedef std::vector<shared_ptr<ISFMLSceneNode> > SFMLSceneNodeList;

	/**
	 * Information coupled to a scene node
	 */
	class SFMLSceneNodeProperties 
	{
		friend class SFMLSceneNode;

	public:

		SFMLSceneNodeProperties(void);

		const ActorId &getActorId() const;

		sf::Vector2f getPosition() const;

		ant::Real getRotation() const;

		void setPosition(const sf::Vector2f& pos);

		void setRotation(const ant::Real& rot);

		const char* getName() const;

		SFMLRenderPass renderPass() const;

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:
		ActorId							m_actorId;
		std::string					m_name;
		SFMLRenderPass			m_renderPass;
		sf::Vector2f				m_position;
		ant::Real						m_rotation;
	};

	/// Implementation
	ANT_INLINE const ActorId& SFMLSceneNodeProperties::getActorId() const { return m_actorId; }

	ANT_INLINE SFMLRenderPass SFMLSceneNodeProperties::renderPass() const { return m_renderPass; }

	ANT_INLINE const char* SFMLSceneNodeProperties::getName() const { return m_name.c_str(); }

	ANT_INLINE sf::Vector2f SFMLSceneNodeProperties::getPosition() const { return m_position; }

	ANT_INLINE ant::Real SFMLSceneNodeProperties::getRotation() const { return m_rotation; }

	ANT_INLINE void SFMLSceneNodeProperties::setPosition(const sf::Vector2f& pos) { m_position = pos; }

	ANT_INLINE void SFMLSceneNodeProperties::setRotation(const ant::Real& rot) { m_rotation = rot; }

	//////////////////////////////////////////////////////////////////////////
	// Scene Node List
	//////////////////////////////////////////////////////////////////////////
	typedef std::vector<ISFMLSceneNodeStrongPtr > SceneNodeList;
	
	/**
	 * Basic implementation of the scene node
	 */
	class SFMLSceneNode : public ISFMLSceneNode
	{
		friend class SFMLScene;

	public:
		SFMLSceneNode(ActorId actorId, SFMLBaseRenderComponentWeakPtr renderComponent, 
			SFMLRenderPass renderPass, 
			const sf::Vector2f& pos, 
			const ant::Real& rot );
		
		virtual ~SFMLSceneNode();

		virtual const SFMLSceneNodeProperties * const getNodeProps() const ANT_OVERRIDE;

		virtual HRESULT onUpdate(SFMLScene *scene, ant::DeltaTime dt) ANT_OVERRIDE;

		virtual HRESULT onRestore(SFMLScene *scene) ANT_OVERRIDE;

		virtual HRESULT preRender(SFMLScene *scene) ANT_OVERRIDE;

		virtual HRESULT render(SFMLScene *scene) ANT_OVERRIDE; 

		virtual HRESULT postRender(SFMLScene *scene) ANT_OVERRIDE;

		virtual bool isVisible(SFMLScene *scene) const ANT_OVERRIDE;

		virtual HRESULT onLostDevice(SFMLScene *scene) ANT_OVERRIDE;

		// This support child nodes. Is this even necessary?
		virtual HRESULT renderChildren(SFMLScene *scene) ANT_OVERRIDE; 
		 
		virtual bool addChild(ISFMLSceneNodeStrongPtr ikid) ANT_OVERRIDE;

		virtual bool removeChild(ActorId id) ANT_OVERRIDE;
		 
		//virtual HRESULT pick()=0; // This can be done with just simple coords

		virtual void setPosition(const sf::Vector2f& pos) ANT_OVERRIDE;

		virtual void setRotation(const ant::Real& rot) ANT_OVERRIDE;

		virtual sf::Vector2f getPosition() ANT_OVERRIDE;

		virtual ant::Real getRotation() ANT_OVERRIDE;

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:
		SFMLSceneNodeProperties					m_props;
		SFMLBaseRenderComponentWeakPtr  m_baseRenderComponent;
		SFMLSceneNodeList								m_Children;
		SFMLSceneNode										*m_pParent;
	};

	// Implementation
	ANT_INLINE const SFMLSceneNodeProperties * const SFMLSceneNode::getNodeProps() const { return &m_props; }

	ANT_INLINE sf::Vector2f SFMLSceneNode::getPosition() { return m_props.getPosition(); }

	ANT_INLINE ant::Real SFMLSceneNode::getRotation() { return m_props.getRotation(); }
	
	ANT_INLINE void SFMLSceneNode::setPosition(const sf::Vector2f& pos) { m_props.setPosition(pos); }

	ANT_INLINE void SFMLSceneNode::setRotation(const ant::Real& rot) { return m_props.setRotation(rot); }

	/**
	 * A scene node
	 */
	class SFMLRootNode : public SFMLSceneNode	
	{
	public:
		SFMLRootNode();
		virtual bool addChild(ISFMLSceneNodeStrongPtr kid);
		virtual HRESULT renderChildren(SFMLScene *pScene);
		virtual bool removeChild(ActorId id);
		virtual bool isVisible(SFMLScene *pScene) const;
	};

	// Implementation
	ANT_INLINE bool SFMLRootNode::isVisible(SFMLScene *pScene) const {return true;}
	
	/**
	 * A scene node that functions as a camera, setting the view of the scene according to a position
	 */
	class SFMLCameraNode : public SFMLSceneNode
	{
		public:
			SFMLCameraNode(const sf::Vector2f& position, const ant::Real& rot)
				: SFMLSceneNode(INVALID_ACTOR_ID, NULL, RenderPass_0, position, rot)
			{
			}

			virtual HRESULT render(SFMLScene *scene) ANT_OVERRIDE; 		

			virtual HRESULT onRestore(SFMLScene *scene) ANT_OVERRIDE;

			virtual bool isVisible(SFMLScene *pScene) const ANT_OVERRIDE; //inline

			void setTarget(SFMLSceneNodeStrongPtr pTarget); //inline
			
			void clearTarget(); //inline
			
			SFMLSceneNodeStrongPtr getTarget(); // inline

			HRESULT setView(SFMLScene * pScene); 

			void setCameraOffset(const sf::Vector2f& offset);

			//////////////////////////////////////////////////////////////////////////
			// Variables
			//////////////////////////////////////////////////////////////////////////
	protected:
		shared_ptr<SFMLSceneNode>			m_target;
		bool													m_active;		
		sf::Vector2f									m_cameraOffset;
	};

	// Implementation
	ANT_INLINE bool SFMLCameraNode::isVisible(SFMLScene *pScene) const { return true; }

	ANT_INLINE void SFMLCameraNode::setTarget(SFMLSceneNodeStrongPtr pTarget){ m_target = pTarget;}

	ANT_INLINE SFMLSceneNodeStrongPtr SFMLCameraNode::getTarget(){ return m_target; }

	ANT_INLINE void SFMLCameraNode::clearTarget(){ m_target=SFMLSceneNodeStrongPtr(); }

	ANT_INLINE void SFMLCameraNode::setCameraOffset(const sf::Vector2f& offset) { m_cameraOffset = offset; }

	/**
	 * Node that contains and draws a sprite 
	 */
	class SFMLSpriteNode : public SFMLSceneNode
	{
	public:
		SFMLSpriteNode(ActorId actorId, 
			SFMLBaseRenderComponentWeakPtr renderComponent, 
			const std::string& textureName,
			SFMLRenderPass renderPass, 
			const sf::Vector2f& pos, 
			const ant::Real& rot);

		virtual HRESULT render(SFMLScene *scene) ANT_OVERRIDE; 		

		virtual HRESULT onRestore(SFMLScene *scene) ANT_OVERRIDE;

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:
		std::string m_textureName;
		sf::Texture m_texture;
		sf::Sprite m_SFMLSprite;
	};

}

#endif