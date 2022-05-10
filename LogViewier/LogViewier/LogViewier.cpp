#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <vector>
#include <filesystem>
#include <windows.h>
#include <map>

#include "Frame.h"
#include "Param.h"
#include "FileManager.h"
#include "FormatFunctions.h"
#include "Graph.h"


namespace fs = std::filesystem;
namespace ffunc = formatfunctions;
using namespace std;



int CountCorrect(vector<Frame>&);

void PrintFrames(vector<Frame>&);

void ChoiceParam(Frame&, string&, string&);

void PrintStatistics(vector<Frame>&);

Graph MACReader(vector<Frame>&);

void LogReader(string);


int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    //setlocale(LC_ALL, "Russian"); //Cyrillic input from the console does not work

    fs::path lastPath = "";
    string command = "";

    while (command != "exit")
    {
        fs::path file = filemanager::FileManager::FileManagerFunc(lastPath);
        lastPath = file.parent_path();

        if (file.extension() == ".log")
        {
            LogReader(file.string());
        }
        else if (file.extension() == ".pcap")
        {

        }

        cout << "\nДля выхода введите exit, для обработки еще одного файла нажмите любую клавишу\n";

        getline(cin, command);
    }

    system("pause");

    return 0;
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

void PrintStatistics(vector<Frame>& frames)
{
    int correctFrames = CountCorrect(frames);
    double correctPercent = correctFrames * 100.0 / frames.size();

    cout << "\nВсего фреймов: " << frames.size() << endl;
    cout << "Из них корректных: " << correctFrames << endl;
    cout << "Процент корректности фреймов: " << correctPercent << "%\n\n";
}

Graph MACReader(vector<Frame>& frames)
{
    Graph g;
    int countNoAddress = 0;
    map<int, map<int, string>> subtypes
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
        {1,
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
        {2,
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

    vector<string> addresses;
    std::vector<string>::iterator iter;

    addresses.clear();

    for (Frame f : frames)
    {
        if (f.getCorrect())
        {
            string firstByte = f.getFrameHex().substr(0, 2);
            //string secondByte = f.getFrameHex().substr(2, 2);

            string binFrame = ffunc::FormatFunctions::HexToBin(firstByte);

            /*string flagsFrame = ffunc::FormatFunctions::HexToBin(secondByte);
            reverse(flagsFrame.begin(), flagsFrame.end());
            string direction = flagsFrame.substr(0, 2);*/

            string typeBin = binFrame.substr(4, 2);
            string subtypeBin = binFrame.substr(0, 4);

            int typeDec = ffunc::FormatFunctions::BinToDec(typeBin);
            int subtypeDec = ffunc::FormatFunctions::BinToDec(subtypeBin);

            auto it = subtypes.at(typeDec).find(subtypeDec);

            if (it != subtypes.at(typeDec).end())
            {
                string RA = f.GetAddress(f.getFrameHex(), 8);
                string TA = "";
                /*cout << f.getFrameName() << "\n";
                cout << "Type=" << it->second << endl;
                cout << "RA=" << RA << endl;*/

                iter = find(addresses.begin(), addresses.end(), RA);
                if (iter == addresses.end())
                {
                    addresses.push_back(RA);
                }

                if (!(typeDec == 1 && (it->first == 6 || it->first == 7 || it->first == 12 || it->first == 13)))
                {
                    TA = f.GetAddress(f.getFrameHex(), 20);

                    if (find(addresses.begin(), addresses.end(), TA) == addresses.end())
                    {
                        addresses.push_back(f.GetAddress(f.getFrameHex(), 20));
                    }
                    //cout << "TA=" << TA << endl;
                }
                else
                {
                    countNoAddress++;
                }
                
                

                GraphFunction(g, RA, TA);

                /*if (typeDec == 0)
                {
                    if (find(addresses.begin(), addresses.end(), f.GetAddress(f.getFrameHex(), 32)) == addresses.end())
                    {
                        addresses.push_back(f.GetAddress(f.getFrameHex(), 32));
                    }

                    cout << "BSSID=" << f.GetAddress(f.getFrameHex(), 32) << endl;
                }*/

                /*if (typeDec == 2 && direction == "11")
                {
                    if (find(addresses.begin(), addresses.end(), f.GetAddress(f.getFrameHex(), 46)) == addresses.end())
                    {
                        addresses.push_back(f.GetAddress(f.getFrameHex(), 46));
                    }

                    cout << "DA=" << f.GetAddress(f.getFrameHex(), 32) << endl;
                    cout << "SA=" << f.GetAddress(f.getFrameHex(), 46) << endl;
                }*/
                    
            }
            else
            {
                cout << "Элемент не найден!\n";
            }

            //cout << endl << endl;
        }
    }

    cout << "Адреса участников:\n\n";
    for (string str : addresses)
    {
        cout << str << endl;
    }

    //cout << endl;
    cout << "Фреймов без адресной информации: " << countNoAddress << endl;
    return g;
}

void LogReader(string fileName)
{
    vector<Frame> frames;
    ifstream fin;

    fin.open(fileName);
    while (!fin.eof())
    {
        Frame frame;
        string buffStr[4];
        string name;
        string value;
        char buffChar[1000] = { 0 };

        fin >> name;
        frame.setFrameName(name);

        for (int i = 0; i < size(buffStr); i++)
        {
            getline(fin, buffStr[i]);
            buffStr[i] = ffunc::FormatFunctions::EraseSymbol(buffStr[i], '\t');
        }

        for (string b : buffStr)
        {
            if (!b.empty())
            {
                string param[2];
                ffunc::FormatFunctions::Separator(b, param[0], param[1]);

                if (param[0] == "Frame")    // записывается отдельно, чтобы не нагружать буфер и стек
                {
                    frame.setFrameHex(param[1]);
                    continue;
                }

                for (int i = 0, j = 0; i <= b.length(); i++)
                {
                    if (b[i] == ',' || b[i] == '\0')
                    {
                        value = buffChar;
                        memset(buffChar, 0, strlen(buffChar));
                        j = 0;

                        //string param[2];
                        ffunc::FormatFunctions::Separator(value, param[0], param[1]);

                        ChoiceParam(frame, param[0], param[1]);

                        continue;
                    }

                    buffChar[j] = b[i];
                    j++;
                }
            }
        }

        if (frame.getFrameName() != "")
            frames.push_back(frame);
    }
    fin.close();

    //PrintFrames(&frames);

    Graph g = MACReader(frames);

    PrintGraph(g);

    PrintStatistics(frames);


}