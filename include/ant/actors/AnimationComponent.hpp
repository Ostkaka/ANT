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
		Animation();

		~Animation();

		bool update(ant::DeltaTime dt);

		ant::Real getFrameRate() const;
		
		bool shouldLoop() const;

		ant::UInt getCurrentFrameIndex() const;

	private:
		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
		ant::Real m_frameRate;	
		ant::UInt m_currentListIndex;
		bool m_shouldLoop;
		FrameIndexList m_frameList;
	};
	
	ANT_INLINE ant::Real Animation::getFrameRate() const { return m_frameRate; }
	
	ANT_INLINE bool Animation::shouldLoop() const { return m_shouldLoop; }

	ANT_INLINE ant::UInt Animation::getCurrentFrameIndex() const { GCC_ASSERT(m_currentListIndex < m_frameList.size());  return m_frameList[m_currentListIndex]; }
		
	/**
	* Base class for handling and updating different animations for an entity. Is known an used by render components that 
	* support animation, such as AnimatedSprite.
	*/
	class AnimationComponent : ActorComponent
	{
		typedef std::map<AnimationId, Animation> AnimationMap;
	public: 
		AnimationComponent();
	
		virtual ~AnimationComponent();

		virtual bool init(TiXmlElement* pData);

		virtual TiXmlElement* generateXml(void);

		virtual void postInit();

		virtual void update(ant::DeltaTime dt);

		virtual void onChanged(void);

		void changeAnimationStateDelegate(IEventDataStrongPtr eventData);

		virtual const char *getName() const { return g_Name; }

	protected:

		virtual bool delegateInit(TiXmlElement *data);

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////

	protected:	
		static const char *g_Name;
		AnimationMap m_animationMap;
		// Animation data
		ant::DeltaTime m_currentTime;
		ant::DeltaTime m_lastAnimTime;
		Animation *    m_currentAnimation;
	};

	class EvtData_ChangeAnimation : ScriptEvent
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

		~EvtData_ChangeAnimation()
		{

		}

		virtual IEventDataStrongPtr copy(void) const
		{
			return IEventDataStrongPtr(GCC_NEW EvtData_ChangeAnimation(m_actorId, m_animationId));
		}

		virtual const EventType& getEventType(void) const
		{
			return sk_EventType;
		}

		virtual void deserialize(std::istrstream& in)
		{
			in >> m_actorId;
			in >> m_animationId;
		}

		virtual void serialize(std::ostrstream& out) const
		{
			out << m_actorId << "	";
			out << m_animationId << "	";
		}

		virtual std::string getName(void) const
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

		virtual bool buildEventFromScript(void);

		virtual void buildEventData(void);

		ANT_EXPORT_FOR_SCRIPT_EVENT(EvtData_TestExecute);

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:
		ActorId     m_actorId;
		AnimationId m_animationId;
	};

	void registerAnimationScriptEvents(void);
}

#endif