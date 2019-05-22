// This source file is part of the 'tau' open source project.
// Copyright (c) 2016, Yuriy Vosel.
// Licensed under Boost Software License.
// See LICENSE.txt for the licence information.


#pragma once
#include "image_resource.h"
#include <string>

namespace tau {
namespace common {

class SVG_ImageResource:
    public ImageResource
{
private:
    std::string m_textData;
public:
	SVG_ImageResource(std::string const & data);
    virtual void serialize(std::ostream & target) const;
};

} // namespace common
} // namespace tau

#ifdef TAU_HEADERONLY
#include "SVG_image_resource.cpp"
#endif