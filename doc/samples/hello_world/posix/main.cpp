// This source file is part of the 'tau' open source project.
// Copyright (c) 2016, Yuriy Vosel.
// Licensed under Boost Software License.
// See LICENSE.txt for the licence information.

#include <tau/layout_generation/layout_info.h>
#include <tau/util/basic_events_dispatcher.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <tau/communications_handling/outgiong_packets_generator.h>
#include <tau/communications_handling/incoming_data_stream_parser.h>

class MyEventsDispatcher : public tau::util::BasicEventsDispatcher
{
public:
    MyEventsDispatcher(tau::communications_handling::OutgiongPacketsGenerator & outgoingPacketsGeneratorToUse)
        :tau::util::BasicEventsDispatcher(outgoingPacketsGeneratorToUse) {};

    void packetReceived_requestProcessingError(std::string const & layoutID, std::string const & additionalData)
    {
        std::cout << "Error received from client:\nLayouID: " << layoutID << "\nError: " << additionalData << "\n";
    }

    void onClientConnected(tau::communications_handling::ClientConnectionInfo const & connectionInfo)
    {
        std::cout << "Client connected: remoteAddr: " << connectionInfo.getRemoteAddrDump() << ", localAddr : " << connectionInfo.getLocalAddrDump() << "\n";
    }
    void packetReceived_clientDeviceInfo(tau::communications_handling::ClientDeviceInfo const & info) {
        using namespace tau::layout_generation;
        std::cout << "Received client information packet\n";
        std::string greetingMessage = "Hello, world!";
        sendPacket_resetLayout(LayoutInfo().pushLayoutPage(LayoutPage(tau::common::LayoutPageID("FIRST_LAYOUT_PAGE"), LabelElement(greetingMessage))).getJson());
    }
};
class MyOutgoingPacketsGenerator : 
    public tau::communications_handling::OutgiongPacketsGenerator
{
    int m_socketHandle;
public:
    MyOutgoingPacketsGenerator(int output_socket_handle): m_socketHandle(output_socket_handle)
    {};
    virtual void sendData(std::string const & data) {
        write(m_socketHandle, data.c_str(), data.size());
    }
};
std::string getAddrString(sockaddr_in const & toConvert) {
	int buflen = INET_ADDRSTRLEN;
	char buf[buflen];
	return inet_ntop(AF_INET, &(toConvert.sin_addr), buf, buflen);
}

int main(int argc, char ** argv)
{
    int socket_handle = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_handle == -1) {
        std::cerr << "Can't create the socket. Exiting.\n";
        return -1;
    }
    std::cout << "Socket created. ";
    int listenPort = 12345;
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(listenPort);

    if (bind(socket_handle, (sockaddr*)(&serverAddr), sizeof(serverAddr)) < 0) {
        std::cerr << "Can't bind the socket. Exiting.\n";
        return -2;
    }
    std::cout << "bind() succeded. Starting to listen on the socket (port " << listenPort << ")...\n";
    listen(socket_handle, 5);
    sockaddr_in client;
    int client_socket_size = sizeof(sockaddr_in);
    int client_handle = accept(socket_handle, (sockaddr*)(&client), (socklen_t*)&client_socket_size);
    if (client_handle < 0) {
        std::cerr << "Accept failed. Exiting.\n";
        return -3;
    }
    std::cout << "Connection accepted.";
    int read_bufSize;
    const int BUFSIZE = 1024;
    char buffer[BUFSIZE];
    MyOutgoingPacketsGenerator outgoingPacketsGenerator(client_handle);
    MyEventsDispatcher myServerLogic(outgoingPacketsGenerator);
    tau::communications_handling::IncomingDataStreamParser incomingDataStreamParser;
    
    tau::communications_handling::ClientConnectionInfo connectionInfo(getAddrString(client), ntohs(client.sin_port), getAddrString(serverAddr), ntohs(serverAddr.sin_port));
    myServerLogic.onClientConnected(connectionInfo); //Notify the user code about the connection details
    
    while((read_bufSize = recv(client_handle, buffer, BUFSIZE, 0)) > 0) {
        std::string received_data(buffer, read_bufSize);
        incomingDataStreamParser.newData(
                received_data, 
                myServerLogic.getIncomingPacketsHandler(), 
                myServerLogic.getCommunicationIssuesHandler()
            );
    }
    myServerLogic.onConnectionClosed();
    if (read_bufSize == 0) {
        std::cout << "Client disconnected. Exiting.\n";
    } else if (read_bufSize == -1) {
        std::cout << "recv() failed. Exiting.\n";
        return -4;
    }
    return 0;
}
