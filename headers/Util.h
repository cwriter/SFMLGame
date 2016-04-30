#pragma once
#include "stdafx.h"
#include <stdarg.h>
#ifdef _WIN32
#include <Windows.h>
#include <DbgHelp.h>
#endif

namespace SFG
{
	///<summary>
	///Provides several miscellaneous functions for debugging
	///</summary>
	class Util
	{
	public:
		Util();
		~Util();

		enum LogMessageType {
			Error = 0,
			Warning,
			Development,
			Information
		};

		///<summary>
		///Prints a stacktrace to the given stream
		///</summary>
		static void getStackTrace(std::string& stream);

		///<summary>
		///
		///</summary>
		static void printLog(LogMessageType t, const char* file, int line, const char* format, ...);
	private:
	};

};
