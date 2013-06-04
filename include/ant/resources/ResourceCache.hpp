#ifndef RESOURCECACHE_HPP_
	#define RESOURCECACHE_HPP_

#include <ant/core_types.hpp>

namespace ant
{

	typedef std::list< ResourceHandleStrongPtr > ResourceHandleList;
	typedef std::map< std::string , ResourceHandleStrongPtr > ResourceHandleMap;
	typedef std::list< IResourceLoaderStrongPtr > ResourceLoaders;

	/**
	 * Class that manages resources 
	 */
	class ResourceCache
	{
	public:
		/// Default constructor
		ResourceCache();
	
		/// Default destructor
		~ResourceCache();

	protected:
	
	private:
	};
}

#endif