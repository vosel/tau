// This source file is part of the 'tau' open source project.
// Copyright (c) 2016, Yuriy Vosel.
// Licensed under Boost Software License.
// See LICENSE.txt for the licence information.

#pragma once

#include <string>

namespace tau {
namespace communications_handling {

class IncomingPacketsHandler;
class CommunicationIssuesHandler;
class IncomingDataStreamParser
{
	enum {bufSize = 1024};
	std::string m_buffer;
	size_t usedBufferSpace;
public:
	IncomingDataStreamParser() : usedBufferSpace(0) {
		m_buffer.reserve(bufSize);
	};
	void newData(std::string const & data, IncomingPacketsHandler * packetsHandler, CommunicationIssuesHandler * issuesHandler);
private:
	void processPacketInBuffer(IncomingPacketsHandler * packetsHandler, CommunicationIssuesHandler * issuesHandler);
};
}
}

#ifdef TAU_HEADERONLY
#include "incoming_data_stream_parser.cpp"
#endif