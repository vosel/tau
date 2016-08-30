// This source file is part of the 'tau' open source project.
// Copyright (c) 2016, Yuriy Vosel.
// Licensed under Boost Software License.
// See LICENSE.txt for the licence information.

#ifdef TAU_HEADERONLY
#define LINKAGE_RESTRICTION inline
#else
#include "client_info.h"
#define LINKAGE_RESTRICTION 
#endif

#include <sstream>

namespace tau {
namespace communications_handling {

namespace {
	std::string getEndpointDump(std::string const & addr, unsigned short port)
	{
		std::stringstream result;
		result << addr << ':' << port;
		return result.str();
	}
}

LINKAGE_RESTRICTION std::string ClientConnectionInfo::getRemoteAddrDump() const
{
	return getEndpointDump(remoteAddr, remotePort);
}

LINKAGE_RESTRICTION std::string ClientConnectionInfo::getLocalAddrDump() const
{
	return getEndpointDump(localAddr, localPort);
}
} // namespace communications_handling
} // namespace tau