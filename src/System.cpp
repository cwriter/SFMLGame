#include "stdafx.h"
#include "System.h"
#include "Profiler.h"

namespace SFG
{

	System::System()
	{
		m_running = true;
		m_thread_future = std::async(&System::watch_thread, this); //Start the thread
	}

	System::~System()
	{
		this->m_running = false; //Set to let the thread exit gracefully
		//this->continue_thread(); //Don't block otherwise, just rush to an end
		assert(m_thread_future.valid());
		m_thread_future.get();
	}

	int System::CreateID()
	{
		if (last_id == INT_MAX - 1)
		{
			//This should never happen. The game would have to have been running for quite some time...
			printf("Congratulations! We've reached %d async system calls! %s:%d\n", last_id, __FILE__, __LINE__);
			last_id = 0;
		}
		return last_id++;
	}

	int System::readfile(SystemClient* client, const char* path, size_t start, size_t count)
	{
		//Get the ID for this task
		int ID = CreateID();
		//Create our file event
		SFG::Pointer<FileEvent> fe(new FileEvent);
		fe->ID = ID;
		fe->done = std::bind(&SystemClient::IDUnlocked, client, ID);
		client->addID(ID, fe);
		//Everything has been set

		//Get the map lock
		m_watch_thread_mutex.lock();

		//Add element to map
		m_pending[ID] = std::async([=]() {
			//First, create a FILE instance
			File f;
			//Open the file (default rb as we expect all files to be unix-ln-encoded
			if (f.open(path, "rb") != File::Success)
			{
				//Error

			}
			//Get to the starting position
			if (f.seek(long(start), SEEK_SET) != start)
			{
				//Error
			}


			fe->datasize = count;
			fe->data = (char*)malloc(fe->datasize);
			if (fe->data == nullptr)
			{
				//Error
			}

			//Read the requested count
			if (f.read(fe->data, count) != count)
			{
				//Error (or all read)
			}
			return SFG::Pointer<Event>(fe);
		});

		//Unlock immediately
		m_watch_thread_mutex.unlock();

		return ID;
	}

	int System::watch_thread()
	{
		while (this->m_running)
		{
			SFG_PROFILE_START
			m_watch_thread_mutex.lock(); //Take it from the calling thread
			for (auto& e : m_pending)
			{
				if (e.second.wait_for(std::chrono::milliseconds(1)) == std::future_status::ready)
				{
					//Go call finisher functions
					auto v = e.second.get();
					v->done((void*)&v->ID); //Blocking. Should not take too long
										   //Delete from watchlist
					m_pending.erase(v->ID);
					break;
				}
			}
			m_watch_thread_mutex.unlock(); //Hand it off to the calling thread
			std::this_thread::yield(); //Let the other threads work
			std::this_thread::sleep_for(std::chrono::milliseconds(30)); //Some arbitrary waiting time to allow locking
			SFG_PROFILE_END(__FILE__)
		}
		return 0;
	}
}