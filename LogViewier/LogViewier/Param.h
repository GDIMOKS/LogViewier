#pragma once
#ifndef PARAM_H
#define PARAM_H

#include <iostream>
using namespace std;
class Param
{
private:
	string paramName;
	string paramValue;
public:
	Param();

	string getParamName();
	void setParamName(string);

	string getParamValue();
	void setParamValue(string);

	Param(string, string);

	void PrintParam();
};

#endif //PARAM_H
