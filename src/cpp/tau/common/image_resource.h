// This source file is part of the 'tau' open source project.
// Copyright (c) 2016, Yuriy Vosel.
// Licensed under Boost Software License.
// See LICENSE.txt for the licence information.


#pragma once
#include <ostream>
namespace tau {
namespace common {


// This class is a base for all the image resources that are sent
// to client as a byte stream.
class ImageResource
{
public:
    virtual void serialize(std::ostream & target) const = 0;
};

} //namespace common
} //namespace tau

