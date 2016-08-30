// This source file is part of the 'tau' open source project.
// Copyright (c) 2016, Yuriy Vosel.
// Licensed under Boost Software License.
// See LICENSE.txt for the licence information.

#ifndef TAU_HEADERONLY
#include "incoming_data_stream_parser.h"
#endif

#include "incoming_packets_handler.h"
#include "communications_issues_handler.h"
#include "packet_types.h"
#include <sstream>

#ifdef TAU_HEADERONLY
#define LINKAGE_RESTRICTION inline
#else
#define LINKAGE_RESTRICTION 
#endif

namespace tau {
namespace communications_handling {
LINKAGE_RESTRICTION void IncomingDataStreamParser::newData(std::string const & data, tau::communications_handling::IncomingPacketsHandler * packetsHandler, CommunicationIssuesHandler * issuesHandler)
{
	for (size_t i = 0; i < data.size(); ++i) {
		char current = data[i];
		if (current == '\0') {
			processPacketInBuffer(packetsHandler, issuesHandler);
		} else {
			m_buffer.push_back(current);
		}
	}
}

namespace {
	// Small utility function needed in the IncomingDataStreamParser::processPacketInBuffer()
	std::pair<std::string, std::string> util_clipBeginningOff(std::string const & toProcess, char delimiter) {
		size_t index = toProcess.find(delimiter);
		if (index == std::string::npos) {
			return std::pair<std::string, std::string>(toProcess, "");
		}
		return std::pair<std::string, std::string>(toProcess.substr(0, index), toProcess.substr(index + 1, std::string::npos));
	}
}

LINKAGE_RESTRICTION void IncomingDataStreamParser::processPacketInBuffer(IncomingPacketsHandler * packetsHandler, CommunicationIssuesHandler * issuesHandler)
{
	std::pair<std::string, std::string> packetTypeSeparated = util_clipBeginningOff(m_buffer, ';');
	std::pair<std::string, std::string> layoutIDSeparated = util_clipBeginningOff(packetTypeSeparated.second, ';');
	std::string packetType(packetTypeSeparated.first);
	std::string layoutElementID(layoutIDSeparated.first);
	std::string additionalData(layoutIDSeparated.second);
	packetsHandler->onPacketReceived(packetType, layoutElementID, additionalData);
	m_buffer.clear();
}
}
}

#undef LINKAGE_RESTRICTION 