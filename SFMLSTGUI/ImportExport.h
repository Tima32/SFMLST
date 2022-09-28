#pragma once
#ifdef SFMLSTGUI_EXPORTS
	#define SF_ST_GUI_EXPORT __declspec(dllexport)
#else
	#define SF_ST_GUI_EXPORT __declspec(dllimport)

	#ifdef _WIN64
		#pragma comment(lib,"SFMLSTGUIx64.lib")
	#else
		#pragma comment(lib,"SFMLSTGUIx32.lib")
	#endif
#endif

// For Visual C++ compilers, we also need to turn off this annoying C4251 warning
#ifdef _MSC_VER
	#pragma warning(disable: 4251)
#endif