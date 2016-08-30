// This source file is part of the 'tau' open source project.
// Copyright (c) 2016, Yuriy Vosel.
// Licensed under Boost Software License.
// See LICENSE.txt for the licence information.

#ifndef TAU_HEADERONLY
#include "json_field_names.h"
#endif

#include <sstream>

#ifdef TAU_HEADERONLY
#define LINKAGE_RESTRICTION inline
#else
#define LINKAGE_RESTRICTION 
#endif

namespace tau {
namespace layout_generation {

namespace {
	// Small utility function for putting quote symbols around strings.
	std::string q(std::string const & toEnquote)
	{
		std::stringstream str;
		str << "\"" << toEnquote << "\"";
		return str.str().c_str();
	}
}
LINKAGE_RESTRICTION std::string JsonValues::getBooleanValueDeclaration(std::string const & jsonName, bool value)
	{
		std::stringstream result;
		result << q(jsonName) << ':' << (value ? TRUE_() : FALSE_());
		return result.str();
	}
}; // namespace layout_generation
}; // namespace tau

#undef LINKAGE_RESTRICTION