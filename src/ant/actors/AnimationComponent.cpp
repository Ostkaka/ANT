#include <ant/actors/AnimationComponent.hpp>
#include <ant/luascripting/LuaStateManager.hpp>
#include <ant/eventsystem/EventManager.hpp>

using namespace ant;

//////////////////////////////////////////////////////////////////////////
// Animation
//////////////////////////////////////////////////////////////////////////

Animation::Animation(const FrameIndexList& list, ant::Real fps, bool shouldLoop)
{
	m_currentListIndex = 0;
	m_frameList        = list;
	m_frameRate        = fps;
	m_shouldLoop       = shouldLoop;
}

Animation::~Animation()
{

}

ant::UInt Animation::incrementToNextFrame()
{
	if (m_shouldLoop)
	{
		m_currentListIndex = (m_currentListIndex + 1) % m_frameList.size();
	}
	else if (m_currentListIndex + 1 > m_frameList.size())
	{
		return m_currentListIndex;
	}
	else
	{
		return ++m_currentListIndex;
	}
}

//////////////////////////////////////////////////////////////////////////
// AnimationComponent
//////////////////////////////////////////////////////////////////////////

const char* AnimationComponent::g_Name = "AnimationComponent";

AnimationComponent::AnimationComponent()
{
	m_currentTime      = 0;
	m_lastAnimTime     = 0;
	m_currentAnimation = nullptr;
}

AnimationComponent::~AnimationComponent()
{
	IEventManager * globalManager = IEventManager::instance();
	globalManager->removeListener(MakeDelegate(this, &AnimationComponent::changeAnimationStateDelegate), EvtData_ChangeAnimation::sk_EventType);
}

bool AnimationComponent::init(TiXmlElement* pData)
{
	return delegateInit(pData);
}

bool AnimationComponent::delegateInit(TiXmlElement *data)
{
	return true;
}

void AnimationComponent::postInit()
{
	// Register delegates for switching animation state
	IEventManager * globalManager = IEventManager::instance();
	globalManager->addListener(MakeDelegate(this, &AnimationComponent::changeAnimationStateDelegate), EvtData_ChangeAnimation::sk_EventType);
}

void AnimationComponent::changeAnimationStateDelegate(IEventDataStrongPtr eventData)
{
	EvtData_ChangeAnimationStrongPtr p = static_pointer_cast<EvtData_ChangeAnimation>(eventData);

	// Check if the actor has the same id as the event
	if (m_pOwner->getId() == p->getActorId())
	{
		//Does the animation id exist in this component?
		if (m_animationMap.find(p->getAnimationId()) != m_animationMap.end())
		{
			// If so, change the active animation to the one specified by the id
			setAnimation(p->getAnimationId());
		}	
		else
		{
			GCC_WARNING("Trying to set animation :" + p->getAnimationId() + " which does not exits in actor: " + ToStr(p->getActorId()));
		}
	}
}

void AnimationComponent::setAnimation(AnimationId id)
{
	// try to set the current animation
	if (m_animationMap.find(id) != m_animationMap.end())
	{
		m_currentAnimation = m_animationMap[id];
	}

	// Verify that it is not null
	GCC_ASSERT( m_currentAnimation );
}

void AnimationComponent::update(ant::DeltaTime dt)
{
	// Update time according to dt
	m_currentTime += dt;

	// Get the current animation and set it to active 
	if ( m_currentAnimation )
	{
		// Do we animate now?
		ant::Real delta = m_currentTime - m_lastAnimTime;
		ant::Real FPS = m_currentAnimation->getFrameRate();
		if ( delta >  (1.0 / FPS))
		{
			// Update the animation
			int f = m_currentAnimation->incrementToNextFrame();
		}
	}		
}

void AnimationComponent::onChanged(void)
{

}

TiXmlElement* AnimationComponent::generateXml(void)
{
	return nullptr;
}

//////////////////////////////////////////////////////////////////////////
// EvtData_ChangeAnimation
//////////////////////////////////////////////////////////////////////////

const EventType EvtData_ChangeAnimation::sk_EventType(0x47a22307);

bool EvtData_ChangeAnimation::buildEventFromScript(void)
{
	if (m_eventData.IsTable())
	{
		m_actorId     = m_eventData["actorId"].GetInteger();
		m_animationId = m_eventData["animationId"].GetString();
		return true;
	}
	return false;
}

void EvtData_ChangeAnimation::buildEventData(void)
{
	m_eventData.AssignNewTable(LuaStateManager::instance()->getLuaState());
	m_eventData.SetInteger("actorId", m_actorId);
	m_eventData.SetString("animationId", m_animationId.c_str());
}

void ant::registerAnimationScriptEvents(void)
{
	ANT_REGISTER_SCRIPT_EVENT(EvtData_ChangeAnimation, EvtData_ChangeAnimation::sk_EventType);
}