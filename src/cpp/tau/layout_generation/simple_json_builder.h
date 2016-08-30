// This source file is part of the 'tau' open source project.
// Copyright (c) 2016, Yuriy Vosel.
// Licensed under Boost Software License.
// See LICENSE.txt for the licence information.

#pragma once

#include <sstream>
#include <stack>
namespace tau {
namespace layout_generation {

class simple_json_builder;

// The simple guard objects that are used to control the handling of the json arrays and objects boundaries automatically.
struct simple_json_builder_object_scoped_guard
{
	simple_json_builder & guarded;
	simple_json_builder_object_scoped_guard(simple_json_builder & generator);
	~simple_json_builder_object_scoped_guard();
};

struct simple_json_builder_array_scoped_guard
{
	simple_json_builder & guarded;
	simple_json_builder_array_scoped_guard(simple_json_builder & generator);
	~simple_json_builder_array_scoped_guard();
};

// This class is used for facilitating the genration of json data for the layout element objects.
class simple_json_builder
{
	enum CursorPosition {InsideObject, InsideArray};
	std::stack<CursorPosition> nestedJsonObjects;

	std::stringstream result;
	bool is_at_start_of_block;

	void addCommaIfNeeded();
	void preAppendValue();
public:
	simple_json_builder();

	simple_json_builder_array_scoped_guard getArrayScopedGuard();
	simple_json_builder & startArray();
	simple_json_builder & endArray();

	simple_json_builder_object_scoped_guard getObjectScopedGuard();
	simple_json_builder & startObject();
	simple_json_builder & endObject();	

	simple_json_builder & name(std::string const & name);

	simple_json_builder & pushDoubleValue(double value);
	simple_json_builder & pushIntValue(int value);
	simple_json_builder & pushBoolValue(bool value);

	template <typename T>
	simple_json_builder & pushStringValue(common::BasicWrapperForID<T> const & idWrapper)
	{
		return pushStringValue(idWrapper.getValue());
	}
	simple_json_builder & pushStringValue(std::string const & value);

	std::string getResult();
};

}; // namespace layout_generation
}; // namespace tau

#ifdef TAU_HEADERONLY

#include "simple_json_builder.cpp"

#endif // TAU_HEADERONLY
