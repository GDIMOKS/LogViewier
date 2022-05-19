#include "Frame.h"


Frame::Frame()
{
    Param p;
}


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
    }
}

string Frame::SearchParam(string name)//, string value = "")
{
    for (Param p : params)
    {
        if (p.getParamName() == name)
        {
            return p.getParamValue();
        }
    }
    
    return "";
}

string Frame::GetAddress(string frameHex, int position)
{
    string address = "";
    for (int i = 0; i < 12; i++)
    {
        address += frameHex[i + position];

        if ((i + 1) % 2 == 0 && (i + 1) != 12)
        {
            address += ":";
        }
    }

    return address;
}
