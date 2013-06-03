#ifndef RESOURCEHANDLE_HPP_
	#define RESOURCEHANDLE_HPP_

#include <ant/core_types.hpp>

// Forward declaration
class ant::ResourceCache;
class ant::Resource;
class ant::IResourceExtraData;

namespace ant
{

	typedef shared_ptr<IResourceExtraData> ResourceExtraDataStrongPtr;

	/**
	 * Class that couples loaded resource data with an resource identifier Resource
	 */
	class ResourceHandle
	{
	public:
		friend class ResourceChache;

		/// Default constructor
		ResourceHandle(Resource &resource, char* buffer, ant::UInt size, ResourceCache * pResCache);

		/// Default destructor
		virtual ~ResourceHandle();

		/// Returns the raw size of the resource
		ant::UInt getSize() const; // inline this shit

		/// Returns the raw buffer data of the resource
		char* getBuffer() const; //inline

		/// Returns a buffer pointer that can be written to
		char* getWritablebuffer(); //inline

		/// Returns the extra data associated with this object
		ResourceExtraDataStrongPtr getExtraData(); //inline

		/// Sets the extra data of this resource
		void setExtraData(ResourceExtraDataStrongPtr extra); //inline

		//////////////////////////////////////////////////////////////////////////
		// Variables
		//////////////////////////////////////////////////////////////////////////
	protected:
		/// Resource identifier
		Resource m_Resource;
		/// Raw resource data
		char* m_buffer
		/// size if the data
		ant::UInt m_size;
		/// Pointer to the extra data
		ResourceExtraDataStrongPtr m_extra;
		/// Raw pointer to the cache this 
		ResourceCache *m_pResCache;
		
	};
}
#endif