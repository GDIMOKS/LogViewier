#pragma once

#ifndef DEVICE_H
#define DEVICE_H

#include <iostream>
#include <vector>

#include "Frame.h"

class Device {
private:
    std::string mac;
public:
    std::string getMac();
    void setMac(std::string value);

    vector<Frame> frames;
};

#endif