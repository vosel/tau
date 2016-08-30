// This source file is part of the 'tau' open source project.
// Copyright (c) 2016, Yuriy Vosel.
// Licensed under Boost Software License.
// See LICENSE.txt for the licence information.

#ifdef TAU_HEADERONLY
#define LINKAGE_RESTRICTION inline
#else
#include "packet_types.h"
#define LINKAGE_RESTRICTION 
#endif
#include <map>

namespace tau {
namespace communications_handling {

namespace {
	typedef std::map<std::string, incoming_packets_types::TypeID> RegistryType;

	//Small utility function for incoming_packets_types::getTypeID()
	RegistryType createTypeidRegistry()
	{
		RegistryType result;
		result[incoming_packets_types::BUTTON_CLICK_()]                            = incoming_packets_types::ButtonClick;
		result[incoming_packets_types::CLIENT_DEVICE_INFO_()]                      = incoming_packets_types::ClientDeviceInfo;
		result[incoming_packets_types::BOOL_VALUE_()]                              = incoming_packets_types::BoolValue;
		result[incoming_packets_types::BOOL_VALUE_INFO_AUTOMATIC_UPDATE_()]        = incoming_packets_types::BoolValueInfoAutoUpdate;
		result[incoming_packets_types::INT_VALUE_()]                               = incoming_packets_types::IntValue;
		result[incoming_packets_types::INT_VALUE_INFO_AUTOMATIC_UPDATE_()]         = incoming_packets_types::IntValueInfoAutoUpdate;
		result[incoming_packets_types::FLOAT_POINT_VALUE_()]                       = incoming_packets_types::FloatPointValue;
		result[incoming_packets_types::FLOAT_POINT_VALUE_INFO_AUTOMATIC_UPDATE_()] = incoming_packets_types::FloatPointValueInfoAutoUpdate;
		result[incoming_packets_types::TEXT_VALUE_()]                              = incoming_packets_types::TextValue;
		result[incoming_packets_types::TEXT_VALUE_INFO_AUTOMATIC_UPDATE_()]        = incoming_packets_types::TextValueInfoAutoUpdate;
		result[incoming_packets_types::LAYOUT_REFRESHED_()]                        = incoming_packets_types::LayoutRefreshed;
		result[incoming_packets_types::LAYOUT_PAGE_SWITCHED_()]                    = incoming_packets_types::LayoutPageSwitched;
		result[incoming_packets_types::SERVER_REQUEST_PROCESSING_ERRROR_()]        = incoming_packets_types::ServerRequestProcessingError;
		return result;
	}
}

LINKAGE_RESTRICTION incoming_packets_types::TypeID incoming_packets_types::getTypeID(std::string const & packet_type_string) {
	static RegistryType registry = createTypeidRegistry();
	RegistryType::const_iterator iter = registry.find(packet_type_string);
	if (iter != registry.end()) {
		return iter->second;
	}
	return UnknownPacketType;
}

}// namespace communications_handling
}// namespace tau

#undef LINKAGE_RESTRICTION 