#pragma once
#include "stdafx.h"
#ifdef _WIN32
#include <Windows.h>
#else
//#pragma message("Warning: Untested on *nix. Please verify.")
#include <dlfcn.h>
#endif


namespace SFG
{
#ifdef _WIN32
	typedef FARPROC SFGFunc;
#else
	typedef void* SFGFunc;
#endif

	class DLL
	{
	public:
		DLL();
		~DLL();

		///<summary>
		///Loads a dynamic library
		///</summary>
		///<param name="path">The path to the library file.</param>
		bool load(const char* path);

		///<summary>
		///Resolve a function name from the loaded library.
		///</summary>
		///<param name="symname">The name of the function to extract</param>
		SFGFunc resolve(const char* symname);

		///<summary>
		///Closes the file.
		///</summary>
		void close();

		///<summary>
		///Unloads the DLL and locks the dll from all access other than hotpatch_reload().
		///</summary>
		void hotpatch_unload();

		///<summary>
		///Reloads DLL after hotpatch_unload() has unloaded it
		///</summary>
		bool hotpatch_reload();


		///<summary>
		///Gets a threadsafe pointer to the flagset
		///</summary>
		std::atomic<int32_t>* getDLLFlags()
		{
			return &this->m_dllflags;
		}

		///<summary>
		///Check if a dll is open or now
		///</summary>
		bool isOpen()
		{
			if (m_dll == NULL)
				return false;
			else
				return true;
		}

	private:
#ifdef _WIN32
		HMODULE m_dll;
#else
		void* m_dll;
#endif
		std::string m_path;
		std::mutex m_hotpatch_mutex;
		std::atomic<int32_t> m_dllflags;
	};

};
