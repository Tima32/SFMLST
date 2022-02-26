#pragma once
#ifdef SFMLSTGUI_EXPORTS
	#define SF_ST_GUI_EXPORT __declspec(dllexport)
#else
	#define SF_ST_GUI_EXPORT __declspec(dllimport)
	#pragma comment(lib,"SFMLSTGUI.lib")
#endif

// For Visual C++ compilers, we also need to turn off this annoying C4251 warning
#ifdef _MSC_VER
	#pragma warning(disable: 4251)
#endif