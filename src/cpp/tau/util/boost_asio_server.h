// This source file is part of the 'tau' open source project.
// Copyright (c) 2016, Yuriy Vosel.
// Licensed under Boost Software License.
// See LICENSE.txt for the licence information.

#pragma once

#include "../communications_handling/abstract_events_dispatcher.h"
#include "../communications_handling/incoming_packets_handler.h"
#include "../communications_handling/outgiong_packets_generator.h"
#include "../communications_handling/communications_issues_handler.h"
#include "../communications_handling/incoming_data_stream_parser.h"
#include "../communications_handling/packet_types.h"


#include <cstdlib>
#include <iostream>
#include <sstream>

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>


#ifdef TAU_CPP_03_COMPATIBILITY
#define OVERRIDE_IDENTIFIER 
#else
#define OVERRIDE_IDENTIFIER override
#endif

namespace tau {
namespace util {


class AbstractBoostAsioSession;

class BoostAsioOutgiongPacketsGenerator :
	public tau::communications_handling::OutgiongPacketsGenerator
{
	AbstractBoostAsioSession * m_session;
public:
	void setParentSession(AbstractBoostAsioSession * session);
	virtual void sendData(std::string const & toSend) OVERRIDE_IDENTIFIER;
};

class AbstractBoostAsioSession
{	
	boost::asio::ip::tcp::socket m_socket;
	enum { max_length = 1024 };
	char data_[max_length];
	communications_handling::IncomingDataStreamParser m_dataStreamParser;
protected:
	BoostAsioOutgiongPacketsGenerator m_outgoingPacketsGenerator;
public:
	AbstractBoostAsioSession(boost::asio::io_service& io_service);
	boost::asio::ip::tcp::socket& socket();

public:
	void sendStringToSocket(std::string const & str);
	void start();
protected:
	virtual tau::communications_handling::AbstractEventsDispatcher * getEventsDispatcher() = 0;
private:
	void handle_read(const boost::system::error_code& error, size_t bytes_transferred);
	void handle_write(const boost::system::error_code& error);
};

class AbstractBoostAsioServer
{
	boost::asio::ip::tcp::acceptor m_acceptor;
protected:
	boost::asio::io_service & m_io_service;
public:
	AbstractBoostAsioServer(boost::asio::io_service & io_service, short port);
	void start();
protected:

// TODO: change the approach to the returned value of the call (it is currently very prone to leaks, which is unacceptable)
// One is the options is use boost::movelib::unique_ptr<Session> as a result of this call. This way we convey that the ownership of this object is transferred to us, so, when the connection is closed, we could delete it on the spot.
	virtual AbstractBoostAsioSession* createSession() = 0;
private:
	void start_accept();
	void handle_accept(AbstractBoostAsioSession* new_session, const boost::system::error_code& error);
};

template <typename EventsDispatcherObjectType>
class BoostAsioSession: public AbstractBoostAsioSession
{
	EventsDispatcherObjectType	m_eventsDispatcher;
public:
	BoostAsioSession(boost::asio::io_service & io_service) : AbstractBoostAsioSession(io_service), m_eventsDispatcher(m_outgoingPacketsGenerator) {};
protected:
	virtual tau::communications_handling::AbstractEventsDispatcher * getEventsDispatcher() OVERRIDE_IDENTIFIER 
	{
		return &m_eventsDispatcher;
	};
};

template <typename Session>
class BoostAsioServer:public AbstractBoostAsioServer
{
public:
	BoostAsioServer(boost::asio::io_service& io_service, short port): AbstractBoostAsioServer(io_service, port) {};
protected:	
	virtual AbstractBoostAsioSession* createSession() OVERRIDE_IDENTIFIER 
	{
		return new Session(m_io_service); //TODO: wrap this in some kind of unique_ptr
	};
};

template <typename EventsDispatcherObjectType>
struct SimpleBoostAsioServer
{
	typedef BoostAsioServer<BoostAsioSession<EventsDispatcherObjectType> > type;
};

} // namespace util 
} // namespace tau 

#undef OVERRIDE_IDENTIFIER 

#ifdef TAU_HEADERONLY
#include "boost_asio_server.cpp"
#endif
