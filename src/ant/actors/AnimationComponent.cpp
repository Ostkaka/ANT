#include <ant/actors/AnimationComponent.hpp>
#include <ant/luascripting/LuaStateManager.hpp>
#include <ant/eventsystem/EventManager.hpp>

using namespace ant;

//////////////////////////////////////////////////////////////////////////
// AnimationComponent
//////////////////////////////////////////////////////////////////////////

const char* AnimationComponent::g_Name = "AnimationComponent";

AnimationComponent::AnimationComponent()
{
	m_currentTime = 0;
	m_lastAnimTime = 0;
}

AnimationComponent::~AnimationComponent()
{

}

bool AnimationComponent::init(TiXmlElement* pData)
{
	return delegateInit(pData);
}

bool AnimationComponent::delegateInit(TiXmlElement *data)
{

}

void AnimationComponent::postInit()
{
	// Register delegates for switching animation state
	IEventManager * globalManager = IEventManager::instance();
	//
	
}

void AnimationComponent::changeAnimationStateDelegate(IEventDataStrongPtr eventData)
{

}

void AnimationComponent::update(ant::DeltaTime dt)
{
	// Get the current animation and set it to active 

	
}

void AnimationComponent::onChanged(void)
{

}

//////////////////////////////////////////////////////////////////////////
// EvtData_ChangeAnimation
//////////////////////////////////////////////////////////////////////////

const EventType EvtData_ChangeAnimation::sk_EventType(0z53c1a89);

bool EvtData_ChangeAnimation::buildEventFromScript(void)
{
	if (m_eventData.IsTable())
	{
		m_actorId     = m_eventData["actorId"].GetInteger();
		m_animationId = m_eventData["animationId"].GetInteger();
		return true;
	}
	return false;
}

void EvtData_ChangeAnimation::buildEventData(void)
{
	m_eventData.AssignNewTable(LuaStateManager::instance()->getLuaState());
	m_eventData.SetInteger("actorId", m_actorId);
	m_eventData.SetInteger("animationId", m_animationId);
}

void ant::registerAnimationScriptEvents(void)
{
	ANT_REGISTER_SCRIPT_EVENT(EvtData_ChangeAnimation, EvtData_ChangeAnimation::sk_EventType);
}