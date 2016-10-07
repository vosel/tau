// This source file is part of the 'tau' open source project.
// Copyright (c) 2016, Yuriy Vosel.
// Licensed under Boost Software License.
// See LICENSE.txt for the licence information.


#ifdef TAU_HEADERONLY
#define LINKAGE_RESTRICTION inline
#else
#include "ARGB_image_resource.h"
#define LINKAGE_RESTRICTION 
#endif

namespace tau {
namespace common {
    LINKAGE_RESTRICTION ARGB_point::ARGB_point()
        :a(255), r(255), g(128), b(0)
    {}

    LINKAGE_RESTRICTION ARGB_point::ARGB_point(unsigned char A, unsigned char R, unsigned char G, unsigned char B)
        :a(A), r(R), g(G), b(B)
    {}
    LINKAGE_RESTRICTION ARGB_point::ARGB_point(unsigned int argb)
    {
		this->operator =(argb);
	}

    LINKAGE_RESTRICTION void ARGB_point::serialize(std::ostream & target) const
    {
        target.write((const char *)(&(this->a)), 1);
        target.write((const char *)(&(this->r)), 1);
        target.write((const char *)(&(this->g)), 1);
        target.write((const char *)(&(this->b)), 1);
    }

    LINKAGE_RESTRICTION bool ARGB_point::operator == (ARGB_point const & other) const
    {
        return (a == other.a) && (r == other.r) && (g == other.g) && (b == other.b);
    }

    LINKAGE_RESTRICTION void ARGB_point::operator = (ARGB_point const & other)
    {
        a = other.a;
        r = other.r;
        g = other.g;
        b = other.b;
    }

	LINKAGE_RESTRICTION void ARGB_point::operator = (unsigned int argb)
    {
        a = (0xff000000 & argb) >> 24;
        r = (0xff0000 & argb) >> 16;
        g = (0xff00 & argb) >> 8;
        b = 0xff & argb;
    }

    LINKAGE_RESTRICTION ARGB_ImageResource::ARGB_ImageResource(size_t width, size_t height)
        : m_width(width), m_height(height)
    {
        for (size_t i = 0; i < m_width; ++i) {
            m_bitmap.push_back(std::vector<ARGB_point>());
            for (size_t j = 0; j < m_height; ++j) {
                m_bitmap.back().push_back(ARGB_point());
            }
        }
    }
   
	LINKAGE_RESTRICTION ARGB_point & ARGB_ImageResource::at(size_t x, size_t y)
    {
    	return m_bitmap[x][y];
    }
	
	LINKAGE_RESTRICTION ARGB_ImageResource::RowOfPoints & ARGB_ImageResource::operator[](size_t x)
	{
		return m_bitmap[x];
	}
	
    LINKAGE_RESTRICTION void ARGB_ImageResource::serialize(std::ostream & target) const
    {
        target << "ARGB\n" << m_width << "x" << m_height << "\n";
        for (size_t i = 0; i < m_bitmap.size(); ++i) {
            for (size_t j = 0; j < m_bitmap[i].size(); ++j) {
                m_bitmap[i][j].serialize(target);
            }
        }
    }
} // namespace common
} // namespace tau
#undef LINKAGE_RESTRICTION 