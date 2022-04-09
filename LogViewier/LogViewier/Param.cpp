#include "Param.h"

Param::Param(){}

string Param::getParamName() { return paramName; }
void Param::setParamName(string name) { paramName = name; }

string Param::getParamValue() { return paramValue; }
void Param::setParamValue(string value) { paramValue = value; }

Param::Param(string name = "", string value = "")
{
	paramName = name;
	paramValue = value;
}

void Param::PrintParam()
{
	cout << paramName + "=" + paramValue;
}
