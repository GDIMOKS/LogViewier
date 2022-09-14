#pragma once
#ifndef FEATURES_H
#define FEATURES_H

#include <vector>
#include <algorithm>

using namespace std;

class Features
{
private:
    double minValue;
    double maxValue;
    double mean;
    double meanSq;
    double rootMeanSq;
    double median;
    double medianAD;
    double variance;
    double stDeviation;
    double pSkewness;
    double kyrtosys;
    double skewness;

public:
    Features();
    Features(vector<double>& values);

    double getMinValue();
    double getMaxValue();
    double getMean();
    double getMeanSq();
    double getRootMSq();
    double getMedian();
    double getMedianAD();
    double getVariance();
    double getStDeviation();
    double getPSkewness();
    double getKyrtosis();
    double getSkewness();

    double CalcSum(vector<double>& values);

    double CalcSquareSum(vector<double>& values);

    double CalcMin(vector<double>& values);

    double CalcMax(vector<double>& values);

    double CalcMedian(vector<double>& values);

    double CalcMedianAD(vector<double>& values, double median);

    double CalcMean(vector<double>& values);

    double CalcSqMean(vector<double>& values);

    double CalcVariance(vector<double>& values, double mean);

    double CalcPSkewness(double mean, double median, double variance);

    double CalcKyrtosys(vector<double>& values, double mean, double variance);

    double CalcSkewness(vector<double>& values, double mean, double variance);

    void PrintFeatures();


};

#endif