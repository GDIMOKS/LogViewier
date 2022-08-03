#include "Device.h"

std::string Device::getMac() { return mac; }
void Device::setMac(std::string value) { mac = value; }

Features Device::getSizeFeatures()
{
    return sizeFeatures;
}

Features Device::getTimeFeatures()
{
    return timeFeatures;
}

void Device::CalculateFeatures()
{
    if (frames.empty())
        return;

    vector<double> frameSizes;
    vector<double> frameOffsets;

    for (Frame f : frames)
    {
        if (f.getCorrect())
        {
            double size = atof(f.getSize().c_str());
            double time = atof(f.getOffset().c_str());

            frameSizes.push_back(size);
            frameOffsets.push_back(time);
        }
    }

    sizeFeatures = Features(frameSizes);
    timeFeatures = Features(frameOffsets);
}