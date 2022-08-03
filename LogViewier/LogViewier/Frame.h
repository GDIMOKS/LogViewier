#pragma once
#ifndef FRAME_H
#define FRAME_H

#include <string.h>
#include <string>
#include <vector>
#include <map>

#include "Param.h"
#include "FormatFunctions.h"

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

	int type = -1;
	int subtype = -1;

	vector<Param> params;

public:
	Frame();

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

	int getType();
	void setType(int = -1);

	int getSubtype();
	void setSubtype(int = -1);

	static map<int, map<int, string>> subtypes;

	void MakeParam(string, string);

	void PrintParams();

	string SearchParam(string);// string);

	map<int, string>::iterator SearchSubtype(pair<int, int>, bool&);

	string GetAddress(string, int);

	void ChoiceParam(pair<string, string>&);
};

int CountCorrect(vector<Frame>& frames);

void PrintFrames(vector<Frame>& frames);


void PrintStatistics(vector<Frame>& frames, int countNoAddress);


#endif //FRAME_H