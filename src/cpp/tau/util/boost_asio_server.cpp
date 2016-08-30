// This source file is part of the 'tau' open source project.
// Copyright (c) 2016, Yuriy Vosel.
// Licensed under Boost Software License.
// See LICENSE.txt for the licence information.

#ifndef TAU_HEADERONLY
#include "boost_asio_server.h"
#endif

#ifdef TAU_HEADERONLY
#define LINKAGE_RESTRICTION inline
#else
#define LINKAGE_RESTRICTION 
#endif

namespace tau {
	namespace util {


LINKAGE_RESTRICTION void BoostAsioOutgiongPacketsGenerator::setParentSession(AbstractBoostAsioSession * session) {
	m_session = session;
}
LINKAGE_RESTRICTION void BoostAsioOutgiongPacketsGenerator::sendData(std::string const & toSend) {
	std::stringstream tempStream(toSend);
	std::string tmpString;
	size_t counter = 0;
	while(std::getline(tempStream, tmpString)) {
		++counter;
	}
	m_session->sendStringToSocket(toSend);
};

namespace {
	std::string getAddr(boost::asio::ip::tcp::endpoint const & ep)
	{
		return ep.address().to_string();
	}
	
	unsigned short getPort(boost::asio::ip::tcp::endpoint const & ep)
	{
		return ep.port();
	}
}

LINKAGE_RESTRICTION AbstractBoostAsioSession::AbstractBoostAsioSession(boost::asio::io_service& io_service): m_socket(io_service) {	
	m_outgoingPacketsGenerator.setParentSession(this);
}

LINKAGE_RESTRICTION boost::asio::ip::tcp::socket& AbstractBoostAsioSession::socket() {
	return m_socket;
}

LINKAGE_RESTRICTION void AbstractBoostAsioSession::sendStringToSocket(std::string const & str) { // Helper function - so I don't have to call 'boost::asio::write()' everywhere.
	boost::system::error_code errorCode;
	//TODO: use async write here:
	boost::asio::write(m_socket, boost::asio::buffer(str.c_str(), str.size()), errorCode);
	if (errorCode) {
		getEventsDispatcher()->getCommunicationIssuesHandler()->onCommunicationError("Error writing data to the socket. Closing the connection.");
		getEventsDispatcher()->onConnectionClosed();
		delete this; //TODO: need to get rid of these delete operations. It is becomming quite error-prone because we have both async reads and writes, so if delete is done in handle_write, what will happen when we call handle_read after that??? 
	}
}

LINKAGE_RESTRICTION void AbstractBoostAsioSession::start() {
	tau::communications_handling::ClientConnectionInfo connectionInfo(
			getAddr(m_socket.remote_endpoint()), getPort(m_socket.remote_endpoint()),
			getAddr(m_socket.local_endpoint()), getPort(m_socket.local_endpoint()));

	getEventsDispatcher()->onClientConnected(connectionInfo);

	m_socket.async_read_some(
		boost::asio::buffer(data_, max_length),
		boost::bind(
			&AbstractBoostAsioSession::handle_read,
			this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred)
	);
}

LINKAGE_RESTRICTION void AbstractBoostAsioSession::handle_read(const boost::system::error_code& error, size_t bytes_transferred) {
	if (!error) {
		std::string tranferred_data(data_, bytes_transferred);
		
		m_dataStreamParser.newData(
			tranferred_data, 
			getEventsDispatcher()->getIncomingPacketsHandler(), 
			getEventsDispatcher()->getCommunicationIssuesHandler()
		);
		
		// Read more data:
		m_socket.async_read_some(
			boost::asio::buffer(data_, max_length),
			boost::bind(
				&AbstractBoostAsioSession::handle_read,
				this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred
			)
		);
	} else {
		getEventsDispatcher()->getCommunicationIssuesHandler()->onCommunicationError("Error reading data from the socket. Closing the connection");
		getEventsDispatcher()->onConnectionClosed();
		delete this;
	}
}

LINKAGE_RESTRICTION void AbstractBoostAsioSession::handle_write(const boost::system::error_code& error)
{
	if (!error) {
		m_socket.async_read_some(
			boost::asio::buffer(data_, max_length),
			boost::bind(
				&AbstractBoostAsioSession::handle_read,
				this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred
			)
		);
	} else {
		delete this;
	}
}

LINKAGE_RESTRICTION AbstractBoostAsioServer::AbstractBoostAsioServer(boost::asio::io_service& io_service, short port):
	  m_io_service(io_service), m_acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
{}

LINKAGE_RESTRICTION void AbstractBoostAsioServer::start()
{
	//TODO: rewrite some implementation details: need to add proper memory handling.
	start_accept();
}

LINKAGE_RESTRICTION void AbstractBoostAsioServer::start_accept() {
	AbstractBoostAsioSession* new_session = createSession();
	m_acceptor.async_accept(
		new_session->socket(),
		boost::bind(
			&AbstractBoostAsioServer::handle_accept, this, new_session,
			boost::asio::placeholders::error
		)
	);
}

LINKAGE_RESTRICTION void AbstractBoostAsioServer::handle_accept(AbstractBoostAsioSession* new_session, const boost::system::error_code& error)
{
	if (!error) {
		new_session->start();
	} else {
		delete new_session;//TODO: rewrite this aspect of the AbstractBoostAsioServer class - the memory management should be done with smart pointers
	}
	start_accept();
}

} // namespace util
} // namespace tau

#undef LINKAGE_RESTRICTION 
