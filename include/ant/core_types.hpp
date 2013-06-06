/**
 * Provides the ANT namespace includes for the ANT Core includes and
 * variables.
 */

#ifndef   ANT_CORE_TYPES_HPP
	#define   ANT_CORE_TYPES_HPP

/////////////////////////////////////////////////////////////////////////////
// Headers
/////////////////////////////////////////////////////////////////////////////
//#include <MGE/ANT_config.hpp>
#include <map>
#include <string>
#include <vector>
#include <ant_config.hpp>
#include <ant/ant_std.hpp>

//define the resource dir
//#define _RELEASE_GAME
#ifndef _RELEASE_GAME
	#define RESOURCE_DIR "D:/GameProjects/MGE/resources"
#else
	#define RESOURCE_DIR "resources"
#endif

#define INFO_LEVEL   0  ///< Defines the value for ant::SeverityInfo
#define WARN_LEVEL   1  ///< Defines the value for ant::SeverityWarning
#define ERROR_LEVEL  2  ///< Defines the value for ant::SeverityError
#define FATAL_LEVEL  3  ///< Defines the value for ant::Severity
#define NO_LOG_LEVEL 4  ///< Defines the value for no logging

/// Inline macro
#define ANT_FORCE_INLINE _forceinline
#define ANT_INLINE inline

#define ANT_OVERRIDE override;

namespace ant 
{

  /// Enumeration of recommended Graphic Range
  enum GraphicRange
  {
    LowRange  = 0, ///< Recommend using LowRange graphics (32x32 pixels)
    MidRange  = 1, ///< Recommend using MidRange graphics (64x64 pixels)
    HighRange = 2  ///< Recommend using HighRange graphics (128x128 pixels)
  };

  /// Enumeration of all Logging severity types
  enum SeverityType
  {
    // Logger severity types range from 0 to 5
    SeverityInfo          = INFO_LEVEL,   ///< Informational severity type
    SeverityWarning       = WARN_LEVEL,   ///< Warning severity type
    SeverityError         = ERROR_LEVEL,  ///< Error severity type
    SeverityFatal         = FATAL_LEVEL   ///< Fatal severity type
  };

  /// Status Enumeration for Status Return values
  enum StatusType
  {
    // Values from -99 to 99 are common Error and Good status responses
    StatusAppMissingAsset = -4,  ///< Application failed due to missing asset file
    StatusAppStackEmpty   = -3,  ///< Application States stack is empty
    StatusAppInitFailed   = -2,  ///< Application initialization failed
    StatusError           = -1,  ///< General error status response
    StatusAppOK           =  0,  ///< Application quit without error
    StatusNoError         =  0,  ///< General no error status response
    StatusFalse           =  0,  ///< False status response
    StatusTrue            =  1,  ///< True status response
    StatusOK              =  1   ///< OK status response

    // Values from +-100 to +-199 are reserved for File status responses
  };

	// Forward declarations for interfaces
	class IApp;
	class IState;
	class IAssetHandler;

	// Forward declaration for classes
	class AssetManager;
	class StateManager;

	// Forward declaration of core assets
	class TextureAsset;
	class TextureHandler;
	class MusicHandler;
	class MusicAsset;
	class SoundAsset;
	class SoundHandler;

	// Loggers 
	class StringLogger;
	class ScopeLogger;
	class FileLogger;
	class FatalLogger;

	// Forward declarations for Actors
	class Actor;
	class ActorComponent;
	class IProcess;
	class Resource;
	class ProcessManager;
	class ResourceHandle;
	class ResourceCache;
	class IResourceLoader;
	class IResourceFile;

	// Declare id for actors
	typedef unsigned int ActorId;

	// Declare id for actor components
	typedef unsigned int ComponentId;

	// Declare default values
	const ActorId INVALID_ACTOR_ID = 0;
	const ComponentId INVALID_COMPONENT_ID = 0;

	// Declare Pointer types
	ANT_DECLARE_POINTER_TYPES(Actor)
	ANT_DECLARE_POINTER_TYPES(ActorComponent)
	ANT_DECLARE_POINTER_TYPES(IProcess)
	ANT_DECLARE_POINTER_TYPES(ResourceHandle)
	ANT_DECLARE_POINTER_TYPES(IResourceLoader)
	ANT_DECLARE_POINTER_TYPES(IResourceFile)

	/// Declare Asset ID typedef which is used for identifying Asset objects
  typedef std::string typeAssetID;

  /// Declare Asset Handler ID typedef which is used for identifying Asset Handler objects
  typedef std::string typeAssetHandlerID;

  /// Declare Event ID typedef which is used for identifying Event objects
  typedef std::string typeEventID;

  /// Declare Property ID typedef which is used for identifying Properties
  typedef std::string typePropertyID;

  /// Declare State ID typedef which is used for identifying State objects
  typedef std::string typeStateID;

  /// Declare NameValue typedef which is used for config section maps
  typedef std::map<const std::string, const std::string> typeNameValue;

  /// Declare NameValueIter typedef which is used for name,value pair maps
  typedef std::map<const std::string, const std::string>::iterator typeNameValueIter;

	/// Declare data types
	typedef double Real;
	typedef unsigned int UInt;
	typedef double DeltaTime;
	
}

#endif  //ANT_CORE_TYPES_HPP
