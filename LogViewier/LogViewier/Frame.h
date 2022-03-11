#pragma once
#include <string.h>
#include <string>
#include <vector>

#include "Param.h"

using namespace std;

class Frame
{
private:
	string frameName;
	string offset;
	string bw;
	string mcs;
	string size;
	string frameHex;
	bool correct = true;
	Param param;

	//list<Param> params;
	vector<Param> params;

public:
	string getFrameName();
	void setFrameName(string);

	string getOffset();
	void setOffset(string);

	string getBW();
	void setBW(string);

	string getMCS();
	void setMCS(string);

	string getSize();
	void setSize(string);

	string getFrameHex();
	void setFrameHex(string);

	bool getCorrect();
	void setCorrect(bool);

	//list<Param> getParams();
	//void setParams(Param);

	void MakeParam(string, string);

	void PrintParams();

	void SearchParam();
};

