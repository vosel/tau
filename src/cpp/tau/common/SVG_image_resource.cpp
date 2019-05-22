// This source file is part of the 'tau' open source project.
// Copyright (c) 2016, Yuriy Vosel.
// Licensed under Boost Software License.
// See LICENSE.txt for the licence information.


#ifdef TAU_HEADERONLY
#define LINKAGE_RESTRICTION inline
#else
#include "SVG_image_resource.h"
#define LINKAGE_RESTRICTION 
#endif

namespace tau {
namespace common {
    LINKAGE_RESTRICTION SVG_ImageResource::SVG_ImageResource(std::string const & data) : m_textData(data)
    {}

    LINKAGE_RESTRICTION void SVG_ImageResource::serialize(std::ostream & target) const
    {
        target << "SVG\n";
        target << m_textData;
    }
} // namespace common
} // namespace tau
#undef LINKAGE_RESTRICTION 