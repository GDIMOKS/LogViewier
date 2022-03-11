#include "Param.h"

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
	/*if (paramName == "Frame")
	{
		cout << "\n\t\t" << paramName + "=" + paramValue << "\n\t\t";
	}
	else
	{
		cout << paramName + "=" + paramValue << ", ";
	}*/
}
