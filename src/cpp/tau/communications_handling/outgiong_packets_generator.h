// This source file is part of the 'tau' open source project.
// Copyright (c) 2016, Yuriy Vosel.
// Licensed under Boost Software License.
// See LICENSE.txt for the licence information.

#pragma once

#include "../common/element_id.h"
#include "../common/image_resource.h"
#include <string>
namespace tau {
namespace communications_handling {

class OutgiongPacketsGenerator
{
public:
	virtual void sendPacket_LayoutUpdate(std::string const & layout_json_data);
	virtual void sendPacket_SwitchLayoutPage(common::LayoutPageID const & layout_page_id);
	virtual void sendPacket_SetNote(common::ElementID const & layout_element_id, std::string const & text);
	virtual void sendPacket_SetBooleanValue(common::ElementID const & layout_element_id, bool newValue);
	virtual void sendPacket_SetIntValue(common::ElementID const & layout_element_id, int newValue);
	virtual void sendPacket_SetFloatPointValue(common::ElementID const & layout_element_id, double newValue);
	virtual void sendPacket_SetTextValue(common::ElementID const & layout_element_id, std::string const & newValue);
	virtual void sendPacket_GetValue(common::ElementID const & layout_element_id);
	virtual void sendPacket_SetEnabled(common::ElementID const & layout_element_id, bool isActivated);
	virtual void sendPacket_PutImage(common::ImageID const & imageID, common::ImageResource const & image);

	// Sets a new image ID for one of the elements. This element will use this id to reference the image to be displayed.
	virtual void sendPacket_SetImageForElement(common::ElementID const & elementToUpdate, common::ImageID const & imageID);

	virtual void sendPacket_GetLayoutElementPositionInfo(common::ElementID const & layout_element_id);
	virtual void sendPacket_SubscribeToSensor(size_t sensorIndex, size_t interval);
	virtual void sendPacket_UnsubscribeFromSensor(size_t sensorIndex);
	
    // This method contains the code that actually sends the data to the socket (the way of sending it is defined by the user of the library)
	virtual void sendData(std::string const & data) = 0;
	
	virtual void close_connection() = 0;
#if defined(TAU_CPP_03_COMPATIBILITY)
	virtual ~OutgiongPacketsGenerator() {};
#else
	virtual ~OutgiongPacketsGenerator() = default; 
#endif
};
}
}
#ifdef TAU_HEADERONLY
#include "outgiong_packets_generator.cpp"
#endif