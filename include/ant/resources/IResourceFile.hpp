#ifndef IRESOURCEFILE_HPP_
	#define IRESOURCEFILE_HPP_

// Forward declaration
class ant::Resource;
class std::string;

namespace ant
{
	/**
	* Interface class for declaring resource files
	*/
	class IResourceFile
	{
	public:

		/// Default constructor
		IResourceFile(){};
		
		/// Default destructor
		virtual ~IResourceFile(){};

		/// Tries to open the resource file
		virtual bool open()=0;

		/// Get the raw resource data size
		virtual int getRawResourceSize(const Resource&)=0;

		/// Get the raw resource of the handle
		virtual int getResource(const Resource& r, char *buffer)=0;

		/// Get the number of resources from the file
		virtual int getNumResources() const = 0;

		/// Returns the name of the resource in the file given an index
		virtual std::string getResourceName() const = 0;
	};
}

#endif

