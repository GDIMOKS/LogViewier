#include <iostream>

#include "Features.h"

Features::Features(vector<double>& values) {
    minValue = CalcMin(values);
    maxValue = CalcMax(values);
    mean = CalcMean(values);
    meanSq = CalcMean(values);
    rootMeanSq = sqrt(meanSq);
    median = CalcMedian(values);
    medianAD = CalcMedianAD(values, median);
    variance = CalcVariance(values, mean);
    stDeviation = sqrt(variance);
    pSkewness = CalcPSkewness(mean, median, variance);
    kyrtosys = CalcKyrtosys(values, mean, variance);
    skewness = CalcSkewness(values, mean, variance);
}

double Features::CalcSum(vector<double>& values)
{
    double sum = 0;

    for (double v : values)
        sum += v;

    return sum;
}

double Features::CalcSquareSum(vector<double>& values)
{
    double sum = 0;

    for (double v : values)
        sum += pow(v, 2);

    return sum;
}

double Features::CalcMin(vector<double>& values)
{
    double min = DBL_DIG;

    for (double v : values)
        if (v < min)
            min = v;

    return min;
}

double Features::CalcMax(vector<double>& values)
{
    double max = 0;

    for (double v : values)
        if (v > max)
            max = v;

    return max;
}

double Features::CalcMedian(vector<double>& values)
{
    sort(values.begin(), values.end());

    return values[ceil((values.size() + 1) / 2) - 1];
}

double Features::CalcMedianAD(vector<double>& values, double median)
{
    vector<double> valuesAD;

    for (double v : values)
        valuesAD.push_back(abs(v - median));

    sort(valuesAD.begin(), valuesAD.end());

    return valuesAD[ceil((values.size() + 1) / 2) - 1];
}

double Features::CalcMean(vector<double>& values)
{
    double sum = CalcSum(values);
    return (double)sum / values.size();
}

double Features::CalcSqMean(vector<double>& values)
{
    double sum = CalcSquareSum(values);
    return (double)sum / values.size();
}

double Features::CalcVariance(vector<double>& values, double mean)
{
    double tempSum = 0;
    for (double v : values)
    {
        tempSum += (double)pow((v - mean), 2);
    }

    return tempSum / (values.size() - 1);;
}

double Features::CalcPSkewness(double mean, double median, double variance)
{
    return (3 * ((double)mean - median) / variance);
}

double Features::CalcKyrtosys(vector<double>& values, double mean, double variance)
{
    double tempSum = 0;
    for (double v : values)
    {
        tempSum += pow((v - mean) / variance, 4);
    }

    return tempSum / values.size();
}

double Features::CalcSkewness(vector<double>& values, double mean, double variance)
{
    double tempSum = 0;
    for (double v : values)
    {
        tempSum += pow((v - mean) / variance, 3);
    }

    return tempSum / values.size();
}

void Features::PrintFeatures()
{
    cout << "standard deviation: " << stDeviation << endl;
    cout << "variance: " << variance << endl;
    cout << "root mean square: " << rootMeanSq << endl;
    cout << "m_square: " << meanSq << endl;
    cout << "p_skewness: " << pSkewness << endl;
    cout << "kyrtosys: " << kyrtosys << endl;
    cout << "skewness: " << skewness << endl;
    cout << "min: " << minValue << "\nmax: " << maxValue << endl;
    cout << "mean: " << mean << endl;
    cout << "median: " << median << endl;
    cout << "medianAD: " << medianAD << endl;

    cout << endl;
}

void CalculateFeatures(vector<Frame>& frames)
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

    Features sizeFeatures = Features(frameSizes);
    Features timeFeatures = Features(frameOffsets);

    cout << "\nSizes:" << endl;
    sizeFeatures.PrintFeatures();
    cout << "\nOffsets:" << endl;
    timeFeatures.PrintFeatures();

}
