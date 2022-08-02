#pragma once
#ifndef FRAME_H
#define FRAME_H

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

	void MakeParam(string, string);

	void PrintParams();

	string SearchParam(string);// string);

	string GetAddress(string, int);
};

int CountCorrect(vector<Frame>& frames);

void PrintFrames(vector<Frame>& frames);

void ChoiceParam(Frame& frame, pair<string, string>&);

void PrintStatistics(vector<Frame>& frames, int countNoAddress);


#endif //FRAME_H