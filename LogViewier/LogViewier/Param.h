#pragma once
#include <iostream>
using namespace std;
class Param
{
private:
	string paramName;
	string paramValue;
public:
	string getParamName();
	void setParamName(string);

	string getParamValue();
	void setParamValue(string);

	Param(string, string);

	void PrintParam();
};

