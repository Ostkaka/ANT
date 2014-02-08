#ifndef ANIMATIONCOMPONEN_HPP
	#define ANIMATIONCOMPONEN_HPP

#include <ant/actors/ActorComponent.hpp>
#include <ant/luascripting/ScriptEvent.hpp>
#include <SFML/Graphics.hpp>
#include <ant/core_types.hpp>

namespace ant
{
	typedef std::string AnimationId;

	/**
	* Base class that handles the information about an animation component
	*/
	class Animation
	{
		typedef std::vector<int> FrameIndexList;
	public:
	
		// Creates an animation
		Animation(const FrameIndexList& list, ant::Real fps, bool shouldLoop = true );

		~Animation();

		ant::Real getFrameRate() const;
		
		void setShouldLoop(bool shouldLoop);

		bool getShouldLoop() const;

		ant::UInt getCurrentFrameIndex() const;

		/// Increment the current frame index and returns it
		ant::UInt incrementToNextFrame();

	protected:
		// Default constructor
		Animation();

	protected:
		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
		ant::Real      m_frameRate;	
		ant::UInt      m_currentListIndex;
		bool           m_shouldLoop;
		FrameIndexList m_frameList;
	};

	ANT_INLINE ant::Real Animation::getFrameRate() const { return m_frameRate; }
	
	ANT_INLINE bool Animation::getShouldLoop() const { return m_shouldLoop; }

	ANT_INLINE void Animation::setShouldLoop(bool shouldLoop) { m_shouldLoop = shouldLoop; }

	ANT_INLINE ant::UInt Animation::getCurrentFrameIndex() const { GCC_ASSERT(m_currentListIndex < m_frameList.size());  return m_frameList[m_currentListIndex]; }

	ANT_DECLARE_POINTER_TYPES(Animation)

	/**
	* Base class for handling and updating different animations for an entity. Is known an used by render components that 
	* support animation, such as AnimatedSprite.
	*/
	class AnimationComponent : public ActorComponent
	{
		typedef std::map<AnimationId, AnimationStrongPtr> AnimationMap;

	public: 
		AnimationComponent();
	
		virtual ~AnimationComponent();

		virtual bool init(TiXmlElement* pData) ANT_OVERRIDE;

		virtual TiXmlElement* generateXml(void) ANT_OVERRIDE;

		virtual void postInit() ANT_OVERRIDE;

		virtual void update(ant::DeltaTime dt) ANT_OVERRIDE;

		virtual void onChanged(void) ANT_OVERRIDE;

		virtual const char *getName() const ANT_OVERRIDE { return g_Name; }

		ant::UInt getFrameIndex() const;

		void changeAnimationStateDelegate(IEventDataStrongPtr eventData);

	protected:

		virtual bool delegateInit(TiXmlElement *data);

		void setAnimation(AnimationId id);

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:	
		static const char *g_Name;
		AnimationMap              m_animationMap;		
		ant::DeltaTime            m_currentTime;
		ant::DeltaTime            m_lastAnimTime;
		AnimationStrongPtr        m_currentAnimation;
	};
	
	ANT_INLINE ant::UInt AnimationComponent::getFrameIndex() const { GCC_ASSERT(m_currentAnimation); return m_currentAnimation->getCurrentFrameIndex(); }

	//////////////////////////////////////////////////////////////////////////
	// EvtData_ChangeAnimation - Event that is sent out when an animation should change for an entity with an animation component
	//////////////////////////////////////////////////////////////////////////
	class EvtData_ChangeAnimation : public ScriptEvent
	{
	public:
		static const EventType sk_EventType;

		EvtData_ChangeAnimation() :
			m_actorId(INVALID_ACTOR_ID),
			m_animationId(INVALID_ACTOR_ID)
		{

		}

		EvtData_ChangeAnimation(ActorId id, AnimationId animId): 
		m_actorId(id), 
		m_animationId(animId)
		{

		}

		virtual ~EvtData_ChangeAnimation()
		{

		}

		virtual IEventDataStrongPtr copy(void) const ANT_OVERRIDE
		{
			return IEventDataStrongPtr(GCC_NEW EvtData_ChangeAnimation(m_actorId, m_animationId));
		}

		virtual const EventType& getEventType(void) const ANT_OVERRIDE
		{
			return sk_EventType;
		}

		virtual void deserialize(std::istrstream& in) ANT_OVERRIDE
		{
			in >> m_actorId;
			in >> m_animationId;
		}

		virtual void serialize(std::ostrstream& out) const ANT_OVERRIDE
		{
			out << m_actorId << "	";
			out << m_animationId << "	";
		}

		virtual std::string getName(void) const ANT_OVERRIDE
		{
			return "EvtData_ChangeAnimation";
		}

		const ActorId getActorId(void) const 
		{
			return m_actorId;
		}

		AnimationId getAnimationId(void) const
		{
			return m_animationId;
		}

		virtual bool buildEventFromScript(void) ANT_OVERRIDE;

		virtual void buildEventData(void) ANT_OVERRIDE;

		ANT_EXPORT_FOR_SCRIPT_EVENT(EvtData_ChangeAnimation);

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:
		ActorId     m_actorId;
		AnimationId m_animationId;
	};

	void registerAnimationScriptEvents(void);

	ANT_DECLARE_POINTER_TYPES(EvtData_ChangeAnimation);
}

#endif