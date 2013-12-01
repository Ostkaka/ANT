#include <assert.h>
#include <stddef.h>
#include <Core_include.hpp>
#include <ant/interfaces/ISFMLApp.hpp>
#include "TestApp.hpp"

#include <stdio.h>  /* defines FILENAME_MAX */
#include <direct.h>
#define GetCurrentDir _getcwd

int main( int argc, char* argv[] ){

	int exitCode = ant::StatusNoError;

	char cCurrentPath[FILENAME_MAX];

	cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */

	if(!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
	{
		return errno;
	}

	printf("The current working directory is %s", cCurrentPath);

	ant::SFMLApp * app = new(std::nothrow) ant::SFMLApp();

	if (!app->init())
	{
		std::cout << "Could not initialize app!" << std::endl;
	}

	exitCode = app->run();

	delete app;

	app = NULL;

	return exitCode;
} 