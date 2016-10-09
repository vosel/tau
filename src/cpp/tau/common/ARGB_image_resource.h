// This source file is part of the 'tau' open source project.
// Copyright (c) 2016, Yuriy Vosel.
// Licensed under Boost Software License.
// See LICENSE.txt for the licence information.


#pragma once
#include "image_resource.h"
#include <ostream>
#include <vector>

namespace tau {
namespace common {

struct ARGB_point
{
    unsigned char a;
    unsigned char r;
    unsigned char g;
    unsigned char b;
    ARGB_point();
    ARGB_point(unsigned char A, unsigned char R, unsigned char G, unsigned char B);
    ARGB_point(unsigned int argb);
    void serialize(std::ostream & target) const;
    bool operator == (ARGB_point const & other) const;
    void operator = (ARGB_point const & other);
	void operator = (unsigned int argb);
};

class ARGB_ImageResource:
    public ImageResource
{
public:
typedef std::vector<ARGB_point> RowOfPoints;
private:
    std::vector<std::vector<ARGB_point> > m_bitmap;
public:
    size_t const m_width;
    size_t const m_height;
	ARGB_ImageResource(size_t width, size_t height);
    virtual void serialize(std::ostream & target) const;
	ARGB_point & at(size_t x, size_t y);
	RowOfPoints & operator[](size_t x);
};

} // namespace common
} // namespace tau

#ifdef TAU_HEADERONLY
#include "ARGB_image_resource.cpp"
#endif