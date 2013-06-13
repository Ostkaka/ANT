#include <unittests.hpp>
#include <ant/eventsystem/EventManager.hpp>
#include <ant/core_types.hpp>
#include <string.h>

using namespace ant;

class EvtData_Test : public BaseEventData
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

	bool isExecuted(void) const { return m_executed; }

protected:
	bool m_executed;
};


using namespace ant;

TEST(Test_EventManager, EventListenerAddRemove)
{	

}

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}