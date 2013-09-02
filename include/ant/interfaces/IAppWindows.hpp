/*
* This is the abstract Windows app that uses the DIRECTX framework 
*/
#ifndef   IAPPWINDOWS_HPP
	#define   IAPPWINDOWS_HPP

//#include <ant/gui/UserInterface.hpp>
#include <ant/utils/WinInitialization.hpp>

namespace ant
{
	class IAppWindows
	{
	public:
		IAppWindows();
		virtual ~IAppWindows();

		enum Renderer
		{
			Renderer_Unknown,
			Renderer_D3D9,
			Renderer_D3D11
		};

		//////////////////////////////////////////////////////////////////////////
		// Main Loop
		//////////////////////////////////////////////////////////////////////////

		void abortGame() { m_bQuitting = true; }
		int getExitCode() { return DXUTGetExitCode(); }
		bool isRunning() { return m_bIsRunning; }
		void setQuitting(bool quitting) { m_bQuitting = quitting; }

		//////////////////////////////////////////////////////////////////////////
		// Virtual functions
		//////////////////////////////////////////////////////////////////////////

		// Game Application Data
		// You must define these in an inherited
		// class - see TeapotWarsApp for an example
		virtual TCHAR *getGameTitle()=0;
		virtual TCHAR *getGameAppDirectory()=0;
		virtual HICON getIcon()=0;
		virtual bool initInstance(HINSTANCE hInstance, LPWSTR lpCmdLine, HWND hWnd = NULL, int screenWidth = SCREEN_WIDTH, int screenHeight = SCREEN_HEIGHT);	

		// You must define these functions to initialize your game.
		// Look for them in TeapotWars.h and TeapotWars.cpp
		//virtual BaseGameLogic *VCreateGameAndView()=0;
		virtual bool loadGame(void);

		const Point &getScreenSize()  { return m_screenSize; }
		
		//int Modal(shared_ptr<IScreenElement> pModalScreen, int defaultAnswer);

		HWND getHwnd();
		HINSTANCE instance() { return m_hInstance; }
		bool hasModalDialog() { return m_HasModalDialog!=0; }
		//void forceModalExit() { PostMessage(getHwnd(), g_MsgEndModal, 0, g_QuitNoPrompt);	}

		LRESULT onAltEnter();

		//////////////////////////////////////////////////////////////////////////
		// Strings
		//////////////////////////////////////////////////////////////////////////

		bool loadStrings(std::string language);
		std::wstring getString(std::wstring sID);				
		int getHotKeyForString(std::wstring sID);
		UINT mapCharToKeycode(const char pHotkey);

		//////////////////////////////////////////////////////////////////////////
		// WIN32 HWND Callbacks
		//////////////////////////////////////////////////////////////////////////

		static LRESULT CALLBACK msgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void *pUserContext );

		LRESULT onDisplayChange(int colorDepth, int width, int height);
		LRESULT onPowerBroadcast(int event);
		LRESULT onSysCommand(WPARAM wParam, LPARAM lParam);
		LRESULT onClose();
		
		LRESULT onNcCreate(LPCREATESTRUCT cs);

		//////////////////////////////////////////////////////////////////////////
		// DIRECTX
		//////////////////////////////////////////////////////////////////////////

		// DirectX9 Specific Stuff
		static bool CALLBACK IsD3D9DeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext );
		static HRESULT CALLBACK OnD3D9CreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
		static HRESULT CALLBACK OnD3D9ResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
		static void CALLBACK OnD3D9FrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
		static void CALLBACK OnD3D9LostDevice( void* pUserContext );
		static void CALLBACK OnD3D9DestroyDevice( void* pUserContext );

		// DirectX 11 Specific Stuff
		static bool CALLBACK IsD3D11DeviceAcceptable( const CD3D11EnumAdapterInfo *AdapterInfo, UINT Output, const CD3D11EnumDeviceInfo *DeviceInfo, DXGI_FORMAT BackBufferFormat, bool bWindowed, void* pUserContext );
		static HRESULT CALLBACK OnD3D11CreateDevice( ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
		static HRESULT CALLBACK OnD3D11ResizedSwapChain( ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
		static void CALLBACK OnD3D11ReleasingSwapChain( void* pUserContext );
		static void CALLBACK OnD3D11DestroyDevice( void* pUserContext );
		static void CALLBACK OnD3D11FrameRender( ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, double fTime, float fElapsedTime, void* pUserContext );

		static bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext );
		static void CALLBACK OnUpdateGame( double fTime, float fElapsedTime, void *pUserContext );

	protected:

		int m_HasModalDialog;					// determines if a modal dialog is up
		int pumpUntilMessage (UINT msgEnd, WPARAM* pWParam, LPARAM* pLParam);
		int	eatSpecificMessages( UINT msgType, optional<LPARAM> lParam, optional<WPARAM> wParam);
		void flashWhileMinimized();

	private:

		//////////////////////////////////////////////////////////////////////////
		// Variables and structures
		//////////////////////////////////////////////////////////////////////////
	public:
		// Renderer
		//shared_ptr<IRenderer> m_Renderer;
		static Renderer GetRendererImpl();

		// GameCode Specific Stuff
		//BaseGameLogic *m_pGame;
		//struct GameOptions m_Options;

		// File and Resource System
		//class ResCache *m_ResCache;
		TCHAR m_saveGameDirectory[MAX_PATH];

		// Event manager
		//EventManager *m_pEventManager;

		// Socket manager - could be server or client
		//BaseSocketManager *m_pBaseSocketManager;
		//NetworkEventForwarder* m_pNetworkEventForwarder;

	protected:

		HINSTANCE m_hInstance;					// the module instance
		bool m_bWindowedMode;					// true if the app is windowed, false if fullscreen
		bool m_bIsRunning;						// true if everything is initialized and the game is in the main loop
		bool m_bQuitRequested;					// true if the app should run the exit sequence
		bool m_bQuitting;						// true if the app is running the exit sequence
		Rect m_rcDesktop;						// current desktop size - not necessarilly the client window size
		Point m_screenSize;					// game screen size
		int m_iColorDepth;						// current color depth (16 or 32)
		bool m_bIsEditorRunning;				// true if the game editor is running

		std::map<std::wstring,std::wstring> m_textResource; // Holds text strings for the game
		std::map<std::wstring,UINT> m_hotkeys;							// Hotkeys for the game

	};

	extern IAppWindows *g_pApp;
}

#endif