// This source file is part of the 'tau' open source project.
// Copyright (c) 2016, Yuriy Vosel.
// Licensed under Boost Software License.
// See LICENSE.txt for the licence information.

#ifndef TAU_HEADERONLY
#include "dot_net_async_server.h"
#endif

#ifdef TAU_HEADERONLY
#define LINKAGE_RESTRICTION inline
#else
#define LINKAGE_RESTRICTION 
#endif

namespace tau {
namespace util {

LINKAGE_RESTRICTION DotNetAsyncServer::DotNetAsyncServer(int port, int maxSimultaneousConnections):m_is_accepting(false), m_port(port), m_maxSimultaneousConnections(maxSimultaneousConnections)
{}

LINKAGE_RESTRICTION void DotNetAsyncServer::start()
{
	clients = gcnew System::Collections::ArrayList();
	server_socket = gcnew System::Net::Sockets::Socket(
		System::Net::Sockets::AddressFamily::InterNetwork,
		System::Net::Sockets::SocketType::Stream,
		System::Net::Sockets::ProtocolType::Tcp);

	server_socket->Bind(gcnew System::Net::IPEndPoint(System::Net::IPAddress::Any, m_port));
	server_socket->Listen(1);
	begin_accept_if_allowed_to();
}

LINKAGE_RESTRICTION int DotNetAsyncServer::listenPort()
{
	return m_port;
}

LINKAGE_RESTRICTION bool DotNetAsyncServer::isAcceptingNow()
{
	return m_is_accepting;
}

LINKAGE_RESTRICTION int DotNetAsyncServer::maxAllowedConnections()
{
	return m_maxSimultaneousConnections;
}

LINKAGE_RESTRICTION int DotNetAsyncServer::currentConnectionsCount()
{
	return clients->Count;
}	

LINKAGE_RESTRICTION void DotNetAsyncServer::close_all()
{
	for (int i = 0; i < clients->Count; ++i) {
		((DotNetAsyncSession ^)clients[i])->close();
	}
	clients->Clear();
	server_socket->Shutdown(System::Net::Sockets::SocketShutdown::Both);
	server_socket->Close();

}

LINKAGE_RESTRICTION void DotNetAsyncServer::begin_accept_if_allowed_to()
{
	if (m_is_accepting) { // is already accepting
		return;
	}
	if (m_maxSimultaneousConnections >= 0) {
		if (clients->Count >= m_maxSimultaneousConnections) {
			return;
		}
	}
	server_socket->BeginAccept(gcnew System::AsyncCallback(this, &DotNetAsyncServer::acceptCallback), nullptr);	
	m_is_accepting = true;
}

LINKAGE_RESTRICTION void DotNetAsyncServer::acceptCallback(System::IAsyncResult ^ result)
{
	m_is_accepting = false;
	System::Net::Sockets::Socket ^ connectionSocket = server_socket->EndAccept(result);
	DotNetAsyncSession ^ nextHandler = create_connection_handler(connectionSocket);
	clients->Add(nextHandler);
	nextHandler->onClientConnected();
	begin_accept_if_allowed_to(); 
}

LINKAGE_RESTRICTION void DotNetAsyncServer::remove_closed_handler(DotNetAsyncSession ^ handler)
{
	clients->Remove(handler);
	begin_accept_if_allowed_to();
}

LINKAGE_RESTRICTION DotNetAsyncOutgiongPacketsGenerator::DotNetAsyncOutgiongPacketsGenerator(
	void (*sendDataCallbackIntoDotNet)(), void (*closeConnectionCallbackIntoDotNet)())
	: m_sendCallback(sendDataCallbackIntoDotNet), m_closeConnectionCallback(closeConnectionCallbackIntoDotNet)
{}

LINKAGE_RESTRICTION void DotNetAsyncOutgiongPacketsGenerator::sendData(std::string const & toSend)
{
	m_current_pending_buf = toSend; 
	m_sendCallback();
	m_current_pending_buf = "";
}

LINKAGE_RESTRICTION void DotNetAsyncOutgiongPacketsGenerator::close_connection()
{
	m_closeConnectionCallback();
}

LINKAGE_RESTRICTION std::string DotNetAsyncOutgiongPacketsGenerator::getBufferToSend()
{
	return m_current_pending_buf;
}

namespace {

	std::string sysStr2stdStr(System::String ^ toConvert)
	{
		int len = toConvert->Length;
		const char * chars = (const char * )(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(toConvert).ToPointer());
		std::string resultString(chars, len);
		return resultString;
	}
}
LINKAGE_RESTRICTION DotNetAsyncServer::DotNetAsyncSession::DotNetAsyncSession(DotNetAsyncServer ^ parent, System::Net::Sockets::Socket ^ connectionSocket)
	: m_parent(parent), m_buffer(gcnew array<unsigned char>(1024)), client_socket(connectionSocket)
{
	//creating the delegate and extracting function ptr for the callback (it would be called from the native code side to inform .net code that there is some data that need to be sent to the socket)
	m_sendDataDelegate = gcnew MyNativeCallbacksDelegate(this, &DotNetAsyncServer::DotNetAsyncSession::send_data_pending_in_native_code);
	System::IntPtr sendDataCallbackForNativeCode = System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(m_sendDataDelegate);
	m_closeConnectionDelegate = gcnew MyNativeCallbacksDelegate(this, &DotNetAsyncServer::DotNetAsyncSession::close);
	System::IntPtr closeConnectionCallbackForNativeCode = System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(m_closeConnectionDelegate);
	m_packetsGeneratorPtr = new DotNetAsyncOutgiongPacketsGenerator(
		(void (*)())(sendDataCallbackForNativeCode.ToPointer()),
		(void (*)())(closeConnectionCallbackForNativeCode.ToPointer()));

	m_incomingDataParser =
		new tau::communications_handling::IncomingDataStreamParser();
}

LINKAGE_RESTRICTION void DotNetAsyncServer::DotNetAsyncSession::onClientConnected()
{
	System::Net::IPEndPoint ^ localEndpoint = (System::Net::IPEndPoint ^)client_socket->LocalEndPoint;
	System::Net::IPEndPoint ^ remoteEndpoint = (System::Net::IPEndPoint ^)client_socket->RemoteEndPoint;

	tau::communications_handling::ClientConnectionInfo connectionInfo(
			sysStr2stdStr(remoteEndpoint->Address->ToString()), remoteEndpoint->Port,
			sysStr2stdStr(localEndpoint->Address->ToString()), localEndpoint->Port);

	getEventsDispatcher()->onClientConnected(connectionInfo);
	beginReceive();
}

LINKAGE_RESTRICTION void DotNetAsyncServer::DotNetAsyncSession::send_data_pending_in_native_code()
{
	std::string toSend = m_packetsGeneratorPtr->getBufferToSend();
	cli::array<unsigned char> ^ sendbuf = gcnew cli::array<unsigned char>(toSend.size());
	for (size_t i = 0; i < toSend.size(); ++i) {
		sendbuf[i] = toSend[i];
	}
	client_socket->Send(sendbuf, sendbuf->Length, System::Net::Sockets::SocketFlags::None);
}

LINKAGE_RESTRICTION void DotNetAsyncServer::DotNetAsyncSession::beginReceive()
{
	client_socket->BeginReceive(m_buffer, 0, m_buffer->Length, System::Net::Sockets::SocketFlags::None,  gcnew System::AsyncCallback(this, &DotNetAsyncSession::receiveCallback), nullptr);
}

LINKAGE_RESTRICTION void DotNetAsyncServer::DotNetAsyncSession::close()
{
	client_socket->Shutdown(System::Net::Sockets::SocketShutdown::Both);
	client_socket->Close();

	delete m_incomingDataParser; // This 'delete' has to be here because m_incomingDataParser is not visible from outside.
	// So, if the cleanup_native_stuff() gets overridden (which is often the case), it will not be able to clean m_incomingDataParser. 
	// Deleting all the rest is the responsibility of the overriding function. It should clean up m_packetsGeneratorPtr.
	cleanup_native_stuff();
}

// All the objects that are not managed by garbage collector, should be deleted manually in this function (it may be overriden, if there is some more complex resource management is done in subclass of this)
LINKAGE_RESTRICTION void DotNetAsyncServer::DotNetAsyncSession::cleanup_native_stuff()
{
	delete m_packetsGeneratorPtr;
}

LINKAGE_RESTRICTION void DotNetAsyncServer::DotNetAsyncSession::receiveCallback(System::IAsyncResult ^ result)
{
	int recvsize = client_socket->EndReceive(result);
	if (0 < recvsize) {
 		tau::communications_handling::IncomingPacketsHandler * incomingPacketsHandler = 
			getEventsDispatcher()->getIncomingPacketsHandler();
		tau::communications_handling::CommunicationIssuesHandler * communicationIssuesHandler = 
			getEventsDispatcher()->getCommunicationIssuesHandler();
		if ((nullptr != incomingPacketsHandler) && (nullptr != communicationIssuesHandler)) {
			std::string data;
			for (int i = 0; i < recvsize; ++i) {
				unsigned char current = m_buffer[i];
				data += current;
			}
			m_incomingDataParser->newData(
				data, incomingPacketsHandler, communicationIssuesHandler);
		}
		beginReceive();// Receive next batch of data.
	} else {
		getEventsDispatcher()->onConnectionClosed(); // Notify the user code that all the data transfers are not allowed from now on.
		close(); // Clean up the allocated resources.
		m_parent->remove_closed_handler(this); 
	}
}

}//	namespace util
}// namespace tau

#undef LINKAGE_RESTRICTION 
