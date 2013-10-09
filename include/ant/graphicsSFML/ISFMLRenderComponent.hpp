#ifndef ISFMLBASERENDERCOMPONENT_HPP_
	#define ISFMLBASERENDERCOMPONENT_HPP_

#include <ant/actors/ActorComponent.hpp>

namespace ant
{
	class ISFMLRenderComponent : public ActorComponent
 	{
		// RenderComponentInterface
		virtual SFMLSceneNodeStrongPtr getSceneNode(void) = 0;
	};
}

#endif