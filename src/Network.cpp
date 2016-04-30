#include "stdafx.h"
#include "Network.h"

namespace SFG
{

	/*Network::Network()
	{
	}


	Network::~Network()
	{
	}*/

	bool TaskIdentifier::operator<(const TaskIdentifier& other) const
	{
		//First priority: Address
		if (address.toInteger() < other.address.toInteger())
		{
			return true;
		}
		else if (address.toInteger() > other.address.toInteger())
		{
			return false;
		}

		//Second priority: Purpose
		if (purpose < other.purpose) return true;
		else if (purpose > other.purpose) return false;

		//Third: Port
		if (port < other.port) return true;
		else if (port > other.port) return false;

		//Default:
		return true;
	}

	int Network::init(int port_tcp, int port_udp)
	{
		//Set the port numbers
		this->m_udp_port = port_udp;
		this->m_tcp_port = port_tcp;

		//Bind
		if (this->m_udp_sock.bind(port_udp) != sf::UdpSocket::Done)
		{
			//Some Error, please handle
		}
		//(TCP will listen to the point set when it actually should)



		return 0;
	}

	int Network::checkNetworkVersion(sf::Int32 Version)
	{
		if (Version != NETWORK_VERSION)
		{
			if (Version >> 16 != NETWORK_VERSION >> 16)
			{
				//Major Version discrepancy
				return -1;
			}
			else
			{
				//Minor version discrepancy
				return 1;
			}
		}
		else
		{
			return 0; //Versions agree
		}
	}

	int Network::getDownloadInfo(sf::Packet& pkg, const TaskIdentifier& sender)
	{
		//Handle response
		sf::Int32 purpose = 0;
		sf::Uint64 filesize = 0;
		bool useTcp = false;
		sf::Uint16 portToUse = 0;
		sf::String remfilename;
		//Format: Purpose::DownloadInfo >> <remote filename> >> <sf::Uint64 filesize> >> <Use TCP?> >> <port to use>
		if (pkg >> purpose >> remfilename >> filesize >> useTcp >> portToUse)
		{
			//Work with the knowledge we've gotten
			if (purpose != Purpose::DownloadInfo)
			{
				//Uhm, we're wrong in here...
				return -3;
			}
			//Check information
			SFG::Download dwnld;
			dwnld.filesize = filesize;
			dwnld.remotename = remfilename;
			dwnld.state = SFG::Download::State::Pending;

			//Set download task
			int ID = this->CreateID();

			this->m_downloads[ID] = dwnld;

			//Send starting identifer
			sf::Packet sdp;
			//Format: Purpose::DownloadInfo << <remote file name> << <ID>				
			// We need to send an ID so the server can include that ID in his responses to identify the target
			sdp << sf::Int32(Purpose::StartDownload) << remfilename << ID;

			//Add download task
			auto f = std::bind(&Network::getFileChunk, this, std::placeholders::_1, std::placeholders::_2);
			addTaskWaiter(sf::Int32(Purpose::FileChunk), sender.address, portToUse, f);
			

			//Send to server (same address as the address we received the message from, but with the desired port)
			int ret = 0;
			if (!useTcp)
			{
				 ret = this->m_udp_sock.send(sdp, sender.address, portToUse);
			}
			else
			{
				//Needs to be connected
				ret = this->m_tcp_sock.send(sdp);
			}
			if (ret == sf::Socket::Done)
			{
				//Everything's ok
			}
			else
			{
				printf("[Error] Code %d when sending socket in %s:%d\n", ret, __FILE__, __LINE__);
			}
		}
		else
		{
			//Failed to receive everything we needed...
			printf("[Error] Data of DownloadInfo-Package is corrupted: %s:%d\n", __FILE__, __LINE__);
		}

		return 0;
	}

	int Network::verifyEndOfDownload(sf::Packet& pkg, const TaskIdentifier& sender)
	{
		//Format: <Purpose::EndOfDownload> << <ID of task>
		sf::Int32 p = 0;
		sf::Int32 ID = 0;
		if (pkg >> p >> ID)
		{
			//Check if the purpose is correct
			if (p != Purpose::EndOfDownload)
			{
				printf("[Error] Purpose not correct in %s:%d\n", __FILE__, __LINE__);
				return -1;
			}
			
			//Now evaluate the download
			auto it = m_downloads.find(ID);
			if (it == m_downloads.end())
			{
				//Not found
				printf("[Error] Download task with ID %d referenced but not available in %s:%d\n", ID, __FILE__, __LINE__);
				return -2;
			}
			if (it->second.isComplete())
			{
				//We're completely done, now tell the server we don't need any resource anymore
				sf::Packet packet;
				//Format is simple: <Purpose::DownloadComplete> << <ID>
				packet << sf::Int32(Purpose::DownloadComplete) << ID;

				auto ret = m_udp_sock.send(packet, sender.address, sender.port);
				if (ret == sf::Socket::Done)
				{
					//All gud
				}
				else
				{
					printf("[Error] Failed to send UDP-Socket (error: %d) in %s:%d\n", ret, __FILE__, __LINE__);
				}

				return 0;
			}
			else
			{
				//We're missing something; but no worries: We can request the missing parts
				
				//First, find out what's actually missing
				auto li = it->second.getIncompleteChunks();
				//Create a packet
				sf::Packet req;
				//Format: <Purpose::SpecificFileChunkRequest> << <Count of filechunks> << <Filechunk number>...
				req << sf::Int32(Purpose::SpecificFileChunkRequest) << sf::Uint32(li.size());
				for (auto p : li)
				{
					req << p; //Load chunks numbers into packet
				}

				//Aaaand out it goes
				auto ret = this->m_udp_sock.send(req, sender.address, sender.port);
				if (ret == sf::Socket::Done)
				{
					//All gud.
				}
				else
				{
					printf("[Error] Failed to send UDP-Socket (error: %d) in %s:%d\n", ret, __FILE__, __LINE__);
				}
			}

		}
		else
		{
			printf("[Error] Packet format incorrect in %s:%d\n", __FILE__, __LINE__);
		}

		//Just rebind
		auto f = std::bind(&Network::verifyEndOfDownload, this, std::placeholders::_1, std::placeholders::_2);
		this->addTaskWaiter(sender.purpose, sender.address, sender.port, f);

		//Also, bind download task as the last time it might have been removed
		auto g = std::bind(&Network::getFileChunk, this, std::placeholders::_1, std::placeholders::_2);
		this->addTaskWaiter(Purpose::FileChunk, sender.address, sender.port, g);
		return 0;
	}

	int Network::addTaskWaiter(sf::Int32 purpose, sf::IpAddress addr, sf::Uint16 port, std::function<int(sf::Packet&, const TaskIdentifier&)> func)
	{
		TaskIdentifier TI(purpose, addr, port);
		//Add the function
		m_task_map[TI].push_back(func);
		return 0;
	}

	int Network::getFileChunk(sf::Packet& pkg, const TaskIdentifier& sender)
	{
		//pkg shall have format: <Purpose::FileChunk> << <ID of task> << <sf::Uint32 pos> << <sf::Uint16 chunkSize> << <Data>
		sf::Int32 p = 0;			//The purpose ID (just a check)
		sf::Int32 ID = 0;			//The download ID to assign the data received to the correct download task
		sf::Uint16 chunksize = 0;	//This should be enough, it's really unlikely we'll be able to fit 65KB in one single package anytime soon 
									//(Default is 1280 bytes (max 16384) on windows according to https://msdn.microsoft.com/de-de/library/cc737093(v=ws.10).aspx)
		sf::Uint32 position = 0;	//General chunk position

		//Since SFML does not support reading a specific count of bytes, we need to hack around a bit
		if (pkg >> p >> ID >> chunksize >> position)
		{
			//The purpose should be correct
			if (p != Purpose::FileChunk)
			{
				printf("[Error] Purpose not correct in %s:%d\n", __FILE__, __LINE__);
				return -1;
			}
			//Ok, now start the hack:
			size_t actual_pos = sizeof(p) + sizeof(ID) + sizeof(chunksize) + sizeof(position);	//Set the starting index
			assert(actual_pos - pkg.getDataSize() == chunksize);								//The size should agree (otherwise this is awkward)
			if (actual_pos - pkg.getDataSize() != chunksize)									//Fallback for release mode (should never be triggered, though)
			{
				//Print error, but ignore otherwise for now
				printf("[Warning] Expected chunk size does not agree with the received chunk size in %s:%d\n", __FILE__, __LINE__);
			}
			//We should now get access to our download task
			auto it = this->m_downloads.find(ID);
			if (it == this->m_downloads.end())
			{
				//The task wasn't found
				printf("[Error] File chunk for download with ID %d received, but there is no download with that ID pending in %s:%d\n", ID, __FILE__, __LINE__);
			}
			else
			{
				//Found, we can handle it
				SFG::Pointer<SFG::FileChunk> fc(new SFG::FileChunk(chunksize)); //Create the file chunk with the according size
				fc->copy(((char*)pkg.getData() + actual_pos), chunksize);		//Copy from packet to the File chunk

				//Get the download task
				SFG::Download& dwld = it->second;
				//Add the chunk to the task
				dwld.addChunk(position, fc);

				if (dwld.isComplete())
				{
					//We can return already; everything has been done
					return 0;
				}

			}
		}
		else
		{
			//The packet was damaged
			printf("[Error] Packet was damaged in %s:%d\n", __FILE__, __LINE__);
		}

		//Since the download isn't finished yet, we need to add the listener again
		auto f = std::bind(&Network::getFileChunk, this, std::placeholders::_1, std::placeholders::_2);
		this->addTaskWaiter(sf::Int32(Purpose::FileChunk), sender.address, sender.port, f);

		return 0;
	}

	int Network::startDownload(const sf::IpAddress& server, unsigned short remport, const sf::String& filename, bool TCP)
	{
		sf::Packet pkg;
		//#TODO: MB add ID?
		//Format: Purpose::Download << <filename> << <UDP-Port> << <TCP-Port> << <Use tcp?>
		pkg << sf::Int32(Purpose::Download) << filename << m_udp_port << m_tcp_port << TCP;
		//...

		//We always send to the UDP socket. This can be lost, so we'll have to retry if no answer is retrieved.

		//Lock the mutex
		m_udp_mutex.lock();
		//Add a task to the watchlist
		auto f = std::bind(&Network::getDownloadInfo, this, std::placeholders::_1, std::placeholders::_2);
		addTaskWaiter(sf::Int32(Purpose::DownloadInfo), server, remport, f);

		auto ret = m_udp_sock.send(pkg, server, remport);
		//Unlock the mutex already
		m_udp_mutex.unlock();
		switch (ret)
		{
		case sf::Socket::Done:
			//Everything worked!
			break;
		case sf::Socket::NotReady:
			printf("[Error] Socket not ready %s:%d\n", __FILE__, __LINE__);
			return -1;
			break;

		case sf::Socket::Error:
			printf("[Error] Some error... %s:%d\n", __FILE__, __LINE__);
			return -1;
			break;

		default:
			//Error...
			return -1;
			break;
		}

		//All ok, now we just have to wait for the answer
		return 0;
	}

	int Network::network_thread()
	{
		while (m_running)
		{
			m_udp_mutex.lock();

			//In this very case, we don't want to block
			m_udp_sock.setBlocking(false);

			sf::IpAddress remAddr;
			sf::Uint16 remPort;
			sf::Packet pkg;
			if (m_udp_sock.receive(pkg, remAddr, remPort) == sf::Socket::Done)
			{
				sf::Int32 p;
				pkg >> p;

				TaskIdentifier TI(p, remAddr, remPort);

				//Create copy of our map (#TODO: CHECK if not too expensive)
				auto task_map_cpy = m_task_map;

				//Find the functions bound to this task
				auto it = task_map_cpy.find(TI);
				if (it != task_map_cpy.end())
				{
					//Clear old taskmap (so new tasks can be added)
					m_task_map.find(TI)->second.clear();

					//if available, execute all functions bound to this task (usually, there should only be 1 single function)
					for (auto e : it->second)
					{
						e(pkg, TI);
					}
				}
			}
			//When we have treated this case, we should default it back to blocking
			m_udp_sock.setBlocking(true);
			m_udp_mutex.unlock();
		}
		return 0;
	}
};