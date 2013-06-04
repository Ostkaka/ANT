#include <ant/resources/ResourceHandle.hpp>

ant::ResourceHandle::ResourceHandle( Resource &resource, char* buffer, ant::UInt size, ResourceCache * pResCache )
	:m_resource(resource)
{
	m_buffer = buffer;
	m_size = size;
	m_extra = NULL;
	m_pResCache = pResCache;
}





