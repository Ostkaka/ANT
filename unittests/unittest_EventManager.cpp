#include <unittests.hpp>
#include <ant/eventsystem/EventManager.hpp>
#include <ant/core_types.hpp>
#include <string.h>

using namespace ant;

class EvtData_Test : public ant::BaseEventData
{

public:
	static const EventType sk_EventType;

	explicit EvtData_Test()
		: m_executed(false)
	{		
	}

	virtual const EventType& getEventType(void) const ANT_OVERRIDE
	{
		return sk_EventType;
	}

	virtual IEventDataStrongPtr copy(void) const ANT_OVERRIDE
	{
		return IEventDataStrongPtr ( GCC_NEW EvtData_Test ( ) );
	}

	virtual void serialize(std::ostrstream &out) const ANT_OVERRIDE
	{
		out << m_executed;
	}

	virtual void deserialize(std::istrstream& in) ANT_OVERRIDE
	{
		in >> m_executed;
	}

	virtual std::string getName(void) const ANT_OVERRIDE
	{
		return "EvtData_Destroy_Actor";
	}

	void execute(){ m_executed = true;}

	bool isExecuted(void) const { return m_executed; }

protected:
	bool m_executed;
};
const EventType EvtData_Test::sk_EventType(0xa3814acd);

class Test_EventManager : public ::testing::Test
{
protected:

	Test_EventManager()
	{
		std::cout << "Constructor" << std::endl;
	}

	virtual void SetUp() ANT_OVERRIDE
	{
		// Setup logger
		Logger::Init("Test_EventManager.log");

		TheEventManager.reset(new EventManager("TestEventManager",false));
		std::cout << "Set up" << std::endl;
	}

	virtual void TearDown() ANT_OVERRIDE
	{
	}

	virtual ~Test_EventManager() 
	{
		std::cout << "Destructor" << std::endl;
	}

protected:
	shared_ptr<EventManager> TheEventManager;

};

class SimpleTester_Execute
{
public:
	SimpleTester_Execute(){}
	~SimpleTester_Execute(){}

	void init(EventManager * manager)
	{
		GCC_ASSERT(manager);
		if (manager)
		{
			// Create and insert the delegate
			EventListenerDelegate delegateFunc = MakeDelegate(this,&SimpleTester_Execute::execute_TestEvent);

			manager->addListener(delegateFunc,EvtData_Test::sk_EventType);
		}
	}

	void execute_TestEvent(IEventDataStrongPtr pEvent)
	{
		shared_ptr<EvtData_Test> pCastedEvent = static_pointer_cast<EvtData_Test>(pEvent);

		pCastedEvent->execute();
	}
};

TEST_F(Test_EventManager, TestSimpleEvent)
{	
	// Create a tester and insert the manager
	SimpleTester_Execute tester;
	tester.init(TheEventManager.get());

	// Create event and send it to the manager
	IEventDataStrongPtr ev(GCC_NEW EvtData_Test());

	TheEventManager->triggerEvent(ev);

	// Event should have been executed
	EXPECT_TRUE(((EvtData_Test*)(ev.get()))->isExecuted());
}

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}