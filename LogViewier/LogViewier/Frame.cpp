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

int CountCorrect(vector<Frame>& frames)
{
    int sum = 0;

    for (Frame f : frames)
    {
        if (f.getCorrect() == true)
        {
            sum++;
        }
    }

    return sum;
}

void PrintFrames(vector<Frame>& frames)
{
    for (Frame f : frames)
    {
        cout << f.getFrameName() << "\tOffset=" << f.getOffset() << ", BW=" << f.getBW() << ", MCS=" << f.getMCS() << ", Size=" << f.getSize() << endl;
        cout << "\t\tFrame=" << f.getFrameHex() << "\n\t\t";
        f.PrintParams();
        cout << "\n\n";
    }
}

void ChoiceParam(Frame& frame, string& paramName, string& paramValue)
{
    if (paramName == "Offset")
    {
        frame.setOffset(paramValue);
        return;
    }
    else if (paramName == "BW")
    {
        frame.setBW(paramValue);
        return;
    }
    else if (paramName == "MCS")
    {
        frame.setMCS(paramValue);
        return;
    }
    else if (paramName == "Size")
    {
        frame.setSize(paramValue);
        return;
    }
    /*else if (paramName == "Frame")
    {
        frame.setFrameHex(paramValue);
        return;
    }*/

    if (paramName == "FCS" && paramValue == "Fail")
    {
        frame.setCorrect(false);
    }

    frame.MakeParam(paramName, paramValue);
}

void PrintStatistics(vector<Frame>& frames, int countNoAddress)
{
    int correctFrames = CountCorrect(frames);
    double correctPercent = correctFrames * 100.0 / frames.size();

    cout << "\n����� �������: " << frames.size() << endl;
    cout << "�� ��� ����������: " << correctFrames << endl;
    cout << "������� ������������ �������: " << correctPercent << "%" << endl;
    cout << "������� ��� �������� ����������: " << countNoAddress << endl;
}
