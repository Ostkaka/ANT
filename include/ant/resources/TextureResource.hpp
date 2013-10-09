#ifndef TEXTURERESOURCE_HPP_
	#define TEXTURERESOURCE_HPP_

#include <ant/core_types.hpp>
#include <ant/resources/IResourceLoader.hpp>
#include <ant/resources/Resource.hpp>

namespace ant
{
	/**
	 * Resource loader for loading PNG files 
	 */
	class SFMLTextureResourceLoader : public IResourceLoader
	{
		virtual bool useRawFile() { return true; }
		virtual bool discardRawBufferAfterLoad() { return false; }
		virtual ant::UInt getLoadedResourceSize(char* rawBuffer, ant::UInt rawSize);
		virtual bool loadResource(char *rawBuffer, unsigned int rawSize, ResourceHandleStrongPtr handle);
	};

	/**
	 * Loader for PNG files
	 */
	class PngResourceLoader : public SFMLTextureResourceLoader
	{
		virtual std::string getPattern() { return "*.png"; }
	};

	/**
	* Loader for JPG files
	*/
	class JpgResourceLoader : public SFMLTextureResourceLoader
	{
		virtual std::string getPattern() { return "*.jpg"; }
	};
}

#endif