// This source file is part of the 'tau' open source project.
// Copyright (c) 2016, Yuriy Vosel.
// Licensed under Boost Software License.
// See LICENSE.txt for the licence information.

#pragma once
#include <string>

namespace tau {
namespace communications_handling {
	struct incoming_packets_types {
		enum TypeID {
			ButtonClick,
			ClientDeviceInfo,
			BoolValue,
			BoolValueInfoAutoUpdate,
			IntValue,
			IntValueInfoAutoUpdate,
			FloatPointValue,
			FloatPointValueInfoAutoUpdate,
			TextValue,
			TextValueInfoAutoUpdate,
			ServerRequestProcessingError,
			LayoutRefreshed,
			LayoutPageSwitched,
			ImageUpdated,
			LayoutElementPositionInfo,
			SensorsDataUpdate,
			HeartbeatResponse,
			UnknownPacketType
		};
		//This packet is sent when a button is clicked on client.
		static const char * BUTTON_CLICK_()							{ static const char * result = "ButtonClick";				return result; }

		//This packet is sent at the beginning of the communication. It informs server about the preferences of the connected client (screen resolution, device id, e.t.c.)
		static const char * CLIENT_DEVICE_INFO_()					{ static const char * result = "ClientDeviceInfo";		return result; }

		// These 4 packets are sent from client to server to inform it about the value in one of the user input controls.
		// They are sent as a response packets to the server's request about this value.
		static const char * BOOL_VALUE_()							{ static const char * result = "BoolValue";				return result; }
		static const char * INT_VALUE_()							{ static const char * result = "IntValue";				return result; }
		static const char * FLOAT_POINT_VALUE_()					{ static const char * result = "FloatPointValue";				return result; }
		static const char * TEXT_VALUE_()							{ static const char * result = "TextValue";				return result; }
		
		// These 4 packets are sent from client to server to inform it about the value in one of the user input controls.
		// They are sent by client automatically when a specific event triggers the client to do so.
		// The set of events which will trigger the client to send such packets are determined by the attributes of the elements in the current layout.
		// Here are the examples, when one of these packets are generated:
		//   - A button is clicked and this button has an input element's ID of the input element in it's `informServerAboutValueOnClick` attribute (see the ButtonElement's interface).
		//   - A value in the input element is modified by the user and the element has `shouldNotifyOnEdit` attribute set.
		static const char * BOOL_VALUE_INFO_AUTOMATIC_UPDATE_()			{ static const char * result = "BoolValueInfoAutoUpdate";	return result; }
		static const char * INT_VALUE_INFO_AUTOMATIC_UPDATE_()			{ static const char * result = "IntValueInfoAutoUpdate";	return result; }
		static const char * FLOAT_POINT_VALUE_INFO_AUTOMATIC_UPDATE_()	{ static const char * result = "FloatPointValueInfoAutoUpdate";	return result; }
		static const char * TEXT_VALUE_INFO_AUTOMATIC_UPDATE_()			{ static const char * result = "TextValueInfoAutoUpdate";	return result; }

		// This packet is sent by client in case the packet it received is malformed.
		// The ServerRequestProcessingError packet usually contains a human-readable error message, which may provide information, what is the problem.
		static const char * SERVER_REQUEST_PROCESSING_ERRROR_()	{ static const char * result = "ServerRequestProcessingError";		return result; }

		// This packet is sent from the client to server when a new layout is processed and applied on client.
		// It is a notification to the server that the layout was accepted without errors.
		static const char * LAYOUT_REFRESHED_()					{ static const char * result = "LayoutRefreshed";			return result; }

		// This packet is sent to the server by client when (for any reason) the displayed layout page is switched. This may happen either after a server request, or after a specific event on the client itself.
		static const char * LAYOUT_PAGE_SWITCHED_()				{ static const char * result = "LayoutSheetSwitched";		return result; }
		
		static const char * IMAGE_UPDATED_()                    { static const char * result = "ImageUpdated";		return result; }
		static const char * LAYOUT_ELEMENT_POSITION_INFO_()     { static const char * result = "LayoutElementPositionInfo";		return result; }
		
		static const char * SENSORS_DATA_UPDATE_()     { static const char * result = "SensorsDataUpdate";		return result; }
		
		//This packet should be sent by the client when it receives the heartbeat packed from server. If it is not sent, there is probably a connection problem.
		static const char * HEARTBEAT_RESPONSE_()		   { static const char * result = "HeartbeatResponse";	return result; }
		
		static TypeID getTypeID(std::string const & packet_type_string);
	};

	struct outgoing_packets_types {
		static const char * LAYOUT_UPDATE_()		{ static const char * result = "LayoutUpdate";		return result; }
		static const char * END_COMMAND_FLAG_()		{ static const char * result = "CommandEnd";			return result; }
		static const char * SET_NOTE_()				{ static const char * result = "SetNote";				return result; }
		static const char * GET_VALUE_()			{ static const char * result = "GetValue";			return result; }
		static const char * SWITCH_LAYOUT_PAGE_()	{ static const char * result = "SwitchLayoutSheet";	return result; }
		static const char * SET_VALUE_FOR_BOOLEAN_INPUT_()		{ static const char * result = "SetBoolValue";		return result; }
		static const char * SET_VALUE_FOR_INTEGER_INPUT_()		{ static const char * result = "SetIntValue";			return result; }
		static const char * SET_VALUE_FOR_FLOAT_POINT_INPUT_()	{ static const char * result = "SetFloatPointValue";	return result; }
		static const char * SET_VALUE_FOR_TEXT_INPUT_()			{ static const char * result = "SetTextValue";		return result; }
		static const char * SET_ENABLED_FLAG_()					{ static const char * result = "SetEnabled";		return result; }
		static const char * PUT_IMAGE_()						{ static const char * result = "PutImage";	return result; }
		static const char * SET_IMAGE_FOR_ELEM_()				{ static const char * result = "SetImageRefForElem";	return result; }
		static const char * GET_LAYOUT_ELEM_POSITION_()		    { static const char * result = "GetLayoutElemPosition";	return result; }
		static const char * SUBSCRIBE_TO_SENSOR_()		        { static const char * result = "SubscribeToSensor";	return result; }
		static const char * UNSUBSCRIBE_FROM_SENSOR_()		    { static const char * result = "UnsubscribeFromSensor";	return result; }
		static const char * HEARTBEAT_()		   	   	   	      { static const char * result = "Heartbeat";	return result; }
	};
}
}
#ifdef TAU_HEADERONLY
#include "packet_types.cpp"
#endif