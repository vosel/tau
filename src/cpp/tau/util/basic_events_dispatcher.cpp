// This source file is part of the 'tau' open source project.
// Copyright (c) 2016, Yuriy Vosel.
// Licensed under Boost Software License.
// See LICENSE.txt for the licence information.

#ifndef TAU_HEADERONLY
#include "basic_events_dispatcher.h"
#endif

#include "../communications_handling/incoming_packets_handler.h"
#include "../communications_handling/communications_issues_handler.h"
#include "../communications_handling/outgiong_packets_generator.h"

#ifdef TAU_HEADERONLY
#define LINKAGE_RESTRICTION inline
#else
#define LINKAGE_RESTRICTION 
#endif

#ifdef TAU_CPP_03_COMPATIBILITY
#define OVERRIDE_IDENTIFIER 
#else
#define OVERRIDE_IDENTIFIER override
#endif

namespace tau {
namespace util {

namespace {
	class BasicIncomingPacketsHandler : 
		public tau::communications_handling::IncomingPacketsHandler
	{
		BasicEventsDispatcher * m_owner;
	public:
		BasicIncomingPacketsHandler(BasicEventsDispatcher *owner) : m_owner(owner) {};
		void onPacketReceived_ButtonClick(common::ElementID const & buttonID) OVERRIDE_IDENTIFIER {
			m_owner->packetReceived_buttonClick(buttonID);
		}		
		void onPacketReceived_ClientDeviceInfo(tau::communications_handling::ClientDeviceInfo const & info) OVERRIDE_IDENTIFIER {
			m_owner->packetReceived_clientDeviceInfo(info);
		}
		void onPacketReceived_BoolValue(common::ElementID const & inputBoxID, bool value) OVERRIDE_IDENTIFIER {
			m_owner->packetReceived_boolValueUpdate(inputBoxID, value, false);
		}
		void onPacketReceived_BoolValueInfoAutoUpdate(common::ElementID const & inputBoxID, bool value) OVERRIDE_IDENTIFIER {
			m_owner->packetReceived_boolValueUpdate(inputBoxID, value, true);
		}
		void onPacketReceived_IntValue(common::ElementID const & inputBoxID, int value) OVERRIDE_IDENTIFIER {
			m_owner->packetReceived_intValueUpdate(inputBoxID, value, false);
		}
		void onPacketReceived_IntValueInfoAutoUpdate(common::ElementID const & inputBoxID, int value) OVERRIDE_IDENTIFIER {
			m_owner->packetReceived_intValueUpdate(inputBoxID, value, true);
		}
		void onPacketReceived_FloatPointValue(common::ElementID const & inputBoxID, double value) OVERRIDE_IDENTIFIER {
			m_owner->packetReceived_floatPointValueUpdate(inputBoxID, value, false);
		}
		void onPacketReceived_FloatPointValueInfoAutoUpdate(common::ElementID const & inputBoxID, double value) OVERRIDE_IDENTIFIER {
			m_owner->packetReceived_floatPointValueUpdate(inputBoxID, value, true);
		}
		void onPacketReceived_TextValue(common::ElementID const & inputBoxID, std::string const & value) OVERRIDE_IDENTIFIER {
			m_owner->packetReceived_textValueUpdate(inputBoxID, value, false);
		}
		void onPacketReceived_TextValueInfoAutoUpdate(common::ElementID const & inputBoxID, std::string const & value) OVERRIDE_IDENTIFIER {
			m_owner->packetReceived_textValueUpdate(inputBoxID, value, true);
		}
		void onPacketReceived_LayoutPageSwitched(common::LayoutPageID const & newActiveLayoutPageID) OVERRIDE_IDENTIFIER {
			m_owner->packetReceived_layoutPageSwitched(newActiveLayoutPageID);
		}
		void onPacketReceived_LayoutRefreshed(common::LayoutID const & layoutID) OVERRIDE_IDENTIFIER {
			m_owner->packetReceived_layoutRefreshed(layoutID);
		}
		void onPacketReceived_ImageUpdated(common::ImageID const & imageID) {
			m_owner->packetReceived_imageUpdated(imageID);
		}
		void onPacketReceived_LayoutElementPosition(common::ElementID const & elementID, size_t x, size_t y, size_t width, size_t height) {
			m_owner->packetReceived_layoutElementPosition(elementID, x, y, width, height);
		}
		void onPacketReceived_ServerRequestProcessingError(std::string const & layoutID, std::string const & additionalData) OVERRIDE_IDENTIFIER {
			m_owner->packetReceived_requestProcessingError(layoutID, additionalData);
		}
		void unknownErrorOccured(std::string const & errorMessage) OVERRIDE_IDENTIFIER {
			m_owner->unknownErrorOccured(errorMessage);
		}
	};
	
	class BasicCommunicationIssuesHandler : 
		public tau::communications_handling::CommunicationIssuesHandler
	{
		BasicEventsDispatcher * m_owner;
	public: 
		BasicCommunicationIssuesHandler(BasicEventsDispatcher *owner) : m_owner(owner) {};
		void onCommunicationError(std::string const & errorDescription) OVERRIDE_IDENTIFIER {
			m_owner->communicationErrorOccured(errorDescription);
		};
	};
}

LINKAGE_RESTRICTION BasicEventsDispatcher::BasicEventsDispatcher(
	tau::communications_handling::OutgiongPacketsGenerator & outgoingPacketsGeneratorToUse)
	:m_outgiongPacketsGenerator(outgoingPacketsGeneratorToUse)
{
	m_incomingPacketsHandler = tau::layout_generation::my_shared_ptr<BasicIncomingPacketsHandler>::createInstance(this);
	m_communicationIssuesHandler = tau::layout_generation::my_shared_ptr<BasicCommunicationIssuesHandler>::createInstance(this);
}
LINKAGE_RESTRICTION tau::communications_handling::IncomingPacketsHandler * BasicEventsDispatcher::getIncomingPacketsHandler()
{
	return m_incomingPacketsHandler.get();
}
LINKAGE_RESTRICTION tau::communications_handling::CommunicationIssuesHandler * BasicEventsDispatcher::getCommunicationIssuesHandler()
{
	return m_communicationIssuesHandler.get();
}

LINKAGE_RESTRICTION void BasicEventsDispatcher::sendPacket_resetLayout(std::string const & layoutJson)
{
	m_outgiongPacketsGenerator.sendPacket_LayoutUpdate(layoutJson);
}

LINKAGE_RESTRICTION void BasicEventsDispatcher::sendPacket_requestValue(common::ElementID const & inputID)
{
	m_outgiongPacketsGenerator.sendPacket_GetValue(inputID);
}

LINKAGE_RESTRICTION void BasicEventsDispatcher::sendPacket_updateBooleanValue(common::ElementID const & inputID, bool newValue)
{
	m_outgiongPacketsGenerator.sendPacket_SetBooleanValue(inputID, newValue);
}

LINKAGE_RESTRICTION void BasicEventsDispatcher::sendPacket_updateIntValue(common::ElementID const & inputID, int newValue)
{
	m_outgiongPacketsGenerator.sendPacket_SetIntValue(inputID, newValue);
}

LINKAGE_RESTRICTION void BasicEventsDispatcher::sendPacket_updateFloatPointValue(common::ElementID const & inputID, double newValue)
{
	m_outgiongPacketsGenerator.sendPacket_SetFloatPointValue(inputID, newValue);
}

LINKAGE_RESTRICTION void BasicEventsDispatcher::sendPacket_updateTextValue(common::ElementID const & inputID, std::string const & newValue)
{
	m_outgiongPacketsGenerator.sendPacket_SetTextValue(inputID, newValue);
}

LINKAGE_RESTRICTION void BasicEventsDispatcher::sendPacket_changeElementNote(common::ElementID const & elementID, std::string const & note)
{
	m_outgiongPacketsGenerator.sendPacket_SetNote(elementID, note);
}

LINKAGE_RESTRICTION void BasicEventsDispatcher::sendPacket_changeShownLayoutPage(common::LayoutPageID const & layoutPageIdToSwitchTo)
{
	m_outgiongPacketsGenerator.sendPacket_SwitchLayoutPage(layoutPageIdToSwitchTo);
}	

LINKAGE_RESTRICTION void BasicEventsDispatcher::sendPacket_changeElementEnabledState(common::ElementID const & elementID, bool newState)
{
	m_outgiongPacketsGenerator.sendPacket_SetEnabled(elementID, newState);
}

LINKAGE_RESTRICTION void BasicEventsDispatcher::sendPacket_getLayoutElementPositionInfo(common::ElementID const & elementID)
{
	m_outgiongPacketsGenerator.sendPacket_GetLayoutElementPositionInfo(elementID);
}

LINKAGE_RESTRICTION void BasicEventsDispatcher::sendPacket_putImage(common::ImageID const & imageID, common::ImageResource const & image)
{
	m_outgiongPacketsGenerator.sendPacket_PutImage(imageID, image);
}

} //namespace util
} //namespace tau

#undef LINKAGE_RESTRICTION 
