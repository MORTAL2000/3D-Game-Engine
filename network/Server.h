/*
 * Copyright 2015 Alexander Koch
 * File: Server.h
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SERVER_H
#define SERVER_H

#include <core/Commons.h>
#include <core/Console.h>

#ifdef __WINDOWS_API__
	#include <winsock2.h>
	typedef int socklen_t;

#elif defined(__APPLE_API__) || defined(__LINUX_API__)

	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <fcntl.h>

#else
	#error unknown platform!

#endif

namespace Server
{
	bool initializeSockets();
	void shutdownSockets();
}

namespace UDP
{
	bool createSocket(short);
	void closeSocket();

	// port > 1024, a+b+d+c+d = 192.168.0.106
	bool send(const std::string&, short, unsigned int, unsigned int, unsigned int, unsigned int);
	bool receive(std::string&);
}

#endif
