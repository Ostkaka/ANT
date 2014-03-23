#ifndef SFMLRENDERCOMPONENT_HPP
	#define SFMLRENDERCOMPONENT_HPP

#include <ant/graphicsSFML/ISFMLRenderComponent.hpp>
#include <SFML/Graphics.hpp>
#include <ant/core_types.hpp>
#include <ant\graphicsSFML\SpriteUtilities.hpp>

namespace ant
{
	/**
	 * Base render component that all SFML based render components are derived from
	 */
	class SFMLBaseRenderComponent : public ISFMLRenderComponent
	{

	public:
		/// Inits component from XML data
		virtual bool init(TiXmlElement* pData);

		/// post init
		virtual void postInit(void);

		/// updates the components and it's scene node from changes
		virtual void onChanged(void);

		/// Generates XML description from the component
		virtual TiXmlElement* generateXml(void);

		/// Get the color from the component
		const sf::Color& getColor(void) const;

	protected:

		sf::Color loadColor(TiXmlElement * pColorData);

		//// Loads data to the scene node from the <SceneNode> tag
		virtual bool delegateInit(TiXmlElement *data) { return true; }

		/// Factory method for creating scene node. Implemented for each sub component
		virtual SFMLSceneNodeStrongPtr createSceneNode(void)=0;

		/// Functions that subclasses implement that extends XML generation for child-classes
		virtual void createInheritedXmlElements(TiXmlElement* pBaseElement) = 0;

	private:

		virtual SFMLSceneNodeStrongPtr getSceneNode(void);

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:
		SFMLSceneNodeStrongPtr m_pSceneNode;
		sf::Color m_color;
	};

	ANT_INLINE const sf::Color& SFMLBaseRenderComponent::getColor(void) const { return m_color; }

	/**
	 * Extends the base SFML render component with a sprite render component
	 */
	class SFMLSpriteComponent : public SFMLBaseRenderComponent
	{
		friend class SFMLSpriteNode;

	public:
		/// Default constructor
		SFMLSpriteComponent(void);

		virtual const char *getName() const { return g_Name; }

	protected:
		// factory method to create the appropriate scene node
		virtual SFMLSceneNodeStrongPtr createSceneNode(void) ANT_OVERRIDE;  

		/// Loads data to the scene node from the <SceneNode> tag
		virtual bool delegateInit(TiXmlElement *data) ANT_OVERRIDE;

		/// Functions that subclasses implement that extends XML generation for child-classes
		virtual void createInheritedXmlElements(TiXmlElement* pBaseElement) ANT_OVERRIDE;

	private:
		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:		
		std::string  m_textureResource;	
		ant::Real    m_scale;
		sf::Vector2i m_textureRectangle;
		sf::Vector2i m_textureRectanglePos;
	public:
		static const char *g_Name;
	};

	/**
	* Extends the base SFML render component with a sprite render component
	*/
	class SFMLAnimatedSpriteComponent : public SFMLBaseRenderComponent
	{
		friend class SFMLSpriteNode;

	public:
		/// Default constructor
		SFMLAnimatedSpriteComponent(void);

		virtual const char *getName() const { return g_Name; }

	protected:
		// factory method to create the appropriate scene node
		virtual SFMLSceneNodeStrongPtr createSceneNode(void) ANT_OVERRIDE;

		/// Loads data to the scene node from the <SceneNode> tag
		virtual bool delegateInit(TiXmlElement *data) ANT_OVERRIDE;

		/// Functions that subclasses implement that extends XML generation for child-classes
		virtual void createInheritedXmlElements(TiXmlElement* pBaseElement) ANT_OVERRIDE;

	private:
		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
		std::string              m_textureResource;
		std::string              m_spriteSheetResource;
	protected:
		
	public:
		static const char *g_Name;
	};

	/**
	 * Simple background component 
	 */
	class SFMLBackgroundSpriteComponent : public SFMLBaseRenderComponent
	{		

	public:
		/// Default constructor
		SFMLBackgroundSpriteComponent(void);

		virtual const char *getName() const { return g_Name; }

	protected:
		// factory method to create the appropriate scene node
		virtual SFMLSceneNodeStrongPtr createSceneNode(void) ANT_OVERRIDE;  

		/// Loads data to the scene node from the <SceneNode> tag
		virtual bool delegateInit(TiXmlElement *data) ANT_OVERRIDE;

		/// Functions that subclasses implement that extends XML generation for child-classes
		virtual void createInheritedXmlElements(TiXmlElement* pBaseElement) ANT_OVERRIDE;

	private:
		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:		
		std::string m_textureResource;	
		ant::Real   m_scale;
	public:
		static const char *g_Name;
	};

	/**
	 * Class that render a rectangle primitive 
	 */
	class SFMLRectanglePrimitiveComponent : public SFMLBaseRenderComponent
	{		

	public:
		/// Default constructor
		SFMLRectanglePrimitiveComponent(void);

		virtual const char *getName() const { return g_Name; }

	protected:
		// factory method to create the appropriate scene node
		virtual SFMLSceneNodeStrongPtr createSceneNode(void) ANT_OVERRIDE;  

		/// Loads data to the scene node from the <SceneNode> tag
		virtual bool delegateInit(TiXmlElement *data) ANT_OVERRIDE;

		/// Functions that subclasses implement that extends XML generation for child-classes
		virtual void createInheritedXmlElements(TiXmlElement* pBaseElement) ANT_OVERRIDE;

	private:
		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:		
		sf::Vector2f m_size;
		bool				 m_filled;
	public:
		static const char *g_Name;
	};

		/**
	 * Class that render a circle primitive 
	 */
	class SFMLCirclePrimitiveComponent : public SFMLBaseRenderComponent
	{		
	public:
		/// Default constructor
		SFMLCirclePrimitiveComponent(void);

		virtual const char *getName() const { return g_Name; }

	protected:
		// factory method to create the appropriate scene node
		virtual SFMLSceneNodeStrongPtr createSceneNode(void) ANT_OVERRIDE;  

		/// Loads data to the scene node from the <SceneNode> tag
		virtual bool delegateInit(TiXmlElement *data) ANT_OVERRIDE;

		/// Functions that subclasses implement that extends XML generation for child-classes
		virtual void createInheritedXmlElements(TiXmlElement* pBaseElement) ANT_OVERRIDE;

	private:
		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:		
		ant::Real		 m_radius;
		bool				 m_filled;
	public:
		static const char *g_Name;
	};
}


#endif