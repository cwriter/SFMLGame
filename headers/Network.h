#pragma once
#include "stdafx.h"
#include "System.h"

//Defines a 4x 255 version number. y values can be ignored, different x values can result in the
//Network refusing to establish connection
#define MAKE_VERSION(x1,x2,y1,y2) ((y2 << 0) | (y1 << 8) | (x2 << 16) | (x1 << 24))

#define NETWORK_VERSION MAKE_VERSION(0,1,0,0)

//Defines the default UDP port that every instance shall support
#define DEFAULT_UDP_PORT 54222

namespace SFG
{
class CompleteAddress
{
public:
    CompleteAddress()
    {

    }
    CompleteAddress(const sf::IpAddress& addr, const sf::Uint16 port)
        : address(addr), port(port)
    {

    }

    sf::IpAddress address;
    sf::Uint16 port;
};

class TaskIdentifier : public CompleteAddress
{
public:
    TaskIdentifier()
    {

    }
    TaskIdentifier(const sf::Int32 p, const sf::IpAddress& addr, const sf::Uint16 port)
        : CompleteAddress(addr, port), purpose(p)
    {

    }

    bool operator<(const TaskIdentifier& other) const;
    sf::Int32 purpose;
};

///<summary>
///Class same as sf::Packet, but additionally inherits from CompleteAddress to contain address and remoteport of the sender.
///</summary>
class Package
    : public sf::Packet, public SFG::CompleteAddress
{
public:
};

///<summary>
///Wrapper for any download
///</summary>
class Download
{
public:
    enum State
    {
        InProgress = 0,
        notStarted,
        Pending,
        Aborted
    };

    Download();

    ///<summary>
    ///Checks whether the download is completed.
    ///</summary>
    ///<returns>
    ///True if the download is complete, false otherwise
    ///</returns>
    bool isComplete();

    ///<summary
    ///Returns a list of all incomplete chunks
    ///</summary>
    ///<returns>
    ///A list of elements containing the indices of the missing chunks
    ///</returns>
    std::list<int> getIncompleteChunks();

    void addChunk(size_t pos, SFG::Pointer<SFG::FileChunk>& fc)
    {
        chunks[pos] = fc;
    }

    ///<summary>
    ///Lets the system reserve space for the file
    ///</summary>
    void reserveFile()
    {
        ///#TODO
    }

    ///<summary>
    ///Flushes chunks to file if possible
    ///</summary>
    void flush()
    {
        ///#TODO
    }

    sf::Uint64 filesize; //The size of the file this instance is downloading
    sf::Uint64 current_index; //The current pointer
    sf::String remotename; //The name of the file as the server suggests it
    sf::String localname; //The name of the file to write to

    short state; //The state of the download. See Download::State for more information

    std::map<size_t, SFG::Pointer<SFG::FileChunk>> chunks;	//Chunks (only chunks that are done are stored in here
    //If already written to disk, Chunks will be the nullptr)

    System* filesystem;

private:
};

///<summary>
///Manages all network related operations. This will block, however, <br/>
///so using SFG::System will still be necessary for big file downloads etc.
///</summary>
class Network
    : public SFG::SystemClient
{
public:
    Network()
    {
        this->m_filesystem = nullptr;
    }
    ~Network()
    {

    }

    enum Purpose
    {
        CheckNetworkVersion = 0, 	//Send to check the network protocol version. All implementations MUST support this option
        CheckApplication,			//Send to check the application (obviously, different applications might have different purpose codes)

        DisplayMessage,				//Send to make the other side display a message. There is no guarantee the message actually gets displayed, though.

        Download,					//Send to initialize Download process
        DownloadInfo,				//Send to provide information about how the previous "Download" request will be treated.
        StartDownload,				//Send to tell the server to start sending the data
        AbortDownload,				//Send to tell the server to discard the scheduled task
        FileChunk,					//Send to identify a part of a file download
        SpecificFileChunkRequest,	//Send to request the server to send a specific file chunk (use if a file download was incomplete)
        EndOfDownload,				//Send to tell the client that the server has sent all file chunks
        DownloadComplete			//Send to tell the server that the client has received all chunks and the server can discard the file from its memory
    };

    ///<summary>
    ///Sets the application ID
    ///</summary>
    inline void setInformation(const sf::String& appname)
    {
        this->m_appname = appname;
    }

    ///<summary>
    ///Binds all sockets to their ports
    ///</summary>
    ///<param name="port_tcp">The UDP port number</param>
    ///<param name="port_udp">The UDP port number</param>
    int init(int port_tcp, int port_udp);

    ///<summary>
    ///Compares Network (Protocol) Versions.
    ///</summary>
    ///<returns>0 if versions agree, 1 if minor versions disagree, -1 if major versions disagree</returns>
    int checkNetworkVersion(sf::Int32 Version);

    ///<summary>
    ///The network thread. This will run all the time, but can be paused using mutexes.
    ///</summary>
    int network_thread();

    ///<summary>
    ///Adds a specific task to wait for.
    ///</summary>
    int addTaskWaiter(sf::Int32 purpose, sf::IpAddress addr, sf::Uint16 port, std::function<int(sf::Packet&, const TaskIdentifier&)> func);

    ///<summary>
    ///Asks the other side to send the contents of file "filename".
    ///</summary>
    ///<param name="filename">The name of the file the other side should send.</param>
    ///<param name="TCP">Whether the other side should send via TCP (if set to false, send via UDP).
    ///The other side is not required to grant this wish</param>
    int startDownload(const sf::IpAddress& server, unsigned short remport, const sf::String& filename, bool TCP = true);

    ///<summary>
    ///Simple ID generator. Do not use unaltered in productive code.
    ///</summary>
    int CreateID()
    {
        printf("[Warning] This implementation of int Network::CreateID() should not be used in final code: %s:%d\n", __FILE__, __LINE__);
        return int(this->m_downloads.size());
    }

    ///<summary>
    ///Gets the information needed to start the previously requested download
    ///</summary>
    int getDownloadInfo(sf::Packet& pkg, const TaskIdentifier& sender);

    ///<summary>
    ///Gets a chunk of the file that should be downloaded
    ///</summary>
    int getFileChunk(sf::Packet& pkg, const TaskIdentifier& sender);

    ///<summary>
    ///Client side: This function checks whether a download is complete (as the Packet claims). If it isn't, the missing parts will be scheduled again.
    ///</summary>
    int verifyEndOfDownload(sf::Packet& pkg, const TaskIdentifier& sender);

    ///<summary>
    ///Downloads a file to memory.
    ///</summary>
    ///<param name="out">Memory location where the data should be stored.</param>
    ///<param name="cursor">Position in the file where the reading starts.</param>
    ///<param name="chunksize">The amount of data sent per-package. Default is 4096</param>
    int download(void* out, size_t cursor, size_t chunksize = 256)
    {

        return 0;
    }

private:
    sf::String m_appname;
    sf::SocketSelector m_selector; //#TODO: This seems scary to me. Please check.

    bool m_running;

    std::map<TaskIdentifier,std::list<std::function<int(sf::Packet&, const TaskIdentifier&)>>> m_task_map;
    std::map<int, SFG::Download> m_downloads;

    //UDP stuff
    sf::UdpSocket m_udp_sock;
    int m_udp_port;

    std::mutex m_udp_mutex;

    //TCP stuff
    sf::TcpSocket m_tcp_sock;
    sf::TcpListener m_tcp_listener;
    int m_tcp_port;

    SFG::System* m_filesystem;

};

};
