// This source file is part of the 'tau' open source project.
// Copyright (c) 2016, Yuriy Vosel.
// Licensed under Boost Software License.
// See LICENSE.txt for the licence information.

#include "packet_types.h"
#include <sstream>

#ifdef TAU_HEADERONLY
#define LINKAGE_RESTRICTION inline
#else
#include "outgiong_packets_generator.h"
#define LINKAGE_RESTRICTION 
#endif

namespace tau {
namespace communications_handling {
namespace {
	// Small utility function for setting values to boolean parameters of the outgoing packets
	std::string util_bool2string(bool toConvert)
	{
		return toConvert ? "true" : "false";
	}

}

template <typename T>
	std::string generatePacketData(std::string const & commandID, tau::common::BasicWrapperForID<T> const & layoutElementId, std::string const & additionalData)
	{
		std::stringstream packet;
		packet << commandID << "\n" << layoutElementId << "\n" <<
			additionalData << "\n" << outgoing_packets_types::END_COMMAND_FLAG_() << "\n";
		return packet.str();
	}

	LINKAGE_RESTRICTION void OutgiongPacketsGenerator::sendPacket_LayoutUpdate(std::string const & layout_json_data)
	{
		//NOTE: don't get rid of 'toSend' variable in these methods. It is useful in debugging and the compiler should be able to optimize it away itself in release code.
		std::string toSend = generatePacketData(tau::communications_handling::outgoing_packets_types::LAYOUT_UPDATE_(), common::UnassignedID(), layout_json_data);
		sendData(toSend);
	}
	
	LINKAGE_RESTRICTION void OutgiongPacketsGenerator::sendPacket_SwitchLayoutPage(common::LayoutPageID const & layout_page_id)
	{
		std::string toSend = generatePacketData(tau::communications_handling::outgoing_packets_types::SWITCH_LAYOUT_PAGE_(), layout_page_id, "");
		sendData(toSend);
	}

	LINKAGE_RESTRICTION void OutgiongPacketsGenerator::sendPacket_SetNote(common::ElementID const & layout_element_id, std::string const & text)
	{
		std::string toSend = generatePacketData(tau::communications_handling::outgoing_packets_types::SET_NOTE_(), layout_element_id, text);
		sendData(toSend);
	}

	LINKAGE_RESTRICTION void OutgiongPacketsGenerator::sendPacket_SetBooleanValue(common::ElementID const & layout_element_id, bool newValue)
	{		
		std::string toSend = generatePacketData(
			tau::communications_handling::outgoing_packets_types::SET_VALUE_FOR_BOOLEAN_INPUT_(), layout_element_id, util_bool2string(newValue));
		sendData(toSend);
	}
	
	
namespace {
	//Small utility template for converting number to string (it is needed for avoiding code duplication between sendPacket_SetIntValue() and sendPacket_SetFloatPointValue()
	template <typename NumericType>
	std::string getNumericDataStringRepresentation(NumericType value)
	{
		std::stringstream additionalData;
		additionalData << value;
		return additionalData.str();
	}
	
}
	LINKAGE_RESTRICTION void OutgiongPacketsGenerator::sendPacket_SetIntValue(common::ElementID const & layout_element_id, int newValue)
	{
		std::string toSend = generatePacketData(
			tau::communications_handling::outgoing_packets_types::SET_VALUE_FOR_INTEGER_INPUT_(),
			layout_element_id, getNumericDataStringRepresentation(newValue));
		sendData(toSend);
	}

	LINKAGE_RESTRICTION void OutgiongPacketsGenerator::sendPacket_SetFloatPointValue(common::ElementID const & layout_element_id, double newValue)
	{
		std::string toSend = generatePacketData(
			tau::communications_handling::outgoing_packets_types::SET_VALUE_FOR_FLOAT_POINT_INPUT_(),
			layout_element_id, getNumericDataStringRepresentation(newValue));
		sendData(toSend);
	}
	
	LINKAGE_RESTRICTION void OutgiongPacketsGenerator::sendPacket_SetTextValue(common::ElementID const & layout_element_id, std::string const & newValue)
	{
		std::string toSend = generatePacketData(tau::communications_handling::outgoing_packets_types::SET_VALUE_FOR_TEXT_INPUT_(), layout_element_id, newValue);
		sendData(toSend);
	}
	
	LINKAGE_RESTRICTION void OutgiongPacketsGenerator::sendPacket_GetValue(common::ElementID const & layout_element_id)
	{
		std::string toSend = generatePacketData(tau::communications_handling::outgoing_packets_types::GET_VALUE_(), layout_element_id, "");
		sendData(toSend);
	}

	LINKAGE_RESTRICTION void OutgiongPacketsGenerator::sendPacket_SetEnabled(common::ElementID const & layout_element_id, bool isEnabled)
	{
		std::string toSend = generatePacketData(
			tau::communications_handling::outgoing_packets_types::SET_ENABLED_FLAG_(), layout_element_id, util_bool2string(isEnabled));
		sendData(toSend);
	}
	
	LINKAGE_RESTRICTION void OutgiongPacketsGenerator::sendPacket_PutImage(common::ImageID const & imageID, common::ImageResource const & image)
	{
		std::stringstream result;
		image.serialize(result);
		std::string toSend = generatePacketData(
			tau::communications_handling::outgoing_packets_types::PUT_IMAGE_(),
			imageID, result.str());
		sendData(toSend);
	}

	LINKAGE_RESTRICTION void OutgiongPacketsGenerator::sendPacket_SetImageForElement(common::ElementID const & elementToUpdate, common::ImageID const & imageID)
	{
		std::string toSend = generatePacketData(
			tau::communications_handling::outgoing_packets_types::SET_IMAGE_FOR_ELEM_(),
			elementToUpdate, imageID.getValue());
		sendData(toSend);
	}
	
	LINKAGE_RESTRICTION void OutgiongPacketsGenerator::sendPacket_ClearImageRefForElement(common::ElementID const & elementToUpdate)
	{
		std::string toSend = generatePacketData(
			tau::communications_handling::outgoing_packets_types::CLEAR_IMAGE_REF_FOR_ELEMENT_(),
			elementToUpdate, "");
		sendData(toSend);
	}
	
	LINKAGE_RESTRICTION void OutgiongPacketsGenerator::sendPacket_GetLayoutElementPositionInfo(common::ElementID const & layout_element_id)
	{
		std::string toSend = generatePacketData(
			tau::communications_handling::outgoing_packets_types::GET_LAYOUT_ELEM_POSITION_(),
			layout_element_id, "");
		sendData(toSend);
	}
	
	LINKAGE_RESTRICTION void OutgiongPacketsGenerator::sendPacket_SubscribeToSensor(size_t sensorIndex, size_t interval)
	{
		std::stringstream data;
		data << sensorIndex << "\n" << interval;
		std::string toSend = generatePacketData(
			tau::communications_handling::outgoing_packets_types::SUBSCRIBE_TO_SENSOR_(),
			common::UnassignedID(), data.str());
		sendData(toSend);
	}
	
	LINKAGE_RESTRICTION void OutgiongPacketsGenerator::sendPacket_UnsubscribeFromSensor(size_t sensorIndex)
	{
		std::stringstream data;
		data << sensorIndex;		
		std::string toSend = generatePacketData(
			tau::communications_handling::outgoing_packets_types::UNSUBSCRIBE_FROM_SENSOR_(),
			common::UnassignedID(), data.str());
		sendData(toSend);
	}

	LINKAGE_RESTRICTION void OutgiongPacketsGenerator::sendPacket_Heartbeat()
	{
		std::string toSend = generatePacketData(
			tau::communications_handling::outgoing_packets_types::HEARTBEAT_(),
			common::UnassignedID(), "");
		sendData(toSend);
	}
}
}
#undef LINKAGE_RESTRICTION