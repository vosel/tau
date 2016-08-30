// This source file is part of the 'tau' open source project.
// Copyright (c) 2016, Yuriy Vosel.
// Licensed under Boost Software License.
// See LICENSE.txt for the licence information.

#pragma once
#include "../common/element_id.h"
#include "client_info.h"
namespace tau {
namespace communications_handling {

class IncomingPacketsHandler;
class CommunicationIssuesHandler;
class OutgiongPacketsGenerator;

class AbstractEventsDispatcher
{
public:
	virtual IncomingPacketsHandler * getIncomingPacketsHandler() = 0;
	virtual CommunicationIssuesHandler * getCommunicationIssuesHandler() = 0;

	// Callbacks which are executed when the connection with client is established and terminated.
	virtual void onClientConnected(ClientConnectionInfo const & info) {};
	virtual void onConnectionClosed() {};
	
#if defined(TAU_CPP_03_COMPATIBILITY)
	virtual ~AbstractEventsDispatcher() {};
#else
	virtual ~AbstractEventsDispatcher() = default; 
#endif
};
}
}