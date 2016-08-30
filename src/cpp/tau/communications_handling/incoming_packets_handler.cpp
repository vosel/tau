// This source file is part of the 'tau' open source project.
// Copyright (c) 2016, Yuriy Vosel.
// Licensed under Boost Software License.
// See LICENSE.txt for the licence information.

#ifndef TAU_HEADERONLY
#include "incoming_packets_handler.h"
#endif

#include "packet_types.h"
#include <sstream>
#include <iostream>

#ifdef TAU_HEADERONLY
#define LINKAGE_RESTRICTION inline
#else
#define LINKAGE_RESTRICTION 
#endif

namespace tau {
namespace communications_handling {

namespace {
	//Set of utility functions for parsing the incoming packets data
	bool extractBool(std::string const & string_to_process)
	{
		return (string_to_process == "true");
	}
	
	template <typename T>
	LINKAGE_RESTRICTION T extractNumber(std::string const & string_to_process)
	{
		std::stringstream stream(string_to_process);
		T result = 0;
		stream >> result;
		return result;
	}
}

LINKAGE_RESTRICTION void IncomingPacketsHandler::onPacketReceived(
	std::string const & packetType, std::string const & layoutElementID, std::string const & additionalData)
{
	incoming_packets_types::TypeID typeID = incoming_packets_types::getTypeID(packetType);
	switch (typeID) {
		case incoming_packets_types::ButtonClick:
			onPacketReceived_ButtonClick(common::ElementID(layoutElementID));
			break;
		case incoming_packets_types::ClientDeviceInfo: {
				std::stringstream stream(additionalData);
				size_t width = 0;
				size_t height = 0;
				std::string deviceID;
				size_t clientVersion;
				stream >> width >> height;
				stream >> deviceID;
				stream >> clientVersion;
				ClientDeviceInfo info(width, height, deviceID, clientVersion);
				onPacketReceived_ClientDeviceInfo(info);
			}
			break;
		case incoming_packets_types::BoolValue:
			onPacketReceived_BoolValue(common::ElementID(layoutElementID), extractBool(additionalData));
			break;
		case incoming_packets_types::BoolValueInfoAutoUpdate:
			onPacketReceived_BoolValueInfoAutoUpdate(common::ElementID(layoutElementID), extractBool(additionalData));
			break;
		case incoming_packets_types::IntValue:
			onPacketReceived_IntValue(common::ElementID(layoutElementID), extractNumber<int>(additionalData));
			break;
		case incoming_packets_types::IntValueInfoAutoUpdate:
			onPacketReceived_IntValueInfoAutoUpdate(common::ElementID(layoutElementID), extractNumber<int>(additionalData));
			break;
		case incoming_packets_types::FloatPointValue:
			onPacketReceived_FloatPointValue(common::ElementID(layoutElementID), extractNumber<double>(additionalData));
			break;
		case incoming_packets_types::FloatPointValueInfoAutoUpdate:
			onPacketReceived_FloatPointValueInfoAutoUpdate(common::ElementID(layoutElementID), extractNumber<double>(additionalData));
			break;
		case incoming_packets_types::TextValue:
			onPacketReceived_TextValue(common::ElementID(layoutElementID), additionalData);
			break;
		case incoming_packets_types::TextValueInfoAutoUpdate:
			onPacketReceived_TextValueInfoAutoUpdate(common::ElementID(layoutElementID), additionalData);
			break;
		case incoming_packets_types::LayoutRefreshed:
			onPacketReceived_LayoutRefreshed(common::LayoutID(layoutElementID));
			break;
		case incoming_packets_types::LayoutPageSwitched:
			onPacketReceived_LayoutPageSwitched(common::LayoutPageID(layoutElementID));
			break;
		case incoming_packets_types::ServerRequestProcessingError:
			onPacketReceived_ServerRequestProcessingError(layoutElementID, additionalData);
			break;
		default: 
			std::stringstream error;
			error << "Unknown type of packet is received: " << packetType;
			unknownErrorOccured(error.str());
			break;
	}
}
LINKAGE_RESTRICTION void IncomingPacketsHandler::unknownErrorOccured(std::string const & errorMessage)
{}

LINKAGE_RESTRICTION void IncomingPacketsHandler::onPacketReceived_ButtonClick(common::ElementID const & buttonID)
{}

LINKAGE_RESTRICTION void IncomingPacketsHandler::onPacketReceived_ClientDeviceInfo(ClientDeviceInfo const & info)
{}

LINKAGE_RESTRICTION void IncomingPacketsHandler::onPacketReceived_BoolValue(common::ElementID const & inputBoxID, bool value)
{}

LINKAGE_RESTRICTION void IncomingPacketsHandler::onPacketReceived_BoolValueInfoAutoUpdate(common::ElementID const & inputBoxID, bool value)
{
	onPacketReceived_BoolValue(inputBoxID, value);
}

LINKAGE_RESTRICTION void IncomingPacketsHandler::onPacketReceived_IntValue(common::ElementID const & inputBoxID, int value)
{}

LINKAGE_RESTRICTION void IncomingPacketsHandler::onPacketReceived_IntValueInfoAutoUpdate(common::ElementID const & inputBoxID, int value)
{
	onPacketReceived_IntValue(inputBoxID, value);
}

LINKAGE_RESTRICTION void IncomingPacketsHandler::onPacketReceived_FloatPointValue(common::ElementID const & inputBoxID, double value)
{}

LINKAGE_RESTRICTION void IncomingPacketsHandler::onPacketReceived_FloatPointValueInfoAutoUpdate(common::ElementID const & inputBoxID, double value)
{
	onPacketReceived_FloatPointValue(inputBoxID, value);
}

LINKAGE_RESTRICTION void IncomingPacketsHandler::onPacketReceived_TextValue(common::ElementID const & inputBoxID, std::string const & value)
{}

LINKAGE_RESTRICTION void IncomingPacketsHandler::onPacketReceived_TextValueInfoAutoUpdate(common::ElementID const & inputBoxID, std::string const & value)
{
	onPacketReceived_TextValue(inputBoxID, value);
}

LINKAGE_RESTRICTION void IncomingPacketsHandler::onPacketReceived_LayoutRefreshed(common::LayoutID const & layoutID)
{}

LINKAGE_RESTRICTION void IncomingPacketsHandler::onPacketReceived_LayoutPageSwitched(common::LayoutPageID const & newActiveLayoutPageID)
{}

LINKAGE_RESTRICTION void IncomingPacketsHandler::onPacketReceived_ServerRequestProcessingError(std::string const & layoutID, std::string const & additionalData)
{}

}
}

#undef LINKAGE_RESTRICTION 