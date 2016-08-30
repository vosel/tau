// This source file is part of the 'tau' open source project.
// Copyright (c) 2016, Yuriy Vosel.
// Licensed under Boost Software License.
// See LICENSE.txt for the licence information.


#pragma once
#include <string>
#include <ostream>

namespace tau {
namespace common {

template <typename T>
class BasicWrapperForID
{    
	std::string m_value;
protected:
	BasicWrapperForID(std::string const & value): m_value(value){};
public:    
    std::string const & getValue() const
    {
        return m_value;
    }
};

template <typename T>
bool operator == (BasicWrapperForID<T> const & left, BasicWrapperForID<T> const & right)
{
	return left.getValue() == right.getValue();
}

template <typename T>
bool operator != (BasicWrapperForID<T> const & left, BasicWrapperForID<T> const & right)
{
	return !(left == right);
}

template <typename T>
bool operator < (BasicWrapperForID<T> const & left, BasicWrapperForID<T> const & right)
{
	return left.getValue() < right.getValue();
}

class LayoutID:
	public BasicWrapperForID<LayoutID>
{
public:
	explicit LayoutID(std::string const & value): BasicWrapperForID(value) {};
};

class LayoutPageID:
	public BasicWrapperForID<LayoutPageID>
{
public:
	explicit LayoutPageID(std::string const & value): BasicWrapperForID(value) {};
};

class UnassignedID:
	public BasicWrapperForID<UnassignedID>
{
public:
	explicit UnassignedID(): BasicWrapperForID("") {};
};

class ElementID:
	public BasicWrapperForID<ElementID>
{
public:
	explicit ElementID(std::string const & value): BasicWrapperForID(value) {};
};

template <typename T>
std::string const & getStringRef(BasicWrapperForID<T> const & idWrapper)
{
	return idWrapper.getValue();
}

template <typename T>
std::ostream & operator << (std::ostream & target, BasicWrapperForID<T> const & toDump)
{
	target << toDump.getValue();
	return target;
}

}
}