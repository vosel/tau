// This source file is part of the 'tau' open source project.
// Copyright (c) 2016, Yuriy Vosel.
// Licensed under Boost Software License.
// See LICENSE.txt for the licence information.

#ifndef TAU_HEADERONLY
#include "layout_info.h"
#endif

#include "json_field_names.h"
#include <sstream>

#ifdef TAU_HEADERONLY
#define LINKAGE_RESTRICTION inline
#else
#define LINKAGE_RESTRICTION 
#endif
namespace tau {
namespace layout_generation {

LINKAGE_RESTRICTION void LayoutPage::dumpToJson(simple_json_builder & target) const
{
	simple_json_builder_object_scoped_guard guard = target.getObjectScopedGuard();
	
	target.name(JsonFieldNames::Layout::LAYOUT_ID_()).pushStringValue(m_id);
	if (m_height > 0) {
		target.name(JsonFieldNames::Layout::HEIGHT_()).pushIntValue(m_height);
		
	}
	target.name(JsonFieldNames::Layout::LAYOUT_DATA_());
	{
		simple_json_builder_object_scoped_guard guard = target.getObjectScopedGuard();
		m_rootLayoutElement.get()->getDeclarationDump(target);
	}	
}

LINKAGE_RESTRICTION LayoutPage & LayoutPage::height(int height)
{
	m_height = height;
	return *this;
}

LINKAGE_RESTRICTION LayoutInfo::LayoutInfo():m_height(-1), m_startLayoutPageID(""), m_wholeLayoutID("")
{}

LINKAGE_RESTRICTION LayoutInfo & LayoutInfo::setTitle(std::string const & title)
{
	m_title = title;
	return *this;
}

LINKAGE_RESTRICTION LayoutInfo & LayoutInfo::setStartLayoutPage(common::LayoutPageID const & layoutPageID)
{
	m_startLayoutPageID = layoutPageID;
	return *this;
}

LINKAGE_RESTRICTION LayoutInfo & LayoutInfo::setHeight(int height)
{
	m_height = height;
	return *this;
}

LINKAGE_RESTRICTION LayoutInfo & LayoutInfo::setID(common::LayoutID const & id)
{
	m_wholeLayoutID = id;
	return *this;
}

LINKAGE_RESTRICTION LayoutInfo & LayoutInfo::pushLayoutPage(const LayoutPage & infoToPush)
{
	m_elements.push_back(infoToPush);
	return *this;
}

LINKAGE_RESTRICTION std::string LayoutInfo::getJson() const
{
	simple_json_builder generator;
	{
		simple_json_builder_object_scoped_guard guard(generator);
		generator.name(JsonFieldNames::VERSION_()).pushIntValue(3);
		if (common::getStringRef(m_startLayoutPageID).size() > 0) {
			generator.name(JsonFieldNames::STARTING_LAYOUT_()).pushStringValue(m_startLayoutPageID);
		}	
		if (m_height > 0) {
			generator.name(JsonFieldNames::HEIGHT_()).pushIntValue(m_height);
		}
		if (m_title.size() > 0) {
			generator.name(JsonFieldNames::TITLE_()).pushStringValue(m_title);
		}
		if (common::getStringRef(m_wholeLayoutID).size() > 0) {
			generator.name(JsonFieldNames::ID_()).pushStringValue(m_wholeLayoutID);
		}		
		generator.name(JsonFieldNames::LAYOUTS_ARRAY_());
		{
			simple_json_builder_array_scoped_guard guard(generator);
			for (size_t i = 0; i < m_elements.size(); ++i) {
				m_elements[i].dumpToJson(generator);
			}
		}
	}
	return generator.getResult();
}
}; // namespace tau::layout_generation
}; // namespace tau

#undef LINKAGE_RESTRICTION