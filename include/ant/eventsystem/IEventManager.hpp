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

		/// Returns the event type of the object
		virtual const EventType& getEventType(void) const = 0;

		/// Get the timestamp when the event was created
		virtual ant::TimeStamp getTimeStamp(void) const = 0;

		// TODO - make this a class ISerializable
		virtual void serialize(std::ostream& out) const = 0;  
		virtual void deserialize(std::istream& in) = 0;

		/// Makes a copy of the event data
		virtual IEventDataStrongPtr copy() const = 0;

		/// Returns the name of the event
		virtual std::string getName(void) const = 0;
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
	public:

		enum EventManagerConstants { EM_INFINITE = 0xffffffff};

		/// Constructor
		explicit IEventManager(const std::string& name, bool setGlobal);
		
		/// Destructor
		virtual ~IEventManager(void);

		/// Add a listener to the event manager that listens to an event of type is triggered. 
		virtual bool addListener(const EventListenerDelegate& eDelegate, const EventType& type) = 0;

		/// Removes a delegate / event type pairing from the event manager.
		virtual bool removeListener(const EventListenerDelegate& eDelegate, const EventType& type) = 0;

		/// Triggers an event IMMEDIATELY. Bypasses the queue and calls alls registered delegate functions
		virtual bool triggerEvent(const IEventDataStrongPtr& pEvent) const = 0;

		/// Queues and event in the event manager
		virtual bool queueEvent(const IEventDataStrongPtr& pEvent)= 0;

		// TODO - thread safe queue
		/**
		 *  Finds and removes the next available instance of the given event type and removes it
		 *  from the processing queue. if allOfType is true, all events of that type is cleared
		 *  Return true if succ, false if fail
		 */
		virtual bool abortEvent(const EventType& type, bool allOfType = false) = 0;

		/**
		 *  Processes any queued events. Can give optional time limiter to limit event handling time
		 *  Returns true if all events in the queue were handled.
		 */
		virtual bool update(const ant::DeltaTime& dt = EM_INFINITE) = 0;

		/// Returns the global static event manager of the framework
		static IEventManager* instance(void);

	};

}


#endif