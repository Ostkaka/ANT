/*
* Test implementation of the application class
*/
#include <string>
#include <core_include.hpp>
#include "GameState.hpp"
#include "SplashState.hpp"
#include <ant\loggers\FileLogger.hpp>

using namespace ant;

class TestApp : public IApp {

	/*
	* Fill with useless methods
	*/

public:
	TestApp(const std::string title = "TestApp");

	virtual ~TestApp();

protected:

	virtual void initCustomAssetHandlers();

	virtual void initCustomGameStates();

	virtual void handleCustomCleanup();

	ant::FileLogger *		mLooger;

};