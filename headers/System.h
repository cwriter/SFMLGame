#pragma once
#include "stdafx.h"
#include <stdint.h>
#include <inttypes.h>



#ifndef MAXSIZE_T
#define MAXSIZE_T size_t(~(0))
#endif

namespace SFG
{
class File;


///<summary>
///Wraps system events.
///</summary>
class Event
{
public:
    Event()
    {
        progress = -1; //Indeterminate
        max_progress = 100;
    }

    std::function<void(void*)> done; //Syntax

    int ID;

    int progress;
    int max_progress;

    void setDone(void* r) {
        return done(r);
    }
private:
};

class FileEvent : public Event
{
public:

    char* data;
    size_t datasize;
};
///<summary>
///Wraps a system file
///</summary>
class File
{
public:
    enum
    {
        Success,
        OpenFailed,

    };
    int open(const char* path, const char* mode)
    {
#ifdef _WIN32
        fopen_s(&m_file, path, mode);
#else
        m_file = fopen(path, mode);
#endif
        if (m_file == NULL)
            return OpenFailed;
        return Success;
    }

    void close()
    {
        if (m_file != NULL)
        {
            fclose(m_file);
            m_file = NULL;
        }
    }

    long size()
    {
        long oldcur = ftell(m_file);
        fseek(m_file, 0, SEEK_END);
        long ret = ftell(m_file);
        fseek(m_file, oldcur, SEEK_SET); //Reset to old value
        return ret;
    }

    size_t read(char* out, size_t bytecount)
    {
        if (m_file == NULL)	return 0;
        return fread(out, sizeof(char), bytecount, m_file);
    }

    long seek(long offset, int start)
    {
        if (m_file == NULL)	return 0;
        return fseek(m_file, offset, start);
    }


private:

    FILE* m_file; //Actual system-filepointer

};

///<summary>
///Defines a group of tasks that can be executed asynchronously to the main thread but in correct order relative to the task
///</summary>
class Task
{
public:

private:
    std::vector<std::function<void*(void*)>> m_subtasks;
};

///<summary>
///Class for handling file chunks of any given size <= 2GB
///</summary>
class FileChunk
{
public:
    FileChunk()
    {
        m_size = 0;
        m_ptr = nullptr;
    }

    FileChunk(size_t size) : m_size(size)
    {
        assert(size > 0);
        m_ptr = (char*)malloc(size);
        if (m_ptr == nullptr)
        {
            printf("[Critical] Failed to allocate memory for FileChunk (%zd requested) in %s:%d\n", size, __FILE__, __LINE__);
            m_size = 0;
        }
    }
    ///<summary>
    ///Copy 'amount' bytes from source to this. This is a deep copy
    ///</summary>
    ///<param name="source">The source of the data</param>
    ///<param name="amount">The count of bytes to copy. If set to 0 or if amount is greater than the chunk size,
    ///the function will copy as many bytes as it can hold</param>
    void copy(const char* source, size_t amount = 0)
    {
        assert(source != nullptr);
        assert(m_ptr != nullptr);
        if (amount > m_size)
        {
            //Put out a warning
            printf("[Warning] Cannot copy the amount specified (%zd requested, %zd possible). Copying the maximum amount possible %s:%d\n", amount, m_size, __FILE__, __LINE__);
        }

        if (amount == 0 || amount > m_size) amount = m_size;
#ifdef _WIN32
        memcpy_s(m_ptr, m_size, source, amount);
#else
        assert(amount <= m_size);
        memcpy(m_ptr, source, amount);
#endif
    }

    ///<summary>
    ///Grants access to the data stored.
    ///</summary>
    ///<param name="size">If 'size' points to a size_t, the data at 'size' will be filled with the chunk size</param>
    inline char* data(size_t* size = NULL) const
    {
        if (size != nullptr)
        {
            *size = m_size;
        }
        return this->m_ptr;
    }

    ///<summary>
    ///Reallocs the array. If the reallocation fails, data is preserved.
    ///</summary>
    ///<returns>False if the reallocation failed, true otherwise.</returns>
    bool recreate(size_t size)
    {
        assert(size != 0);
        char* tmp = (char*)realloc(m_ptr, size);
        if (tmp == NULL)
        {
            //Allocation failed
            return false;
        }
        else
        {
            //Allocation successful
            m_ptr = tmp;
            this->m_size = size;
        }
        return true;
    }

    void clear()
    {
        if (m_ptr != nullptr) free(m_ptr);
        m_size = 0;
        m_ptr = nullptr;
    }

    ~FileChunk()
    {
        clear();
    }

    inline size_t size() {
        return this->m_size;
    }
private:
    char* m_ptr;
    size_t m_size;
};

///<summary>
///A system client (A class that uses System calls has to inherit from this class)
///</summary>
class SystemClient
{
public:
    bool IDReady(int ID, float* progress = nullptr)
    {
		std::lock_guard<std::mutex> g(m_mutex);
        auto it = m_pending.find(ID);
        if (it != m_pending.end())
        {
            return true;
        }
        if (progress != nullptr)
        {
            it = m_pending.find(-ID);
            if (it != m_pending.end())
            {
                if (it->second->progress == -1) //Indeterminate
                    *progress = -1.f;
                else
                    *progress = float(it->second->progress * 100) / float(it->second->max_progress); //Exact
            }
            else
            {
                //Task doesn't even exist
                printf("[Warning] Asked for Task with ID %d, but this task hasn't been scheduled in %s:%d\n", ID, __FILE__, __LINE__);
            }
        }

        return false;
    }
    ///<summary>
    ///Function that gets called when the ID is signaled ready by the system.
    ///</summary>
    void IDUnlocked(int ID)
    {
        //Lock mutex to disallow any access on the map
        m_mutex.lock();
        //Our ID got unlocked, we can now use the data
        auto it = m_pending.find(-ID);
        if (it == m_pending.end())
        {
            printf("[Error] Event %d was expected to exist, but does not.\n", ID);
        }
        else
        {
            m_pending[ID] = it->second;
            m_pending.erase(it);
        }
        //Unlock mutex to allow others to do their job
        m_mutex.unlock();
    }

    void addID(int ID, const SFG::Pointer<SFG::Event>& ev)
    {
		m_mutex.lock();
        m_pending[-ID] = ev;
		m_mutex.unlock();
    }

    void removeID(int ID)
    {
		m_mutex.lock();
        m_pending.erase(ID);
		m_mutex.unlock();
    }
private:
    std::mutex m_mutex;
    std::map<int, SFG::Pointer<SFG::Event>> m_pending;
};

///<summary>
///Wraps system calls like file operations asynchronously to keep the experience as responsive as possible
///</summary>
class System
{
public:
    System();
    ~System();


    void halt_thread()
    {
        m_watch_thread_mutex.lock();
    }

    void continue_thread()
    {
        m_watch_thread_mutex.unlock();
    }

    ///<summary>
    ///Runs and watches whether a task has been successfully ended.
    ///</summary>
    int watch_thread();

    ///<summary>
    ///Create unique ID for a specific task.
    ///</summary>
    int CreateID();

    ///<summary>
    ///Read a file asynchronously.
    ///</summary>
    int readfile(SystemClient* client, const char* path, size_t start, size_t count);

    ///<summary>
    ///Same as "int readfile(SystemClient* client, const char* path, size_t start, size_t count)", but reads everything
    ///</summary>
    int readfile(SystemClient* client, const char* path)
    {
        return readfile(client, path, 0, MAXSIZE_T);
    }
private:
    bool m_running;
    std::future<int> m_thread_future;
    std::mutex m_watch_thread_mutex; //The watch thread mutex
    std::map<int, std::future<SFG::Pointer<SFG::Event>>> m_pending;
    int last_id;
};

}
