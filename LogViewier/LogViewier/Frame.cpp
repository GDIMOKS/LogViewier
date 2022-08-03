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

int Frame::getType()
{
    if (type == -1)
        setType();

    return type;
}

void Frame::setType(int value)
{
    if (value != -1)
    {
        type = value;
        return;
    }

    string firstByte = getFrameHex().substr(0, 2);
    string binFrame = ffunc::HexToBin(firstByte);

    string typeBin = binFrame.substr(4, 2);
    type = ffunc::BinToDec(typeBin);


    auto iterator = subtypes.find(type);

    if (iterator == subtypes.end())
        type = -2;
}

int Frame::getSubtype()
{
    if (subtype == -1)
        setSubtype();
    
    return subtype;
}

void Frame::setSubtype(int value)
{
    if (value != -1)
    {
        subtype = value;
        return;
    }

    string firstByte = getFrameHex().substr(0, 2);
    string binFrame = ffunc::HexToBin(firstByte);

    string subtypeBin = binFrame.substr(0, 4);

    subtype = ffunc::BinToDec(subtypeBin);

    auto iterator = subtypes.at(getType()).find(subtype);

    if (iterator == subtypes.at(getType()).end())
        subtype = -2;
}



map<int, map<int, string>> Frame::subtypes
{
    {0,
        {
            {0, "Management/Association_Request"},      //3 адреса DA/RA SA/TA BSSID
            {1, "Management/Association_Response"},     // 3 адреса DA/RA SA/TA BSSID
            {2, "Management/Reassociation_Request"},    // 3 адреса
            {3, "Management/Reassociation_Response"},   // 3 адреса
            {4, "Management/Probe_Request"},            // 3 адреса DA/RA SA/TA BSSID
            {5, "Management/Probe_Response"},           // 3 адреса DA/RA SA/TA BSSID
            {6, "Management/Timing_Advertisement"},
            {8, "Management/Beacon"},                   //3 адреса DA/RA SA/TA BSSID
            {9, "Management/ATIM"},
            {10, "Management/Disassociation"},          // 3 адреса
            {11, "Management/Authentication"},          // 3 адреса DA/RA SA/TA BSSID
            {12, "Management/Deauthentication"},        // 3 адреса 
            {13, "Management/Action"}                   // 3 адреса DA/RA SA/TA BSSID
    }
    },
    { 1,
        {
            {3, "Control/TACK"},
            {4, "Control/Beamforming_Report_Poll"},     // 2 адреса RA TA
            {5, "Control/VHT/HE_NDP_Announcement"},     // 2 адреса RA TA
            {6, "Control/Control_Frame_Extension"},     // 1 адрес RA
            {7, "Control/Control_Wrapper"},             // 1 адрес RA
            {8, "Control/Block_Ack_Request"},           // 2 адреса RA TA
            {9, "Control/Block_Ack"},                   // 2 адреса RA TA
            {10, "Control/PS_Poll"},                    // 2 адреса BSSID/RA TA
            {11, "Control/RTS"},                        // 2 адреса RA TA
            {12, "Control/CTS"},                        // 1 адрес RA
            {13, "Control/ACK"},                        // 1 адрес RA
            {14, "Control/CF_End"},
            {15, "Control/CF_End+CF_ACK"}
        }
    },
    { 2,
        {
            {0, "Data/Data"},                           // 3 адреса RA/BSSID SA/TA DA
            {4, "Data/Null"},
            {8, "Data/QoS_Data"},                       // 4 адреса RA TA DA SA
            {9, "Data/QoS_Data+CF_ACK"},
            {10, "Data/QoS_Data+CF_Poll"},
            {11, "Data/QoS_Data+CF_ACK+CF_Poll"},
            {12, "Data/QoS_Null"},                      // 3 адреса RA/DA TA SA
            {14, "Data/QoS+CF_Poll"},
            {15, "Data/QoS+CF_ACK+CF_Poll"}
        }
    }
};

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

map<int, string>::iterator Frame::SearchSubtype(pair<int, int> typeSubtype, bool& isExist)
{
    auto iterator = subtypes.at(typeSubtype.first).find(typeSubtype.second);
    isExist = iterator != subtypes.at(typeSubtype.first).end();
    return iterator;
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

void Frame::ChoiceParam(pair<string, string>& params)
{
    if (params.first == "Offset")
    {
        setOffset(params.second);
        return;
    }
    else if (params.first == "BW")
    {
        setBW(params.second);
        return;
    }
    else if (params.first == "MCS")
    {
        setMCS(params.second);
        return;
    }
    else if (params.first == "Size")
    {
        setSize(params.second);
        return;
    }
    else if (params.first == "Frame" || params.first == "Bits")
    {
        setFrameHex(params.second);
        return;
    }

    if (params.first == "FCS" && params.second == "Fail")
    {
        setCorrect(false);
    }

    MakeParam(params.first, params.second);
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

void PrintStatistics(vector<Frame>& frames, int countNoAddress)
{
    int correctFrames = CountCorrect(frames);
    double correctPercent = correctFrames * 100.0 / frames.size();

    cout << "\nВсего фреймов: " << frames.size() << endl;
    cout << "Из них корректных: " << correctFrames << endl;
    cout << "Процент корректности фреймов: " << correctPercent << "%" << endl;
    cout << "Фреймов без адресной информации: " << countNoAddress << endl;
}
