#pragma once

#ifndef RECORD_H
#define RECORD_H

#include <iostream>
#include <vector>
#include <iomanip>

#include "Frame.h"
#include "Features.h"

class Record
{
private:
    vector<double> features;
public:
    vector<double> get_features();

    void set_features(vector<Frame>&);

    void calculate_unique_features(vector<Frame>&);

    void calculate_standard_features(vector<Frame>&);

    void add_standard_features_to_all(Features&);

    void print_standard_features(Features&, Features&);

    void print_unique_features();

    void print_unique_features(float, float, float, int*);
};

#endif

