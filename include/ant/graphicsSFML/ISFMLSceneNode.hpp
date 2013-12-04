#ifndef ISFMLSCENENODE_HPP_
	#define ISFMLSCENENODE_HPP_

#include <ant/core_types.hpp>
#include <SFML/Graphics.hpp>

namespace ant
{

	class SFMLSceneNodeProperties;

	/**
	 * Interface for a 2D scene node
	 */
	class ISFMLSceneNode
	{
	public:

		virtual ~ISFMLSceneNode() { }

		virtual const SFMLSceneNodeProperties * const getNodeProps() const=0;

		virtual HRESULT onUpdate(SFMLScene *scene, ant::DeltaTime dt)=0;

		virtual HRESULT onRestore(SFMLScene *scene)=0;

		virtual HRESULT preRender(SFMLScene *scene)=0;

		virtual HRESULT render(SFMLScene *scene)=0;

		virtual HRESULT postRender(SFMLScene *scene)=0;

		virtual bool isVisible(SFMLScene *scene) const=0;

		virtual HRESULT onLostDevice(SFMLScene *scene)=0;

		virtual HRESULT renderChildren(SFMLScene *scene)=0; 

		virtual bool addChild(ISFMLSceneNodeStrongPtr kid)=0;

		virtual bool removeChild(ActorId id)=0;	 

		//virtual HRESULT pick()=0;

		virtual void setPosition(const sf::Vector2f& pos)=0;

		virtual const sf::Vector2f& getPosition()=0;

		virtual void setDirection(const sf::Vector2f& pos)=0;

		virtual const sf::Vector2f& getDirection()=0;

		virtual void setRotation(const ant::Real& rot)=0;

		virtual ant::Real getRotation()=0;
	};
}

#endif