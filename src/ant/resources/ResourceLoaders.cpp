#include <ant/resources/ResourceLoaders.hpp>

using namespace ant;

DefaultResourceLoader::DefaultResourceLoader()
{

}

DefaultResourceLoader::~DefaultResourceLoader()
{

}

std::string DefaultResourceLoader::getPattern()
{
	return "*";
}

bool DefaultResourceLoader::useRawFile()
{
	return true;
}

bool DefaultResourceLoader::discardRawBufferAfterLoad()
{
	return true;
}

unsigned int DefaultResourceLoader::getLoadedResourceSize( char *rawBuffer, unsigned int rawSize )
{
	return rawSize;
}

bool DefaultResourceLoader::loadResource( char* rawBuffer, ant::UInt rawSize, ResourceHandleStrongPtr handle )
{
	return true;
}

