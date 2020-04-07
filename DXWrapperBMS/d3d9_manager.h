#pragma once

#include "d3d9_main.h"
#include "F4IReader.h"

#include <chrono>
#include <string>

/*
	TODO:
		- Change output to a function with a strategy: void setText() which calls the setConsoleTitle()????
		- Consider taking input from the BMS command line: -DXWrapperBMS_PollRate=50
*/

class d3d9_manager
{
private:

	// Constants
	inline static const char *m_szVersion = "0.0.3-alpha.1";
	inline static const char* szRealDLLPath = "C:\\Windows\\system32\\d3d9.dll";
	//const int dwSleepBeforeStopDrawing = 1000; // This is a delay so the admin can see that the server made it to 3D. This seems like a poor choice. Should probably blank the screen with some color instead to make our point.
	constexpr static auto PollMemoryRate = std::chrono::seconds(5);

	///////////////
	// Variables //
	///////////////

	// DLL Variables
	HINSTANCE hinstThisDLL;
	HINSTANCE hinstRealDLL;
	bool bDynamicallyLoaded;
	fnDirect3DCreate9 fnCreate9;
	fnDirect3DCreate9Ex fnCreate9Ex;

	// D3D9 variables
	bool bDraw;

	// Init variables
	bool bInit;

	// Object variables
	F4IReader* memReader;
	std::thread *m_thread;

	d3d9_manager()
	{
		// Note: This object should NOT 

		// Initialize variables to 0/false/NULL
		bInit = false;
		fnCreate9 = NULL;
		fnCreate9Ex = NULL;
		bDynamicallyLoaded = false;
		hinstThisDLL = NULL;
		hinstRealDLL = NULL;
		//pd3d9 = NULL;
		memReader = NULL;
		m_thread = NULL;
		// Initialize variables to 1/true
		bDraw = true;
	}

	// Private initialization functions
	void initEnvironment();
	void loadRealDLL();

public:
	static d3d9_manager& getManager()
	{
		// This singleton creation strategy is supposed to be thread-safe as of C++11 and VS2015. (Woo!)
		static d3d9_manager singleton;
		return singleton;
	}

	static const std::string& getVersion()
	{
		static const std::string version = m_szVersion;
		return version;
	}

	static BOOL WINAPI CtrlHandler(DWORD dwCtrlType);

	// Deleting constructor copy and assign. Singleton pattern, baby.
	d3d9_manager(d3d9_manager const&) = delete;
	void operator=(d3d9_manager const&) = delete;

	// Initialization functions
	void setDLL(HINSTANCE hinstDLL, DWORD dwReason, LPVOID lpvReserved);
	//void setD3D9(D9Wrapper::IDirect3D9* pd3d9);

	// Functions to get function pointers to the real DirectX Create* functions.
	fnDirect3DCreate9 getRealCreate9fn();
	fnDirect3DCreate9Ex getRealCreate9Exfn();

	// Function to obtain a pointer to the existing IDirect3D9 object.
	// Note: This may not be useful in the future.
	/*
	D9Wrapper::IDirect3D9* getD3D9()
	{
		return pd3d9;
	}
	*/

	// Sets drawing conditions. true = drawing is enabled; false = drawing is disabled.
	void setDraw(bool bDraw);
	// Gets the draw conditions. true = functions calls from BMS to DirectX should be passed through; false = functions calls from BMS to DirectX should be ignored.
	bool shouldDraw();

	// This is an FPS tracker.
	//FPSTracker& getFPS()
	//{
	//	return fps;
	//}

	// Establishes 
	void establishIO();
	void poll3DEnvironment();
};