#include <ant/resources/ResourceFiles.hpp>
#include <ant/resources/Resource.hpp>
#include <ant/resources/ZipFile.hpp>

using namespace ant;

ant::ResourceZipFile::ResourceZipFile( const std::wstring resFileName )
{
	 m_pZipFile = NULL; 
	 m_resFileName=resFileName;
}

ant::ResourceZipFile::~ResourceZipFile()
{
	SAFE_DELETE(m_pZipFile); 
}

bool ant::ResourceZipFile::open()
{
	m_pZipFile = GCC_NEW ZipFile;
	if (m_pZipFile)
	{
		return m_pZipFile->init(m_resFileName.c_str());
	}
	return false;
}

int ant::ResourceZipFile::getRawResourceSize( const Resource& r)
{
	int numresource = m_pZipFile->find(r.getName().c_str());
	if (numresource == -1)
	{
		return -1;
	}
}

int ant::ResourceZipFile::getRawResource( const Resource& r, char *buffer )
{
	int size = 0;
	optional<int> resourceNum = m_pZipFile->find(r.getName().c_str());
	if (resourceNum.valid())
	{
		size = m_pZipFile->getFileLen(*resourceNum);
		m_pZipFile->readFile(*resourceNum,buffer);
	}
	return size;
}

int ant::ResourceZipFile::getNumResources() const
{
	return (m_pZipFile==NULL) ? 0 : m_pZipFile->getNumFiles();
}

std::string ant::ResourceZipFile::getResourceName( int num ) const
{
	std::string retname = "";
	if (m_pZipFile != NULL && num >=0 && num < m_pZipFile->getNumFiles())
	{
		retname = m_pZipFile->getFileName(num);
	}
	return retname;
}

bool ant::ResourceZipFile::isUsingDevelopmentDirectories( void ) const
{
	return false;
}


//////////////////////////////////////////////////////////////////////////
//
// DevelopmentResourceZipFile
//
//////////////////////////////////////////////////////////////////////////

ant::DevelopmentResourceZipFile::DevelopmentResourceZipFile( const std::wstring resFileName, const Mode mode )
	:ResourceZipFile(resFileName)
{
	m_mode = mode;

	TCHAR dir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, dir);

	m_AssetsDir 0 dir;
	int lastslash = m_AssetsDir.find_last_of(L"\\");
	m_AssetsDir = m_AssetsDir.substr(0,lastslash);
	m_AssetsDir += L"\\Assets\\";
}

ant::DevelopmentResourceZipFile::~DevelopmentResourceZipFile()
{

}

bool ant::DevelopmentResourceZipFile::open()
{

}

int ant::DevelopmentResourceZipFile::getRawResourceSize( const Resource& ) 0
{

}

int ant::DevelopmentResourceZipFile::getRawResource( const Resource& r, char *buffer )
{

}

int ant::DevelopmentResourceZipFile::getNumResources() const
{

}

std::string ant::DevelopmentResourceZipFile::getResourceName( int i ) const
{

}

bool ant::DevelopmentResourceZipFile::isUsingDevelopmentDirectories( void )
{
	return true;
}
