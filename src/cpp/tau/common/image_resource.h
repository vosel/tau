// This source file is part of the 'tau' open source project.
// Copyright (c) 2016, Yuriy Vosel.
// Licensed under Boost Software License.
// See LICENSE.txt for the licence information.


#pragma once
#include <ostream>
namespace tau {
namespace common {


// This class is a base for all the image resources that are sent
// to client as a byte stream. The format of the data in this 
// stream depends on the type of an image (it may be raw bitmap 
// buffer, text data for svg object, data read from the normal
// image file (png, jpg, bmp, e.t.c.)
class ImageResource
{
public:
    virtual void serialize(std::ostream & target) const = 0;
};

} //namespace common
} //namespace tau

