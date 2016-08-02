#pragma once
#ifdef _DEBUG
#ifdef _WIN32
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif	// _WIN32
#endif  // _DEBUG

//#define _SFGPROFILE
//#define _DEBUG 1
//#define SFG_USE_CUSTOM_UI 1

//Basic
#include <stdio.h>
#include <stdlib.h>

//SFML
#include <SFML/Main.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#ifndef SFG_USE_CUSTOM_UI
#include <SFGUI/SFGUI.hpp>
#endif

//Standard
#include <iterator>
#include <functional>
#include <memory>
#include <cmath>
#include <locale>
//#include <codecvt>
#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include <thread>
#include <future>
#include <deque>
#include <queue>
#include <list>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>

//Debug
#include <assert.h>

//Custom
#include "Pointer.h"
#include "Rect.h"
#include "Util.h"
#include "Physics.h"
#include <omp.h>

//Developing
#ifdef _WIN32
#include <Windows.h>
#else
//For *nix, we need XInitThreads
//#include <X11/Xlib.h>
#endif

#ifndef _WIN32
#define memcpy_s(d, dsize, s, amount) memcpy(d, s, amount);
#endif // _WIN32

#ifndef PI
#define PI 3.1415926535897932384626433832795
#endif


#define SFML_ERASE_WHITESPACE(x) \
for (size_t i = 0; i < x.getSize();) \
{ \
	i = std::min({x.find(" ", i), x.find("\t", i), x.find("\n", i), x.find("\b", i), x.find("\r", i)}); \
	if (i == x.InvalidPos) break; \
	else \
	{ \
		x.erase(i, 1);\
	} \
}


//The default Event queue size
#define SFG_EVENT_QUEUE_SIZE 64

#define STILL_TO_DO(f, l) printf("[Development/Warning] Section of Code called that is marked as todo in %s:%d\n", f, l);
#define SFG_UNUSED_STR(param, f, l) if(sf::String(param) != "") printf("[Warning] String marked as unused was specified in %s:%d\n", __FILE__, __LINE__);

/*
sf::String s2ws(const std::string& str);

std::string ws2s(const sf::String& wstr);
*/
int basicLoadFile(const sf::String& xmlpath, sf::String& out);

int split(std::vector<sf::String>& out, sf::String& str);

#ifndef INT_MAX
#define INT_MAX (~int(0))/2 - 1
#endif // INT_MAX

#ifdef _WIN32
#pragma comment(lib, "Dbghelp.lib")
	#ifdef _DEBUG
	#pragma comment(lib, "sfml-system-d.lib")
	#pragma comment(lib, "sfml-main-d.lib")
	#pragma comment(lib, "sfml-graphics-d.lib")
	#pragma comment(lib, "sfml-window-d.lib")
	#pragma comment(lib, "sfml-network-d.lib")
	#else
	#pragma comment(lib, "sfml-main.lib")
	#pragma comment(lib, "sfml-graphics.lib")
	#pragma comment(lib, "sfml-system.lib")
	#pragma comment(lib, "sfml-window.lib")
	#pragma comment(lib, "sfml-network.lib")
	#endif
#endif