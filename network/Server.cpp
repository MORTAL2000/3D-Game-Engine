#include "Server.h"

namespace Server
{
	bool initializeSockets()
	{
#ifdef __WINDOWS_API__
		WSADATA WsaData;
		Console::log("Initializing sockets...");
		return WSAStartup(MAKEWORD(2,2), &WsaData) == NO_ERROR;
#else
		return true;
#endif
	}

	void shutdownSockets()
	{

#ifdef __WINDOWS_API__
		WSACleanup();
		Console::log("Closed sockets");
#endif
	}
}

namespace UDP
{

	int socket = 0;

	void closeSocket()
	{
	#ifdef __WINDOWS_API__
		closesocket(socket);
	#else
		close(socket);
	#endif
	}

	bool createSocket(short port)
	{
		socket = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if(socket <= 0)
		{
			Console::log("Failed to create socket");
			socket = 0;
			return false;
		}

		sockaddr_in address = {0};
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = htonl(INADDR_ANY);
		address.sin_port = htons((unsigned short)port);

		if(bind(socket, (const sockaddr*)&address, sizeof(sockaddr_in)) < 0)
		{
			Console::log("Failed to bind socket");
			closeSocket();
			return false;
		}

	// set non-blocking mode
	#if defined(__APPLE_API) || defined(__LINUX_API__)
		int nonBlocking = 1;
		if(fcntl(socket, F_SETFL, O_NONBLOCK, nonBlocking) == -1)
		{
			Console::log("Failed to set non-blocking mode");
			closeSocket();
			return false;
		}
	#else

		DWORD nonBlocking = 1;
		if(ioctlsocket(socket, FIONBIO, &nonBlocking) != 0)
		{
			Console::log("Failed to set non-blocking mode");
			closeSocket();
			return false;
		}
	#endif
		return true;
	}

	// port > 1024, a+b+d+c+d = 192.168.0.106
	bool send(const std::string& data, short port, unsigned int a, unsigned int b, unsigned int c, unsigned int d)
	{
		char packet_data[256];
		std::size_t length = data.copy(packet_data, data.length(), 0);
		packet_data[length] = '\0';

		unsigned int address = (a << 24) | (b << 16) | (c << 8) | d;
		sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = htonl(address);
		addr.sin_port = htons((unsigned short)port);
	 	unsigned packet_size = 256;

		int sent_bytes = sendto(socket, (const char*)packet_data, packet_size, 0, (sockaddr*)&address, sizeof(sockaddr_in));
		return (sent_bytes == packet_size);
	}

	bool receive(std::string& data)
	{
		unsigned char packet_data[256];
		unsigned int max_packet_size = sizeof(packet_data);

		sockaddr_in sender;
		socklen_t length = sizeof(sockaddr_in);

		int bytes = recvfrom(socket, (char*)packet_data, max_packet_size, 0, (sockaddr*)&sender, &length);
		if(bytes <= 0) return false;

		unsigned int address = ntohl(sender.sin_addr.s_addr);
		unsigned int port = ntohs(sender.sin_port);

		data.clear();
		data = std::string(reinterpret_cast<char*>(packet_data), 256);

		std::cout << packet_data << std::endl;

		return true;
	}

}
