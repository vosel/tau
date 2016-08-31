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

#include <ctime>
namespace {
    tau::common::ElementID const ID_ELEM_BUTTON("ELEM_BUT");
    tau::common::ElementID const ID_ELEM_BOOL_INPUT("ELEM_BOOL");
    tau::common::ElementID const ID_ELEM_INT_INPUT("ELEM_INT");
    tau::common::ElementID const ID_ELEM_FLOAT_INPUT("ELEM_FLOAT");
    tau::common::ElementID const ID_ELEM_TEXT_INPUT("ELEM_TXT");
    tau::common::ElementID const ID_INFO_LABEL_BUTTON("LBL_BUT");
    tau::common::ElementID const ID_INFO_LABEL_BOOL_INPUT("LBL_BOOL");
    tau::common::ElementID const ID_INFO_LABEL_INT_INPUT("LBL_INT");
    tau::common::ElementID const ID_INFO_LABEL_FLOAT_INPUT("LBL_FLOAT");
    tau::common::ElementID const ID_INFO_LABEL_TEXT_INPUT("LBL_TXT");
    std::string getDaytimeString()
    {  
        std::time_t result = std::time(0);
        return std::ctime(&result);
    }
};
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
private:
    std::string generateLayout(bool notifyServerAboutEdits) {
        using namespace tau::layout_generation;
        UnevenlySplitElementsPair topmostLayoutElement(
            EvenlySplitLayoutElementsContainer(false)
                    .push(TextInputLayoutElement().ID(ID_ELEM_TEXT_INPUT).shouldNotifyOnEdit(notifyServerAboutEdits).initialValue("abc"))
                    .push(LabelElement("abc").ID(ID_INFO_LABEL_TEXT_INPUT)),
            EvenlySplitLayoutElementsContainer(true)
                .push(EvenlySplitLayoutElementsContainer(false)
                    .push(NumberInputLayoutElement(10).allowFloatPoint(false).ID(ID_ELEM_INT_INPUT).shouldNotifyOnEdit(notifyServerAboutEdits))
                    .push(NumberInputLayoutElement(10.1).allowFloatPoint(true).ID(ID_ELEM_FLOAT_INPUT).shouldNotifyOnEdit(notifyServerAboutEdits)))
                    .push(EvenlySplitLayoutElementsContainer(false)
                        .push(LabelElement("10").ID(ID_INFO_LABEL_INT_INPUT))
                        .push(LabelElement("10.1").ID(ID_INFO_LABEL_FLOAT_INPUT)))
                .push(BooleanInputLayoutElement(true).note("flag").ID(ID_ELEM_BOOL_INPUT).shouldNotifyOnEdit(notifyServerAboutEdits))
                .push(LabelElement("checked").ID(ID_INFO_LABEL_BOOL_INPUT))
                .push(ButtonLayoutElement().note("button").ID(ID_ELEM_BUTTON))
                .push(LabelElement("not clicked").ID(ID_INFO_LABEL_BUTTON))
                .push(EmptySpace()),
            true, 0.15);
        return LayoutInfo().pushLayoutPage(LayoutPage(tau::common::LayoutPageID("FIRST_LAYOUT_PAGE"), topmostLayoutElement)).getJson();
    }; 

public:
    virtual void packetReceived_clientDeviceInfo(tau::communications_handling::ClientDeviceInfo const & info) {
        sendPacket_resetLayout(generateLayout(true));
    };
    virtual void packetReceived_buttonClick(tau::common::ElementID const & buttonID) {
        std::cout << "event: buttonClick, id=" << buttonID << "\n";
        std::stringstream labelText;
        labelText << "Button clicked at:  " << getDaytimeString();
        sendPacket_changeElementNote(ID_INFO_LABEL_BUTTON, labelText.str());
    };
    virtual void packetReceived_boolValueUpdate(tau::common::ElementID const & inputBoxID, bool new_value, bool is_automatic_update) {
        std::cout << "event: boolValueUpdate, id=" << inputBoxID << ", new value=" << new_value << "\n";
        sendPacket_changeElementNote(ID_INFO_LABEL_BOOL_INPUT, new_value ? "checked" : "unchecked");
    };
    virtual void packetReceived_intValueUpdate(tau::common::ElementID const & inputBoxID, int new_value, bool is_automatic_update) {
        std::cout << "event: intValueUpdate, id=" << inputBoxID << ", new value=" << new_value << "\n";
        std::stringstream note;
        note << "current value: " << new_value;
        sendPacket_changeElementNote(ID_INFO_LABEL_INT_INPUT, note.str());
    };
    
    virtual void packetReceived_floatPointValueUpdate(tau::common::ElementID const & inputBoxID, double new_value, bool is_automatic_update) {
        std::cout << "event: floatPointValueUpdate, id=" << inputBoxID << ", new value=" << new_value << "\n";
        std::stringstream note;
        note << "current value: " << new_value;
        sendPacket_changeElementNote(ID_INFO_LABEL_FLOAT_INPUT, note.str());
    };
    
    virtual void packetReceived_textValueUpdate(tau::common::ElementID const & inputBoxID, std::string const & new_value, bool is_automatic_update) {
        std::cout << "event: textValueUpdate, id=" << inputBoxID << ", new value=" << new_value << "\n";
        sendPacket_changeElementNote(ID_INFO_LABEL_TEXT_INPUT, new_value);
    };
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
