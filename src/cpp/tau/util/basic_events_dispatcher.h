// This source file is part of the 'tau' open source project.
// Copyright (c) 2016, Yuriy Vosel.
// Licensed under Boost Software License.
// See LICENSE.txt for the licence information.

#pragma once

#include "../common/element_id.h"
#include "../common/image_resource.h"
#include "../common/sensor_data.h"
#include "../communications_handling/abstract_events_dispatcher.h"
#include "../layout_generation/my_shared_ptr.h"
#include <string>

namespace tau {
namespace util {

class BasicEventsDispatcher:
	public tau::communications_handling::AbstractEventsDispatcher
{
protected:
	tau::layout_generation::my_shared_ptr<tau::communications_handling::IncomingPacketsHandler>::MyInternalType m_incomingPacketsHandler;
	tau::layout_generation::my_shared_ptr<tau::communications_handling::CommunicationIssuesHandler>::MyInternalType m_communicationIssuesHandler;
	tau::communications_handling::OutgiongPacketsGenerator & m_outgiongPacketsGenerator;
	BasicEventsDispatcher(tau::communications_handling::OutgiongPacketsGenerator & outgoingPacketsGeneratorToUse);
public:

	virtual void packetReceived_clientDeviceInfo(tau::communications_handling::ClientDeviceInfo const & info) {};
	virtual void packetReceived_buttonClick(common::ElementID const & buttonID) {};
	virtual void packetReceived_boolValueUpdate(common::ElementID const & inputBoxID, bool new_value, bool is_automatic_update) {};
	virtual void packetReceived_intValueUpdate(common::ElementID const & inputBoxID, int new_value, bool is_automatic_update) {};
	virtual void packetReceived_floatPointValueUpdate(common::ElementID const & inputBoxID, double new_value, bool is_automatic_update) {};
	virtual void packetReceived_textValueUpdate(common::ElementID const & inputBoxID, std::string const & new_value, bool is_automatic_update) {};
	virtual void packetReceived_layoutPageSwitched(common::LayoutPageID const & newActiveLayoutPageID) {};
	virtual void packetReceived_layoutRefreshed(common::LayoutID const & layoutID) {};
	virtual void packetReceived_requestProcessingError(std::string const & layoutID, std::string const & additionalData) {};
	virtual void packetReceived_imageUpdated(common::ImageID const & imageID) {};
	virtual void packetReceived_layoutElementPosition(common::ElementID const & elementID, size_t x, size_t y, size_t width, size_t height) {};
	virtual void packetReceived_sensorsDataUpdate(common::SensorData const & data) {};
	virtual void communicationErrorOccured(std::string const & errorDescription) {};
	virtual void unknownErrorOccured(std::string const & errorMessage) {};

	virtual void sendPacket_resetLayout(std::string const & layoutJson);
	virtual void sendPacket_requestValue(common::ElementID const & inputID);
	
	virtual void sendPacket_updateBooleanValue(common::ElementID const & inputID, bool newValue);
	virtual void sendPacket_updateIntValue(common::ElementID const & inputID, int newValue);
	virtual void sendPacket_updateFloatPointValue(common::ElementID const & inputID, double newValue);
	virtual void sendPacket_updateTextValue(common::ElementID const & inputID, std::string const & newValue);
	
	virtual void sendPacket_changeElementNote(common::ElementID const & elementID, std::string const & note);
	virtual void sendPacket_changeShownLayoutPage(common::LayoutPageID const & layoutPageIdToSwitchTo);
	virtual void sendPacket_changeElementEnabledState(common::ElementID const & elementID, bool newState);
	
	virtual void sendPacket_getLayoutElementPositionInfo(common::ElementID const & elementID);
	virtual void sendPacket_putImage(common::ImageID const & imageID, common::ImageResource const & image);
	virtual void sendPacket_subscribeToSensor(size_t sensorIndex, size_t interval);
	virtual void sendPacket_unsubscribeFromSensor(size_t sensorIndex);
	
	virtual tau::communications_handling::IncomingPacketsHandler * getIncomingPacketsHandler();
	virtual tau::communications_handling::CommunicationIssuesHandler * getCommunicationIssuesHandler();
	
	void closeConnection();
#if defined(TAU_CPP_03_COMPATIBILITY)
	virtual ~BasicEventsDispatcher() {};
#else
	virtual ~BasicEventsDispatcher() = default; 
#endif
};

} //namespace tau::util
} //namespace tau

#ifdef TAU_HEADERONLY
#include "basic_events_dispatcher.cpp"
#endif
