#ifndef SFMLSCENENODE_HPP_
	#define SFMLSCENENODE_HPP_

#include <ant/core_types.hpp>
#include <ant/graphicsSFML/ISFMLSceneNode.hpp>
#include <ant/interfaces/IRenderer.hpp>
#include <SFML/Graphics.hpp>
#include <ant/graphicsSFML/SFMLRenderComponent.hpp>
#include <ant/actors/AnimationComponent.hpp>

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

		void setPosition(const sf::Vector2f& pos);

		const sf::Vector2f& getPosition() const;
		
		void setRotation(const ant::Real& rot);
		
		ant::Real getRotation() const;

		void setDirection(const sf::Vector2f& pos);

		const sf::Vector2f& getDirection() const;

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
		sf::Vector2f				m_direction;
		ant::Real						m_rotation;
	};

	/// Implementation
	ANT_INLINE const ActorId& SFMLSceneNodeProperties::getActorId() const { return m_actorId; }

	ANT_INLINE SFMLRenderPass SFMLSceneNodeProperties::renderPass() const { return m_renderPass; }

	ANT_INLINE const char* SFMLSceneNodeProperties::getName() const { return m_name.c_str(); }

	ANT_INLINE const sf::Vector2f& SFMLSceneNodeProperties::getPosition() const { return m_position; }

	ANT_INLINE ant::Real SFMLSceneNodeProperties::getRotation() const { return m_rotation; }

	ANT_INLINE void SFMLSceneNodeProperties::setPosition(const sf::Vector2f& pos) { m_position = pos; }

	ANT_INLINE void SFMLSceneNodeProperties::setRotation(const ant::Real& rot) { m_rotation = rot; }

	ANT_INLINE void SFMLSceneNodeProperties::setDirection(const sf::Vector2f& dir) { m_direction = dir; }

	ANT_INLINE const sf::Vector2f& SFMLSceneNodeProperties::getDirection() const { return m_direction; }

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

		virtual bool onUpdate(SFMLScene *scene, ant::DeltaTime dt) ANT_OVERRIDE;

		virtual bool onRestore(SFMLScene *scene) ANT_OVERRIDE;

		virtual bool preRender(SFMLScene *scene) ANT_OVERRIDE;

		virtual bool render(SFMLScene *scene) ANT_OVERRIDE; 

		virtual bool postRender(SFMLScene *scene) ANT_OVERRIDE;

		virtual bool isVisible(SFMLScene *scene) const ANT_OVERRIDE;

		virtual bool onLostDevice(SFMLScene *scene) ANT_OVERRIDE;

		// This support child nodes. Is this even necessary?
		virtual bool renderChildren(SFMLScene *scene) ANT_OVERRIDE; 
		 
		virtual bool addChild(ISFMLSceneNodeStrongPtr ikid) ANT_OVERRIDE;

		virtual bool removeChild(ActorId id) ANT_OVERRIDE;
		 
		//virtual bool pick()=0; // This can be done with just simple coords

		virtual void setPosition(const sf::Vector2f& pos) ANT_OVERRIDE;

		virtual const sf::Vector2f& getPosition() ANT_OVERRIDE;

		virtual void setDirection(const sf::Vector2f& pos) ANT_OVERRIDE;

		virtual const sf::Vector2f& getDirection() ANT_OVERRIDE;

		virtual void setRotation(const ant::Real& rot) ANT_OVERRIDE;

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

	ANT_INLINE void SFMLSceneNode::setPosition(const sf::Vector2f& pos) { m_props.setPosition(pos); }

	ANT_INLINE const sf::Vector2f& SFMLSceneNode::getPosition() { return m_props.getPosition(); }

	ANT_INLINE ant::Real SFMLSceneNode::getRotation() { return m_props.getRotation(); }

	ANT_INLINE void SFMLSceneNode::setRotation(const ant::Real& rot) { return m_props.setRotation(rot); }

	ANT_INLINE void SFMLSceneNode::setDirection(const sf::Vector2f& pos) { m_props.setDirection(pos); }

	ANT_INLINE const sf::Vector2f& SFMLSceneNode::getDirection() { return m_props.getDirection(); }

	/**
	 * A scene node
	 */
	class SFMLRootNode : public SFMLSceneNode	
	{
	public:
		SFMLRootNode();
		virtual bool addChild(ISFMLSceneNodeStrongPtr kid);
		virtual bool renderChildren(SFMLScene *pScene);
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
				m_zoomFactor = 1.0;
			}

			virtual bool render(SFMLScene *scene) ANT_OVERRIDE; 		

			virtual bool onRestore(SFMLScene *scene) ANT_OVERRIDE;

			virtual bool isVisible(SFMLScene *pScene) const ANT_OVERRIDE; //inline

			void setTarget(SFMLSceneNodeStrongPtr pTarget); //inline
			
			void clearTarget(); //inline
			
			SFMLSceneNodeStrongPtr getTarget(); // inline

			bool setView(SFMLScene * pScene); 

			void setCameraOffset(const sf::Vector2f& offset);

			void setCameraZoom(const ant::Real& zoomFactor);

			//////////////////////////////////////////////////////////////////////////
			// Variables
			//////////////////////////////////////////////////////////////////////////
	protected:
		shared_ptr<SFMLSceneNode>			m_target;
		bool													m_active;		
		sf::Vector2f									m_cameraOffset;
		ant::Real                     m_zoomFactor;
	};

	// Implementation
	ANT_INLINE bool SFMLCameraNode::isVisible(SFMLScene *pScene) const { return true; }

	ANT_INLINE void SFMLCameraNode::setTarget(SFMLSceneNodeStrongPtr pTarget){ m_target = pTarget;}

	ANT_INLINE SFMLSceneNodeStrongPtr SFMLCameraNode::getTarget(){ return m_target; }

	ANT_INLINE void SFMLCameraNode::clearTarget(){ m_target=SFMLSceneNodeStrongPtr(); }

	ANT_INLINE void SFMLCameraNode::setCameraOffset(const sf::Vector2f& offset) { m_cameraOffset = offset; }

	ANT_INLINE void SFMLCameraNode::setCameraZoom(const ant::Real& zoomFactor) { m_zoomFactor = zoomFactor; }

	/**
	 * Node that contains and draws a sprite 
	 */
	class SFMLSpriteNode : public SFMLSceneNode
	{
	public:
		SFMLSpriteNode(ActorId actorId, 
			SFMLBaseRenderComponentWeakPtr renderComponent, 
			const std::string& textureName,
			const sf::IntRect& textureRect,	
			const ant::Real& scale,
			SFMLRenderPass renderPass, 
			const sf::Vector2f& pos, 
			const ant::Real& rot);

		virtual bool render(SFMLScene *scene) ANT_OVERRIDE; 		

		virtual bool onRestore(SFMLScene *scene) ANT_OVERRIDE;

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:
		std::string m_textureName;
		sf::Texture m_texture;
		sf::Sprite  m_SFMLSprite;
		ant::Real   m_scale;
	};

	/**
	* Nodes that contains information about a sprite that can be animated
	*/
	class SFMLAnimatedSpriteNode : public SFMLSceneNode
	{
	public:
		SFMLAnimatedSpriteNode(ActorId actorId,
			SFMLBaseRenderComponentWeakPtr renderComponent,
			const std::string& textureName,
			const std::string& spriteDataName,
			SFMLRenderPass renderPass,
			const sf::Vector2f& pos,
			const ant::Real& rot);

		virtual bool render(SFMLScene * scene) ANT_OVERRIDE;

		virtual bool onRestore(SFMLScene * scene) ANT_OVERRIDE;

	protected:
		std::string              m_textureName;
		sf::Texture              m_texture;
		SpriteSheetDataStrongPtr m_spriteSheetData;
		sf::Sprite               m_SFMLSprite;
		ant::Real                m_scale;
	};

	/**
	 * Node that contains and draws a background sprite 
	 */
	class SFMLBackgroundSpriteNode : public SFMLSceneNode
	{
	public:
		SFMLBackgroundSpriteNode(ActorId actorId, 
			SFMLBaseRenderComponentWeakPtr renderComponent, 
			const std::string& textureName,
			SFMLRenderPass renderPass, 
			const sf::Vector2f& pos, 
			const ant::Real& rot);

		virtual bool render(SFMLScene *scene) ANT_OVERRIDE; 		

		virtual bool onRestore(SFMLScene *scene) ANT_OVERRIDE;

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:
		std::string m_textureName;
		sf::Texture m_texture;
		sf::Sprite  m_SFMLSprite;
		ant::Real   m_scale;
	};

	/**
	 * Rectangle primitive drawn with SFML
	 */
	class SFMLRectanglePrimitiveNode : public SFMLSceneNode
	{
	public:
		SFMLRectanglePrimitiveNode(ActorId actorId, 
			SFMLBaseRenderComponentWeakPtr renderComponent, 
			const sf::Vector2f& size,
			const bool& filled,
			SFMLRenderPass renderPass, 
			const sf::Vector2f& pos, 
			const ant::Real& rot);

		virtual bool render(SFMLScene *scene) ANT_OVERRIDE; 		

		virtual bool onRestore(SFMLScene *scene) ANT_OVERRIDE;

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:
		sf::Vector2f       m_size;
		bool               m_filled;
		sf::RectangleShape m_rectangleShape;
	};

	/**
	 * Circle primitive drawn with SFML
	 */
	class SFMLCirclePrimitiveNode : public SFMLSceneNode
	{
	public:
		SFMLCirclePrimitiveNode(ActorId actorId, 
			SFMLBaseRenderComponentWeakPtr renderComponent, 
			const ant::Real& size,
			const bool& filled,
			SFMLRenderPass renderPass, 
			const sf::Vector2f& pos, 
			const ant::Real& rot);

		virtual bool render(SFMLScene *scene) ANT_OVERRIDE; 		

		virtual bool onRestore(SFMLScene *scene) ANT_OVERRIDE;

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:
		ant::Real          m_radius;
		bool               m_filled;
		sf::CircleShape    m_circleShape;
	};
}

#endif