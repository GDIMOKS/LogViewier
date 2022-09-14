#pragma once

#ifndef DEVICE_H
#define DEVICE_H

#include <iostream>
#include <vector>
#include <iomanip>

#include "Frame.h"
#include "Features.h"

class Device {
private:
    std::string mac;
    Features sizeFeatures;
    Features timeFeatures;
public:
    vector<Frame> frames;
    
    std::string getMac();
    void setMac(std::string value);

    Features getSizeFeatures();
    Features getTimeFeatures();
    void CalculateFeatures();
    
    void PrintFeatures();

};

#endif