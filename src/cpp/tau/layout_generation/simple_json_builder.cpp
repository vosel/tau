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

LINKAGE_RESTRICTION void simple_json_builder::assertNoError() const
{
	if (errorMessage.size() > 0) {
		throw std::runtime_error(errorMessage);
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
		//Note we can't throw an exception here, because this method is called from the destructor of the guard object (see above). Throwing from destructors should be avoided, so we are just storing an error here and will throw it later.
		errorMessage = "JsonGenerator trying to close an object at unexpected moment (probably, the order of closing operations is mixed up).";
	}
	nestedJsonObjects.pop();
	is_at_start_of_block = false;
	return *this;
}

LINKAGE_RESTRICTION simple_json_builder & simple_json_builder::startArray()
{
	assertNoError();
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
		//Note we can't throw an exception here, because this method is called from the destructor of the guard object (see above). Throwing from destructors should be avoided, so we are just storing an error here and will throw it later.
		errorMessage = "JsonGenerator trying to close an array at unexpected moment (probably, the order of closing operations is mixed up).";
	}
	nestedJsonObjects.pop();
	is_at_start_of_block = false;
	return *this;
}

LINKAGE_RESTRICTION simple_json_builder & simple_json_builder::name(std::string const & name)
{
	assertNoError();
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
	assertNoError();
	preAppendValue();
	result << value;
	return *this;
}

LINKAGE_RESTRICTION simple_json_builder & simple_json_builder::pushIntValue(int value)
{
	assertNoError();
	preAppendValue();
	result << value;
	return *this;
}

LINKAGE_RESTRICTION simple_json_builder & simple_json_builder::pushBoolValue(bool value)
{
	assertNoError();
	preAppendValue();
	result << (value ? "true" : "false");
	return *this;
}

LINKAGE_RESTRICTION simple_json_builder & simple_json_builder::pushStringValue(std::string const & value)
{
	assertNoError();
	preAppendValue();
	result << '\"' << value << '\"';
	return *this;
}

LINKAGE_RESTRICTION std::string simple_json_builder::getResult()
{
	assertNoError();
	return result.str();
}
} // namespace layout_generation
} // namespace tau

#undef LINKAGE_RESTRICTION 
