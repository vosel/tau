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

#include <cassert>
#include <ctime>
namespace {
    tau::common::LayoutPageID const LAYOUT_PAGE_1("FIRST_LAYOUT_PAGE");
    tau::common::LayoutPageID const LAYOUT_PAGE_2("SECOND_LAYOUT_PAGE");
    tau::common::ElementID const BOOLEAN_INPUT("BOOL_INP");
    tau::common::ElementID const NUMERIC_INPUT("NUM_INP");
    tau::common::ElementID const TEXT_INPUT("TXT_INP");
    tau::common::ElementID const BUTTON_ELEMENT("BUTTON");
    tau::common::ElementID const BUTTON_ENABLE_DISABLE("BUTTON_ENABLE_DISABLE");
    tau::common::ElementID const LABEL_ELEMENT("LABEL");
    tau::common::ElementID const BUTTON_GO_TO_SECOND_PAGE("BUTTON_GO_TO_SECOND");
    tau::common::ElementID const BUTTON_GO_TO_FIRST_PAGE("BUTTON_GO_TO_FIRST");
    tau::common::ElementID const BUTTON_RESET_VALUES("BUTTON_RESET_VALUES");
    std::string getDaytimeString()
    {  
        std::time_t result = std::time(0);
        return std::ctime(&result);
    }
};
class MyEventsDispatcher : public tau::util::BasicEventsDispatcher
{
    bool m_enabled;
public:
    MyEventsDispatcher(tau::communications_handling::OutgiongPacketsGenerator & outgoingPacketsGeneratorToUse)
        :tau::util::BasicEventsDispatcher(outgoingPacketsGeneratorToUse), m_enabled(true) {}
    virtual void packetReceived_requestProcessingError(
		std::string const & layoutID, std::string const & additionalData)
    {
        std::cout << "Error received from client:\nLayouID: "
			<< layoutID << "\nError: " << additionalData << "\n";
    }

    virtual void onClientConnected(
        tau::communications_handling::ClientConnectionInfo const & connectionInfo)
    {
        std::cout << "Client connected: remoteAddr: "
            << connectionInfo.getRemoteAddrDump()
            << ", localAddr : "
            << connectionInfo.getLocalAddrDump() << "\n";
    }
    virtual void packetReceived_clientDeviceInfo(tau::communications_handling::ClientDeviceInfo const & info) {
        using namespace tau::layout_generation;
        UnevenlySplitElementsPair layout_page_1(
            LabelElement("Layout page 1."), 
            EvenlySplitLayoutElementsContainer(false)
                .push(EvenlySplitLayoutElementsContainer(true)
                    .push(NumberInputLayoutElement(12345).allowFloatPoint(false).ID(NUMERIC_INPUT).shouldNotifyOnEdit(false))
                    .push(TextInputLayoutElement().ID(TEXT_INPUT).initialValue("text to edit").shouldNotifyOnEdit(false))
                    .push(BooleanInputLayoutElement(true).note("flag").ID(BOOLEAN_INPUT).shouldNotifyOnEdit(false))
                    .push(ButtonLayoutElement().note("button").ID(BUTTON_ELEMENT))
                    .push(LabelElement("...").ID(LABEL_ELEMENT))
                    .push(ButtonLayoutElement().note("disable").ID(BUTTON_ENABLE_DISABLE))
                    .push(ButtonLayoutElement().note("reset input values").ID(BUTTON_RESET_VALUES))
                    .push(ButtonLayoutElement().note("go to page 2").ID(BUTTON_GO_TO_SECOND_PAGE))
                    ),
            true, 0.15);
        UnevenlySplitElementsPair layout_page_2(
            LabelElement("Layout page 2."), 
            ButtonLayoutElement().note("go to page 1").ID(BUTTON_GO_TO_FIRST_PAGE), true, 0.5);
            
        sendPacket_resetLayout(LayoutInfo()
            .pushLayoutPage(LayoutPage(LAYOUT_PAGE_1, layout_page_1))
            .pushLayoutPage(LayoutPage(LAYOUT_PAGE_2, layout_page_2)).setStartLayoutPage(LAYOUT_PAGE_1).getJson());
    }
 virtual void packetReceived_buttonClick(tau::common::ElementID const & buttonID) {
        std::cout << "event: buttonClick, id=" << buttonID << "\n";
        if (buttonID == BUTTON_ELEMENT) {
            std::string datetime = getDaytimeString();
            sendPacket_changeElementNote(BOOLEAN_INPUT, datetime);
            sendPacket_changeElementNote(BUTTON_ELEMENT, datetime);
            sendPacket_changeElementNote(LABEL_ELEMENT, datetime);
            std::cout << "Requesting the current values in the input controls...\n";
            sendPacket_requestValue(BOOLEAN_INPUT);
            sendPacket_requestValue(NUMERIC_INPUT);
            sendPacket_requestValue(TEXT_INPUT);
        } else if (buttonID == BUTTON_ENABLE_DISABLE) {
            m_enabled = !m_enabled;
            sendPacket_changeElementEnabledState(BUTTON_ELEMENT, m_enabled);
            sendPacket_changeElementEnabledState(BOOLEAN_INPUT, m_enabled);
            sendPacket_changeElementEnabledState(NUMERIC_INPUT, m_enabled);
            sendPacket_changeElementEnabledState(TEXT_INPUT, m_enabled);
            sendPacket_changeElementNote(BUTTON_ENABLE_DISABLE, m_enabled ? "disable" : "enable");
        } else if (buttonID == BUTTON_GO_TO_FIRST_PAGE) {
            sendPacket_changeShownLayoutPage(LAYOUT_PAGE_1);
        } else if (buttonID == BUTTON_GO_TO_SECOND_PAGE) {
            sendPacket_changeShownLayoutPage(LAYOUT_PAGE_2);
        } else if (buttonID == BUTTON_RESET_VALUES) {
            sendPacket_updateBooleanValue(BOOLEAN_INPUT, true);
            sendPacket_updateIntValue(NUMERIC_INPUT, 10);
            sendPacket_updateTextValue(TEXT_INPUT, "default text");
        } else {
            assert(false); //unreachable code
        }
    }
    virtual void packetReceived_layoutRefreshed(tau::common::LayoutPageID const & newActiveLayoutPageID) {
        std::cout << "event: layoutPageSwitch, id=" << newActiveLayoutPageID << "\n";
    }
    
    virtual void packetReceived_boolValueUpdate(tau::common::ElementID const & inputBoxID, bool new_value, bool is_automatic_update) {
        std::cout << "event: boolValueUpdate, id=" << inputBoxID << ", value=" << new_value << "\n";
    }
    
    virtual void packetReceived_intValueUpdate(tau::common::ElementID const & inputBoxID, int new_value, bool is_automatic_update) {
        std::cout << "event: intValueUpdate, id=" << inputBoxID << ", value=" << new_value << "\n";
    }
    
    virtual void packetReceived_floatPointValueUpdate(tau::common::ElementID const & inputBoxID, double new_value, bool is_automatic_update) {
        std::cout << "event: floatPointValueUpdate, id=" << inputBoxID << ", value=" << new_value << "\n";
    }
    
    virtual void packetReceived_textValueUpdate(tau::common::ElementID const & inputBoxID, std::string const & new_value, bool is_automatic_update) {
        std::cout << "event: textValueUpdate, id=" << inputBoxID << ", value=" << new_value << "\n";
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
    virtual void close_connection() {
        close(m_socketHandle);
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
