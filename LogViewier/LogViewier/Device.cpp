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
            double size = stod(f.getSize());
            double time = stod(f.getOffset());

            frameSizes.push_back(size);
            frameOffsets.push_back(time);
        }
    }

    sizeFeatures = Features(frameSizes);
    timeFeatures = Features(frameOffsets);
}

void Device::PrintFeatures()
{
    
    cout << "Device " << mac << endl;
    cout.width(25);
    cout << setw(30) << right << "Sizes" << setw(30) << "Offsets" << endl;
    cout << left << setw(25) << "standard deviation:" << setw(28) << sizeFeatures.getStDeviation() << timeFeatures.getStDeviation() << endl;
    cout << setw(25) << "variance:" << setw(28) << sizeFeatures.getVariance() << timeFeatures.getVariance() << endl;
    cout << setw(25) << "root mean square:" << setw(28) << sizeFeatures.getRootMSq() << timeFeatures.getRootMSq() << endl;
    cout << setw(25) << "m_square:" << setw(28) << sizeFeatures.getMeanSq() << timeFeatures.getMeanSq() << endl;
    cout << setw(25) << "p_skewness:" << setw(28) << sizeFeatures.getPSkewness() << timeFeatures.getPSkewness() << endl;
    cout << setw(25) << "kyrtosys:" << setw(28) << sizeFeatures.getKyrtosis() << timeFeatures.getKyrtosis() << endl;
    cout << setw(25) << "skewness:" << setw(28) << sizeFeatures.getSkewness() << timeFeatures.getSkewness() << endl;
    cout << setw(25) << "min:" << setw(28) << sizeFeatures.getMinValue() << timeFeatures.getMinValue() << endl;
    cout << setw(25) << "max:" << setw(28) << sizeFeatures.getMaxValue() << timeFeatures.getMaxValue() << endl;
    cout << setw(25) << "mean:" << setw(28) << sizeFeatures.getMean() << timeFeatures.getMean() << endl;
    cout << setw(25) << "median:" << setw(28) << sizeFeatures.getMedian() << timeFeatures.getMedian() << endl;
    cout << setw(25) << "medianAD:" << setw(28) << sizeFeatures.getMedianAD() << timeFeatures.getMedianAD() << endl;
}