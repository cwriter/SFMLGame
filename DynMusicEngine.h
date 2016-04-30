#pragma once
#include "stdafx.h"
#include "rtmidi-2.1.0\RtMidi.h"
#include <Windows.h>

#ifdef _WIN32
#pragma comment(lib, "winmm.lib")
#endif

#ifdef _DEBUG
#pragma comment(lib, "rtmidid.lib")
#else
#pragma comment(lib, "rtmidi.lib")
#endif

struct Note {
	sf::Int64 time;
	unsigned char on;
	unsigned char pitch;
	unsigned char force;
};

class DynMusicEngine
{
public:
	DynMusicEngine();
	~DynMusicEngine();

	int start() {
		m_mout = new RtMidiOut();
		if (m_mout->getPortCount() == 0) {
			printf("[Error] in %s:%d: Failed to find usable midi device.\n", __FILE__, __LINE__);
			return -1;
		}
		else 
			printf("[Info] %d port available\n", m_mout->getPortCount());
		printf("[Info] Trying to open port name \"%s\"\n", m_mout->getPortName(0).c_str());
		try {
			m_mout->openPort(0);
		}
		catch (RtMidiError& err) {
			printf("[Error] %s", err.what());
			return -2;
		}

		
		printf("[Info] DynMusicEngine successfully started.\n");

		m_timer.restart();	//Start timer
		
		auto hand = std::async(std::launch::async, &DynMusicEngine::Thread, this);
		//Thread();
		

		return 0;
	}

	int Thread() {
		//This thread is the music (not necessarily the sound thread)
		while (1) {
			if (this->m_tmp.size() == 0) {
				//Nothing left, quit this thread
				return 1;
			}
			std::vector<unsigned char> message;
			sf::Int64 usecs = m_timer.getElapsedTime().asMicroseconds();
			if (this->m_tmp.front()->time <= usecs) {
				//Needs to be played
				message.push_back(m_tmp.front()->on);
				message.push_back(m_tmp.front()->pitch);
				message.push_back(m_tmp.front()->force);

				try {
					m_mout->sendMessage(&message);
				}
				catch (RtMidiError& err) {
					err.printMessage();
				}

				delete this->m_tmp.front();
				this->m_tmp.pop_front();
			}

		}
		return 0;
	}

	int create(const char* seed) {
		for (size_t i = 0; i < 6; i++) {
			Note* n = new Note;
			if (n == NULL) {
				printf("[Error] Failed to allocate memory in %s:%d\n", __FILE__, __LINE__);
			}

			if (i < 3) {
				n->on = (unsigned char)144;
				n->time = 0;
			}
			else {
				n->on = 128;
				n->time = 3000000;
			}
			
			switch (i % 3) {
			case 0:
				n->pitch = 48;
				break;
			case 1:
				n->pitch = 52;
				break;
			case 2:
				n->pitch = 55;
				break;
			}
			n->force = 103;
			
			this->m_tmp.push_back(n);

		}
		return 0;
	}

	int play(const char* seed) {
		std::vector<unsigned char> message;

		message.push_back(144);
		message.push_back(64);
		message.push_back(90);

		m_mout->sendMessage(&message);

		Sleep(1000);

		message[0] = 128;
		message[1] = 64;
		message[2] = 40;

		m_mout->sendMessage(&message);

		return 0;
	}

	int end() {
		m_mout->closePort();
		delete m_mout;
		return 0;
	}
private:
	RtMidiOut* m_mout;
	sf::Clock m_timer;
	std::deque<Note*> m_tmp;
};

