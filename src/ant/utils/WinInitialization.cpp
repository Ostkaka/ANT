#include <ant/ant_std.hpp>
#include <shlobj.h>
#include <direct.h>
#include <ant/utils/WinInitialization.hpp>

using namespace ant;


GameOptions::GameOptions()
{
	// set all the options to decent default valu
	m_Level = "";
	m_Renderer = "Direct3D 9";
	m_runFullSpeed = false;
	m_soundEffectsVolume = 1.0f;			
	m_musicVolume = 1.0f;				
	m_expectedPlayers = 1;
	m_listenPort = -1;					
	std::string m_gameHost = "MrMike-m1710";
	m_numAIs = 1;
	m_maxAIs = 4;
	m_maxPlayers = 4;
	m_ScreenSize = Point(1024,768);
	m_useDevelopmentDirectories = false;

	m_pDoc = NULL;
}

// TODO - Check agx config manager for better ways to do this
void GameOptions::Init(const char* xmlFileName, LPWSTR lpCmdLine)
{

	// read the XML file
	// if needed, override the XML file with options passed in on the command line.

	m_pDoc = new TiXmlDocument(xmlFileName);
	if (m_pDoc && m_pDoc->LoadFile())
	{
		TiXmlElement *pRoot = m_pDoc->RootElement();
		if (!pRoot)
			return;

		// Loop through each child element and load the component
		TiXmlElement* pNode = NULL;
		pNode = pRoot->FirstChildElement("Graphics"); 
		if (pNode)
		{
			std::string attribute;
			attribute = pNode->Attribute("renderer");
			if (attribute != "Direct3D 9" && attribute != "Direct3D 11")
			{
				GCC_ASSERT(0 && "Bad Renderer setting in Graphics options.");
			}
			else
			{
				m_Renderer = attribute;
			}

			if (pNode->Attribute("width"))
			{
				m_ScreenSize.x = atoi(pNode->Attribute("width"));
				if (m_ScreenSize.x < 800) m_ScreenSize.x = 800;
			}

			if (pNode->Attribute("height"))
			{
				m_ScreenSize.y = atoi(pNode->Attribute("height"));
				if (m_ScreenSize.y < 600) m_ScreenSize.y = 600;
			}

			if (pNode->Attribute("runfullspeed"))
			{
				attribute = pNode->Attribute("runfullspeed");
				m_runFullSpeed = (attribute == "yes") ? true : false;
			}
		}

		pNode = pRoot->FirstChildElement("Sound"); 
		if (pNode)
		{
			m_musicVolume = atoi(pNode->Attribute("musicVolume")) / 100.0f;
			m_soundEffectsVolume = atoi(pNode->Attribute("sfxVolume")) / 100.0f;
		}

		pNode = pRoot->FirstChildElement("Multiplayer"); 
		if (pNode)
		{
			m_expectedPlayers = atoi(pNode->Attribute("expectedPlayers"));
			m_numAIs = atoi(pNode->Attribute("numAIs"));
			m_maxAIs = atoi(pNode->Attribute("maxAIs"));
			m_maxPlayers = atoi(pNode->Attribute("maxPlayers"));

			m_listenPort = atoi(pNode->Attribute("listenPort"));
			m_gameHost = pNode->Attribute("gameHost");
		}

		pNode = pRoot->FirstChildElement("ResCache");
		if (pNode)
		{
			std::string attribute(pNode->Attribute("useDevelopmentDirectories"));
			m_useDevelopmentDirectories = ((attribute == "yes") ? (true) : (false));
		}
	}
}

bool CheckStorage(const DWORDLONG diskSpaceNeeded)
{
	// Check for enough free disk space on the current disk.
	int const drive = _getdrive();
	struct _diskfree_t diskfree;

	_getdiskfree(drive, &diskfree);

	unsigned __int64 const neededClusters = 
		diskSpaceNeeded /(diskfree.sectors_per_cluster*diskfree.bytes_per_sector);

	if (diskfree.avail_clusters < neededClusters)
	{
		// if you get here you don�t have enough disk space!
		GCC_ERROR("CheckStorage Failure: Not enough physical storage.");
		return false;
	}
	return true;
}

//
// CheckMemory							- Chapter 5, page 139
//
bool CheckMemory(const DWORDLONG physicalRAMNeeded, const DWORDLONG virtualRAMNeeded)
{
	MEMORYSTATUSEX status;
	GlobalMemoryStatusEx(&status);
	if (status.ullTotalPhys < physicalRAMNeeded)
	{
		// you don�t have enough physical memory. Tell the player to go get a real 
		// computer and give this one to his mother. 
		GCC_ERROR("CheckMemory Failure: Not enough physical memory.");
		return false;
	}

	// Check for enough free memory.
	if (status.ullAvailVirtual < virtualRAMNeeded)
	{
		// you don�t have enough virtual memory available. 
		// Tell the player to shut down the copy of Visual Studio running in the
		// background, or whatever seems to be sucking the memory dry.
		GCC_ERROR("CheckMemory Failure: Not enough virtual memory.");
		return false;
	}

	char *buff = GCC_NEW char[(unsigned int)virtualRAMNeeded];
	if (buff)
		delete[] buff;
	else
	{
		// even though there is enough memory, it isn�t available in one 
		// block, which can be critical for games that manage their own memory
		GCC_ERROR("CheckMemory Failure: Not enough contiguous available memory.");
		return false;
	}
	return true;
}

//
// ReadCPUSpeed							- Chapter 5, page 140
//
DWORD ReadCPUSpeed()
{
	DWORD BufSize = sizeof(DWORD);
	DWORD dwMHz = 0;
	DWORD type = REG_DWORD;
	HKEY hKey;

	// open the key where the proc speed is hidden:
	long lError = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
        L"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
        0, KEY_READ, &hKey);
    
	if(lError == ERROR_SUCCESS)
	{
		// query the key:
		RegQueryValueEx(hKey, L"~MHz", NULL, &type, (LPBYTE) &dwMHz, &BufSize);
	}
	return dwMHz;
}

//
// IsOnlyInstance							- Chapter 5, page 137
//
bool IsOnlyInstance(LPCTSTR gameTitle)
{
	// Find the window.  If active, set and return false
	// Only one game instance may have this mutex at a time...

	HANDLE handle = CreateMutex(NULL, TRUE, gameTitle);

	// Does anyone else think 'ERROR_SUCCESS' is a bit of an oxymoron?
	if (GetLastError() != ERROR_SUCCESS)
	{
		HWND hWnd = FindWindow(gameTitle, NULL);
		if (hWnd) 
		{
			// An instance of your game is already running.
			ShowWindow(hWnd, SW_SHOWNORMAL);
			SetFocus(hWnd);
			SetForegroundWindow(hWnd);
			SetActiveWindow(hWnd);
			return false;
		}
	}
	return true;
}

//
// GetSaveGameDirectory - Chapter 5, page 146
//
const TCHAR *GetSaveGameDirectory(HWND hWnd, const TCHAR *gameAppDirectory)
{
	HRESULT hr;
	static TCHAR m_SaveGameDirectory[MAX_PATH];
	TCHAR userDataPath[MAX_PATH];

	hr = SHGetSpecialFolderPath(hWnd, userDataPath, CSIDL_APPDATA, true);

	_tcscpy_s(m_SaveGameDirectory, userDataPath);
	_tcscat_s(m_SaveGameDirectory, _T("\\"));
	_tcscat_s(m_SaveGameDirectory, gameAppDirectory);

	// Does our directory exist?
	if (0xffffffff == GetFileAttributes(m_SaveGameDirectory))
	{
		if (SHCreateDirectoryEx(hWnd, m_SaveGameDirectory, NULL) != ERROR_SUCCESS)
			return false;
	}
		
	_tcscat_s(m_SaveGameDirectory, _T("\\"));

	return m_SaveGameDirectory;
}

//
// bool CheckForJoystick							- Chapter 9, page 245
//
bool CheckForJoystick(HWND hWnd)
{
	JOYINFO joyinfo; 
	UINT wNumDevs; 
	BOOL bDev1Attached, bDev2Attached; 
 
    if((wNumDevs = joyGetNumDevs()) == 0) 
        return false; 
    bDev1Attached = joyGetPos(JOYSTICKID1,&joyinfo) != JOYERR_UNPLUGGED; 
    bDev2Attached = joyGetPos(JOYSTICKID2,&joyinfo) != JOYERR_UNPLUGGED; 
    if(bDev1Attached)
		joySetCapture(hWnd, JOYSTICKID1, 1000/30, true);   
	if (bDev2Attached)   
		joySetCapture(hWnd, JOYSTICKID2, 1000/30, true);   

	return true;
}
