// This source file is part of the 'tau' open source project.
// Copyright (c) 2016, Yuriy Vosel.
// Licensed under Boost Software License.
// See LICENSE.txt for the licence information.

#pragma once
#include <string>


namespace tau {
namespace communications_handling {

class CommunicationIssuesHandler
{
public:
	virtual void onCommunicationError(std::string const & errorDescription) {};

#if defined(TAU_CPP_03_COMPATIBILITY)
	virtual ~CommunicationIssuesHandler() {};
#else
	virtual ~CommunicationIssuesHandler() = default; 
#endif

};
}
}