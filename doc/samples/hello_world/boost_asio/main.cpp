// This source file is part of the 'tau' open source project.
// Copyright (c) 2016, Yuriy Vosel.
// Licensed under Boost Software License.
// See LICENSE.txt for the licence information.

#include <tau/layout_generation/layout_info.h>
#include <tau/util/basic_events_dispatcher.h>
#include <tau/util/boost_asio_server.h>
class MyEventsDispatcher : public tau::util::BasicEventsDispatcher
{
public:
    MyEventsDispatcher(
        tau::communications_handling::OutgiongPacketsGenerator & outgoingGeneratorToUse): 
            tau::util::BasicEventsDispatcher(outgoingGeneratorToUse)
        {};

    virtual void packetReceived_requestProcessingError(
		std::string const & relatedID, std::string const & additionalData)
    {
        std::cout << "Error received from client:\nRelated elementID/layoutID: "
			<< relatedID << "\nError: " << additionalData << "\n";
    }

    virtual void onClientConnected(
        tau::communications_handling::ClientConnectionInfo const & connectionInfo)
    {
        std::cout << "Client connected: remoteAddr: "
            << connectionInfo.getRemoteAddrDump()
            << ", localAddr : "
            << connectionInfo.getLocalAddrDump() << "\n";
    }
    void packetReceived_clientDeviceInfo(
        tau::communications_handling::ClientDeviceInfo const & info)
    {
        using namespace tau::layout_generation;
        std::cout << "Received client information packet\n";
        std::string greetingMessage = "Hello, world!";
        sendPacket_resetLayout(LayoutInfo().pushLayoutPage(
            LayoutPage(tau::common::LayoutPageID("FIRST_LAYOUT_PAGE"),
            LabelElement(greetingMessage))).getJson());
    }
};

int main(int argc, char ** argv)
{
    boost::asio::io_service io_service;
    short port = 12345;
    tau::util::SimpleBoostAsioServer<MyEventsDispatcher>::type s(io_service, port);
    std::cout << "Starting server on port " << port << "...\n";
    s.start();
    std::cout << "Calling io_service.run()\n";
    io_service.run();
    return 0;
}
