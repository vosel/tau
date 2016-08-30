// This source file is part of the 'tau' open source project.
// Copyright (c) 2016, Yuriy Vosel.
// Licensed under Boost Software License.
// See LICENSE.txt for the licence information.

#include "simple_json_builder.h"
#include <stdexcept>
#ifdef TAU_HEADERONLY
#define LINKAGE_RESTRICTION inline
#else
#define LINKAGE_RESTRICTION 
#endif

namespace tau {
namespace layout_generation {

LINKAGE_RESTRICTION simple_json_builder_object_scoped_guard::simple_json_builder_object_scoped_guard(simple_json_builder & generator):guarded(generator)
{
	guarded.startObject();
};

LINKAGE_RESTRICTION simple_json_builder_object_scoped_guard::~simple_json_builder_object_scoped_guard()
{
	guarded.endObject();
};
LINKAGE_RESTRICTION simple_json_builder_array_scoped_guard::simple_json_builder_array_scoped_guard(simple_json_builder & generator):guarded(generator)
{
	guarded.startArray();
};
LINKAGE_RESTRICTION simple_json_builder_array_scoped_guard::~simple_json_builder_array_scoped_guard()
{
	guarded.endArray();
};

LINKAGE_RESTRICTION simple_json_builder::simple_json_builder():is_at_start_of_block(true)
{}

LINKAGE_RESTRICTION void simple_json_builder::addCommaIfNeeded()
{
	if (!is_at_start_of_block) {
		result << ',';
	}
	is_at_start_of_block = false;
}

LINKAGE_RESTRICTION void simple_json_builder::preAppendValue()
{
	if ((!nestedJsonObjects.empty()) && (nestedJsonObjects.top() == InsideArray)) {
		addCommaIfNeeded();
	}
}

LINKAGE_RESTRICTION simple_json_builder & simple_json_builder::startObject()
{
	preAppendValue();	
	result << '{';
	nestedJsonObjects.push(InsideObject);
	is_at_start_of_block = true;
	return *this;
}

LINKAGE_RESTRICTION simple_json_builder_array_scoped_guard simple_json_builder::getArrayScopedGuard()
{
	return simple_json_builder_array_scoped_guard(*this);
}

LINKAGE_RESTRICTION simple_json_builder_object_scoped_guard simple_json_builder::getObjectScopedGuard()
{
	return simple_json_builder_object_scoped_guard(*this);
}

LINKAGE_RESTRICTION simple_json_builder & simple_json_builder::endObject()
{
	result << '}';
	if (nestedJsonObjects.top() != InsideObject) {
		throw std::runtime_error("JsonGenerator trying to close an object at unexpected moment (probably, the order of closing operations is mixed up).");
	}
	nestedJsonObjects.pop();
	is_at_start_of_block = false;
	return *this;
}

LINKAGE_RESTRICTION simple_json_builder & simple_json_builder::startArray()
{
	preAppendValue();
	result << '[';
	nestedJsonObjects.push(InsideArray);
	is_at_start_of_block = true;
	return *this;
}

LINKAGE_RESTRICTION simple_json_builder & simple_json_builder::endArray()
{
	result << ']';
	if (nestedJsonObjects.top() != InsideArray) {
		throw std::runtime_error("JsonGenerator trying to close an array at unexpected moment (probably, the order of closing operations is mixed up).");
	}
	nestedJsonObjects.pop();
	is_at_start_of_block = false;
	return *this;
}

LINKAGE_RESTRICTION simple_json_builder & simple_json_builder::name(std::string const & name)
{
	if (nestedJsonObjects.top() == InsideObject) {
		addCommaIfNeeded();
	} else {
		throw std::runtime_error("JsonGenerator error: trying to add field to an array object");
	}
	result << '\"' << name << '\"' << ':';
	return *this;
}

LINKAGE_RESTRICTION simple_json_builder & simple_json_builder::pushDoubleValue(double value)
{
	preAppendValue();
	result << value;
	return *this;
}

LINKAGE_RESTRICTION simple_json_builder & simple_json_builder::pushIntValue(int value)
{
	preAppendValue();
	result << value;
	return *this;
}

LINKAGE_RESTRICTION simple_json_builder & simple_json_builder::pushBoolValue(bool value)
{
	preAppendValue();
	result << (value ? "true" : "false");
	return *this;
}

LINKAGE_RESTRICTION simple_json_builder & simple_json_builder::pushStringValue(std::string const & value)
{
	preAppendValue();
	result << '\"' << value << '\"';
	return *this;
}

LINKAGE_RESTRICTION std::string simple_json_builder::getResult()
{
	return result.str();
}
} // namespace layout_generation
} // namespace tau

#undef LINKAGE_RESTRICTION 
