// This source file is part of the 'tau' open source project.
// Copyright (c) 2016, Yuriy Vosel.
// Licensed under Boost Software License.
// See LICENSE.txt for the licence information.


#pragma once
#include <vector>


namespace tau {
namespace common {

struct SensorData
{
    size_t sensorIndex;
    std::vector<double> sensorData;
    std::string timestamp;
};


} //namespace common
} //namespace tau