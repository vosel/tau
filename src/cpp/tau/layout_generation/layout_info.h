// This source file is part of the 'tau' open source project.
// Copyright (c) 2016, Yuriy Vosel.
// Licensed under Boost Software License.
// See LICENSE.txt for the licence information.

#pragma once

#include "layout_elements.h"
#include <algorithm>
namespace tau {
namespace layout_generation {

struct LayoutPage
{
	common::LayoutPageID m_id;
	my_shared_ptr<AbstractLayoutElement>::MyInternalType m_rootLayoutElement;
	int m_height;

	template <typename T>
	LayoutPage(common::LayoutPageID const & id, const T & rootRectRegion): 
		m_id(id), m_rootLayoutElement(my_shared_ptr<T>::createInstance(rootRectRegion)), m_height(-1)
	{}
	LayoutPage & height(int height);
	void dumpToJson(simple_json_builder & target) const;
};

class LayoutInfo
{
private:
	common::LayoutPageID m_startLayoutPageID;
	int m_height;
	std::string m_title;
	common::LayoutID m_wholeLayoutID;
	std::vector<LayoutPage> m_elements;
public:
	LayoutInfo();
	LayoutInfo & pushLayoutPage(const LayoutPage & infoToPush);
	LayoutInfo & setTitle(std::string const & );
	LayoutInfo & setStartLayoutPage(common::LayoutPageID const & layoutPageId);
	LayoutInfo & setHeight(int height);
	LayoutInfo & setID(common::LayoutID const & id);
	std::string getJson() const;
};

}; // namespace tau::layout_generation 
}; // namespace tau
#ifdef TAU_HEADERONLY
#include "layout_info.cpp"
#endif// TAU_HEADERONLY
