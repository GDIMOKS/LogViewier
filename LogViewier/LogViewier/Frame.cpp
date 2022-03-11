#include "Frame.h"

string Frame::getFrameName() { return frameName; }
void Frame::setFrameName(string value) { frameName = value; }

string Frame::getOffset() { return offset; }
void Frame::setOffset(string value) { offset = value; }

string Frame::getBW() { return bw; }
void Frame::setBW(string value) { bw = value; }

string Frame::getMCS() { return mcs; }
void Frame::setMCS(string value) { mcs = value; }

string Frame::getSize() { return size; }
void Frame::setSize(string value) { size = value; }

string Frame::getFrameHex() { return frameHex; }
void Frame::setFrameHex(string value) { frameHex = value; }

bool Frame::getCorrect() { return correct; }
void Frame::setCorrect(bool value) { correct = value; }

//list<Param> Frame::getParams() { return params; }
//void Frame::setParams(Param param) { params.push_back(param); }

void Frame::MakeParam(string name, string value)
{
    Param param{ name, value };

    params.push_back(param);
}

void Frame::PrintParams()
{
    Param p1 = params.back();

    for (Param p : params)
    {
        p.PrintParam();
        if (p.getParamName() != p1.getParamName() && p.getParamValue() != p1.getParamValue())
        {
            cout << ", ";
        }
        /*
        if (p.getParamName() == "Frame")
        {
            cout << "\n\t\t";
            p.PrintParam();
            cout << "\t\t";
        }
        else
        {
            p.PrintParam();
            cout << ", ";
        }*/
    }
}

void Frame::SearchParam()
{
    for (Param p : params)
    {
        if (p.getParamName() == "FCS" && p.getParamValue() == "Fail")
        {
            correct = false;
        }
    }
}