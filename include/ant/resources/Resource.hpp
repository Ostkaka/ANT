#ifndef RESOURCE_HPP_
	#define RESOURCE_HPP_

#include <string>

namespace ant 
{
	/**
	 * Class that acts like an identifier for a resource asset in as resource file
	 */
	class Resource
	{
	public:
		
		/// Default constructor that takes the unique identifier for the resource
		Resource(const std::string& name);

		/// Default destructor
		~Resource();

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:
		std::string m_name;
	};

	/**
	 * Class that is used to attach extra data to a resource file.
	 */
	class IResourceExtraData
	{
	public:
		
		/// Default constructor
		IResourceExtraData();
		
		/// Default destructor
		~IResourceExtraData();

		/// Converts the extra data to a string
		virtual std::string toString()=0;
	};


}

#endif