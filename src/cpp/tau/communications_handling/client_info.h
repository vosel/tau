// This source file is part of the 'tau' open source project.
// Copyright (c) 2016, Yuriy Vosel.
// Licensed under Boost Software License.
// See LICENSE.txt for the licence information.

#pragma once

#include <string>

namespace tau {
namespace communications_handling {

struct ClientDeviceInfo 
{
	size_t const width;
	size_t const height;
	size_t const version;
	std::string const deviceID;
	ClientDeviceInfo(size_t screenWidth, size_t screenHeight, std::string const & clientDeviceID, size_t clientVersion):
		width(screenWidth), height(screenHeight), version(clientVersion), deviceID(clientDeviceID) {};
};

struct ClientConnectionInfo
{
	std::string const localAddr;
	unsigned short const localPort;
	std::string const remoteAddr;
	unsigned short const remotePort;
	ClientConnectionInfo(std::string const & connection_remoteAddr, unsigned short connection_remotePort, std::string const & connection_localAddr, unsigned short connection_localPort):
		remoteAddr(connection_remoteAddr), remotePort(connection_remotePort), localAddr(connection_localAddr), localPort(connection_localPort)
	{};
	std::string getRemoteAddrDump() const;
	std::string getLocalAddrDump() const;
};

}
}

#ifdef TAU_HEADERONLY
#include "client_info.cpp"
#endif