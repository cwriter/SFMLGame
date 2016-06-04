#include "stdafx.h"
#include "DLL.h"


namespace SFG
{
DLL::DLL()
{
#ifdef _WIN32
    m_dll = 0;
#else
    m_dll = nullptr;
#endif
}


DLL::~DLL()
{
    close();
}

bool DLL::load(const char* path)
{
    close();
#ifdef _WIN32
    m_dll = LoadLibraryA(path);
#else
    m_dll = dlopen(path, RTLD_LAZY);
#endif
    if (m_dll == NULL)
    {
        printf("[Error] Failed to load library \"%s\" in %s:%d\n", path, __FILE__, __LINE__);
        return false;
    }
    m_path = std::string(path);
    return true;
}

bool DLL::hotpatch_reload()
{
    //Load the DLL
    if (!load(m_path.c_str()))
    {
        printf("[Error] Failed to hotpatch_reload() in %s:%d\n", __FILE__, __LINE__);
        return false;
    }

    //Finally, unlock the mutex
    m_hotpatch_mutex.unlock();

    return true;
}

void DLL::hotpatch_unload()
{
    //Check if the dll is even loaded
    if (m_dll == NULL)
    {
        return;
    }
    //Lock the mutex
    while (!m_hotpatch_mutex.try_lock())
    {
        //Just wait
        std::this_thread::yield();
    }
    //Now we got the mutex, so let's start working

    //Close the dll
    close();

    //We're done here. It is required to call hotpatch_reload after this function has been called!

}

SFGFunc DLL::resolve(const char* symname)
{
    //Lock
    std::lock_guard<std::mutex> lck(m_hotpatch_mutex);

    if (m_dll == NULL)
    {
        printf("[Critical] SFG::DLL::resolve called before loading a library in %s:%d\n",__FILE__, __LINE__);
        return 0;
    }
#ifdef _WIN32
    return GetProcAddress(m_dll, symname);
#else
    return dlsym(m_dll, symname);
#endif
}

void DLL::close()
{
    //Don't do anything when the file is already closed
    if (m_dll == NULL) return;
#ifdef _WIN32
    FreeLibrary(m_dll);
#else
    dlclose(m_dll);
#endif
    m_dll = NULL;
}
};
