#include "App.hpp"
#include <ostream>
#include <string>
#include <vector>

using namespace ant;
/*
* Test implemenation of the application class
*/

TestApp::TestApp( const std::string title /*= "TestApp"*/ ): 
	IApp(title)
{

	mLooger = new FileLogger("./logger.log",true);

}

TestApp::~TestApp()
{
	//std::cout << mLooger->getStream() << std::endl;
}

void TestApp::initCustomAssetHandlers()
{
	//Test custom asset stuff
}

void TestApp::initCustomGameStates()
{

	mStateManager.addActiveState(new(std::nothrow) GameState(*this));
	//mStateManager.addActiveState(new(std::nothrow) SplashState(*this,5));
}

void TestApp::handleCustomCleanup()
{

}


