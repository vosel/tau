// This source file is part of the 'tau' open source project.
// Copyright (c) 2016, Yuriy Vosel.
// Licensed under Boost Software License.
// See LICENSE.txt for the licence information.

#pragma once
#include "../communications_handling/outgiong_packets_generator.h"
#include "../communications_handling/abstract_events_dispatcher.h"
#include "../communications_handling/incoming_data_stream_parser.h"

namespace tau {
namespace util {

class DotNetAsyncOutgiongPacketsGenerator :
	public tau::communications_handling::OutgiongPacketsGenerator
{
	void (*m_sendCallback)(); // Callback into .NET world to send the data in buffer.
	std::string m_current_pending_buf;
public:
	DotNetAsyncOutgiongPacketsGenerator(void (*callbackIntoDotNet)());
	virtual void sendData(std::string const & toSend);
	std::string getBufferToSend();
};

ref class DotNetAsyncServer abstract
{
	System::Net::Sockets::Socket ^ server_socket;
	System::Collections::ArrayList ^ clients;
	bool m_is_accepting;
	int m_port;
	int m_maxSimultaneousConnections;
public:
	DotNetAsyncServer(int port, int maxSimultaneousConnections);
	void start();
	void close_all();

	// Methods providing info about server status:
	bool isAcceptingNow();
	int listenPort();
	int maxAllowedConnections();
	int currentConnectionsCount();
protected:
	ref class DotNetAsyncSession abstract
	{
		System::Net::Sockets::Socket ^ client_socket;
		DotNetAsyncServer ^ m_parent;

		//This object does the parsing of the byte stream from the socket and calls the appropriate handlers depending on that data:
		tau::communications_handling::IncomingDataStreamParser * m_incomingDataParser;

		//These fields are used for preparing and sending data to the socket:
		delegate void MyNativeCallbacksDelegate();
		MyNativeCallbacksDelegate ^ m_sendDataDelegate;
		array<unsigned char> ^ m_buffer;
	protected:
		// The pointer is used here because c++/clr classes don't allow normal c++ fields.
		// So, the clean-up of this object's memory has to be done manually.
		DotNetAsyncOutgiongPacketsGenerator * m_packetsGeneratorPtr;

		virtual void cleanup_native_stuff();
		virtual tau::communications_handling::AbstractEventsDispatcher * getEventsDispatcher() = 0;
	public:
		DotNetAsyncSession(DotNetAsyncServer ^ parent, System::Net::Sockets::Socket ^ connectionSocket);
		void receiveCallback(System::IAsyncResult ^ result);
		void beginReceive();
		void close();
	private:
		void send_data_pending_in_native_code();
	};
private:
	void acceptCallback(System::IAsyncResult ^ result);
	void begin_accept_if_allowed_to();
protected:
	void remove_closed_handler(DotNetAsyncSession ^ handler);
	
	// Override this method to add your logic to the server (the logic inside the session class):
	virtual DotNetAsyncSession ^ create_connection_handler(System::Net::Sockets::Socket ^ connectionSocket) = 0;
};

}//	namespace util
}// namespace tau

#ifdef TAU_HEADERONLY
#include "dot_net_async_server.cpp"
#endif
