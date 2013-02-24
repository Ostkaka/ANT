#include <ant/loggers/FatalLogger.hpp>

namespace ant 
{
	FatalLogger::FatalLogger( SeverityType severity, const std::string& sourceFile, int sourceLine, int exitCode /*= StatusError*/ ):
	StringLogger(false,exitCode)
	{
		//Create tag
		writeTag(getStream(),severity,sourceFile,sourceLine);	
	}
	
	FatalLogger::~FatalLogger()
	{
		// Make sure our global logger is defined before using it
		if(ant::ILogger::getLogger())
		{
			// Log the Fatal message string stream created
			ant::ILogger::getLogger()->logMessage(getString().c_str());
		}
		// Call the FatalShutdown method defined in ILogger
		fatalShutdown();
	}
}