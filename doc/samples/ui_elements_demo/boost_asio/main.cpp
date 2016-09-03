// This source file is part of the 'tau' open source project.
// Copyright (c) 2016, Yuriy Vosel.
// Licensed under Boost Software License.
// See LICENSE.txt for the licence information.

#include <tau/layout_generation/layout_info.h>
#include <tau/util/basic_events_dispatcher.h>
#include <tau/util/boost_asio_server.h>
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
    MyEventsDispatcher(
        tau::communications_handling::OutgiongPacketsGenerator & outgoingGeneratorToUse): 
            tau::util::BasicEventsDispatcher(outgoingGeneratorToUse)
        {};

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
