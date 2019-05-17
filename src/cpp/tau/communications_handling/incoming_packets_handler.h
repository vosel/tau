// This source file is part of the 'tau' open source project.
// Copyright (c) 2016, Yuriy Vosel.
// Licensed under Boost Software License.
// See LICENSE.txt for the licence information.

#pragma once

#include "client_info.h"
#include "../common/element_id.h"
#include "../common/sensor_data.h"
#include <string>
namespace tau {
namespace communications_handling {


//Abstract class for all the handling of the incoming packets
class IncomingPacketsHandler
{
public:
	virtual void onPacketReceived(std::string const & packetType, std::string const & layoutElementID, std::string const & additionalData);

	virtual void onPacketReceived_ButtonClick(common::ElementID const & buttonID);
	virtual void onPacketReceived_ClientDeviceInfo(ClientDeviceInfo const & info);

	virtual void onPacketReceived_BoolValue(common::ElementID const & inputBoxID, bool value);
	virtual void onPacketReceived_BoolValueInfoAutoUpdate(common::ElementID const & inputBoxID, bool value);
	virtual void onPacketReceived_IntValue(common::ElementID const & inputBoxID, int value);
	virtual void onPacketReceived_IntValueInfoAutoUpdate(common::ElementID const & inputBoxID, int value);
	virtual void onPacketReceived_FloatPointValue(common::ElementID const & inputBoxID, double value);
	virtual void onPacketReceived_FloatPointValueInfoAutoUpdate(common::ElementID const & inputBoxID, double value);
	virtual void onPacketReceived_TextValue(common::ElementID const & inputBoxID, std::string const & value);
	virtual void onPacketReceived_TextValueInfoAutoUpdate(common::ElementID const & inputBoxID, std::string const & value);
	
	virtual void onPacketReceived_LayoutPageSwitched(common::LayoutPageID const & newActiveLayoutPageID);
	virtual void onPacketReceived_LayoutRefreshed(common::LayoutID const & layoutID);
	virtual void onPacketReceived_ImageUpdated(common::ImageID const & imageID);

	virtual void onPacketReceived_LayoutElementPosition(common::ElementID const & imageID, size_t x, size_t y, size_t width, size_t height);
	virtual void onPacketReceived_LayoutElementImageSizeInfo(common::ElementID const & imageID, size_t width, size_t height);
	virtual void onPacketReceived_SensorsDataUpdate(common::SensorData const & data);
	
	virtual void onPacketReceived_HeartbeatResponse();
		
	virtual void onPacketReceived_ServerRequestProcessingError(std::string const & layoutID, std::string const & additionalData);

	// This method is called in situations there is an error which can't be resolved in terms of the
	// communication protocol.
	virtual void unknownErrorOccured(std::string const & errorMessage);
#if defined(TAU_CPP_03_COMPATIBILITY)
	virtual ~IncomingPacketsHandler() {};
#else
	virtual ~IncomingPacketsHandler() = default; 
#endif
};
}
}
#ifdef TAU_HEADERONLY
#include "incoming_packets_handler.cpp"
#endif
