#ifndef IEVENTMANAGER_HPP_
	#define IEVENTMANAGER_HPP_

#include <strstream>
#include <FastDelegate.h>
#include <ant/core_types.hpp>

namespace ant
{
	
	/**
	 * Typedefs
	 */
	typedef fastdelegate::FastDelegate1<IEventDataStrongPtr> EventListenerDelegate;
	//typedef concurrent_queue<IEventDataPtr> ThreadSafeEventQueue;

	/**
	 * Macros 
	 */
	extern GenericObjectFactory<IEventData, EventType> EventFactory;
	#define REGISTER_EVENT(eventClass) EventFactory.register<eventClass>(eventClass::EventTypeId);
	#define CREATE_EVENT(eventType) EventFactory.create(eventType);

	/**
	 * Base interface for event objects
	 */
	class IEventData
	{
	public:

		/// Destructor
		virtual ~IEventData(void) {}

		/// Returns the eventtype of the object
		virtual const EventType& getEventType(void) const = 0;

		/// Get the timestamp when the event was created
		virtual ant::TimeStamp getTimeStamp(void) const = 0;

		// TODO - make this a class ISerializable
		virtual void serialize(std::ostream& out) const = 0;  
		virtual void deserialize(std::istream& in) const = 0;

		/// Makes a copy of the event data
		virtual IEventDataStrongPtr copy() const = 0;

		/// Returns the name of the event
		virtual const char* getName(void) const = 0;
	};

	/**
	 * Class the implements a basic event data structure 
	 */
	class BaseEventData : public IEventData
	{
		
	public:

		explicit BaseEventData(const float timeStamp = 0.0f) 
			: m_timeStamp(timeStamp) { }

		// Returns the type of the event
		virtual const EventType& getEventType(void) const = 0;

		/// Return the timestamp when the event was created
		ant::TimeStamp getTimeStamp(void) const { return m_timeStamp; }

		// Serializing 
		virtual void serialize(std::ostrstream &out) const	{ }
		virtual void deserialize(std::istrstream& in) { }

	protected:
		/// Timestamp of when the event was created
		const ant::TimeStamp m_timeStamp;
	};

	/**
	 * Class that manages lists of events and their registered listeners
	 */
	class IEventManager 
	{
		enum EventManagerConstants { EM_INFINITE = 0xffffffff};

		explicit IEventManager(const std::string& name, bool setGlobal);
		virtual ~IEventManager(void);

		virtual bool addListener(const EventListenerDelegate& eDelegate, const EventType& type) = 0;

		virtual bool removeListener(const EventListenerDelegate& eDelegate, const EventType& type) = 0;

		virtual bool triggerEvent(const IEventDataStrongPtr& pEvent) const = 0;

		virtual bool queueEvent(const IEventDataStrongPtr& pEvent)= 0;

		// TODO - thread safe queue

	};

}


#endif