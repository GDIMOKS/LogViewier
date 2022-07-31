#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <vector>
#include <filesystem>
#include <windows.h>
#include <map>
#include <sstream>
#include <stdio.h>

#include "Frame.h"
#include "Param.h"
#include "ExtFrame.h"
#include "FileManager.h"
#include "FormatFunctions.h"
#include "Graph.h"
#include "Features.h"
#include "Device.h"

namespace fs = std::filesystem;
using namespace std;

Graph MACReader(vector<Frame>&);

void LogReader(fs::path file);

void LogsTwoFiles(string, string, fs::path, vector<Frame>&);

void LogsOneFile(string, vector<Frame>&);

void CalculateFeatures(vector<Frame>& frames);

void DefineDevice(vector<Frame>& frames);



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
            LogReader(file);
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



void DefineDevice(vector<Frame>& frames)
{
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

    
    vector<ExtFrame> extF;
    vector<ExtFrame> buffer;

    for (int i = 0; i < frames.size(); i++)
    {
        if (!frames[i].getCorrect())
            continue;

        double offset;

        if (i == 0)
        {
            offset = atof(frames[i].getOffset().c_str());
        }
        else
        {
            offset = atof(frames[i].getOffset().c_str()) - atof(frames[i-1].getOffset().c_str());
        }


        string firstByte = frames[i].getFrameHex().substr(0, 2);

        string binFrame = ffunc::HexToBin(firstByte);

        string typeBin = binFrame.substr(4, 2);
        string subtypeBin = binFrame.substr(0, 4);

        int typeDec = ffunc::BinToDec(typeBin);
        int subtypeDec = ffunc::BinToDec(subtypeBin);

        auto it = subtypes.at(typeDec).find(subtypeDec);

        if (it != subtypes.at(typeDec).end())
        {
            if (typeDec == 1 && (it->first == 6 || it->first == 7 || it->first == 12 || it->first == 13))
                continue;

            ExtFrame tf = ExtFrame(frames[i], typeDec, to_string(offset));

            bool isExist = false;

            for (ExtFrame e : buffer)
                if (tf.getFrNum() == e.getFrNum() && tf.getSeqNum() == e.getSeqNum())
                {
                    isExist = true;
                    break;
                }

            if (!isExist)
                extF.push_back(tf);

            if (buffer.size() >= 8)
            {
                buffer.emplace(buffer.cbegin(), tf);
                buffer.pop_back();
            }
            else
                buffer.push_back(tf);
        }
    }
    buffer.clear();

    

    //cout << "\tsize\toffset\t\tfrnumber\tseqnumber\tflags" << endl;
    //for (ExtFrame e : extF)
    //{
    //    if (e.getSeqNum() == 1)
    //        cout /* << "\t" << e.getMac() */ << "\t" << e.getSize() << "\t" << e.getOffset() << "  \t" << e.getFrNum() << "\t\t" << e.getSeqNum() << "\t\t" << e.getHFlags() << endl;
    //}

    vector<Device> devices;
    Device device;
    for (ExtFrame tmpF : extF)
    {
        Frame frame;
        frame.setSize(tmpF.getSize());
        frame.setOffset(tmpF.getOffset());

        bool isExist = false;
        int j;

        for (int i = 0; i < devices.size(); i++)
        {
            if (devices[i].getMac() == tmpF.getMac())
            {
                isExist = true;
                j = i;
                break;
            }
        }

        if (isExist)
        {
            devices[j].frames.push_back(frame);
        }
        else
        {
            device.setMac(tmpF.getMac());
            device.frames.push_back(frame);
            devices.push_back(device);
        }
    }


    for (Device d : devices)
    {
        cout << "Device " << d.getMac() << ":" << endl;

        CalculateFeatures(d.frames);

        //cout << "\tsize\toffset\t\tfrnumber\tseqnumber\tflags" << endl;
        //for (ExtFrame t : d.tFrames)
        //{
        //    //if (t.seqNumber == 0)
        //    cout /* << "\t" << t.mac */ << "\t" << t.size << "\t" << t.offset << "  \t" << t.frNumber << "\t\t" << t.seqNumber << "\t\t" << t.headerFlags << endl;
        //}
        //cout << endl;

    }
    
}


Graph MACReader(vector<Frame>& frames, int& countNoAddress)
{
    Graph g;
    countNoAddress = 0;
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
        if (!f.getCorrect())
            continue;
                            
        string firstByte = f.getFrameHex().substr(0, 2);
        //string secondByte = f.getFrameHex().substr(2, 2);

        string binFrame = ffunc::HexToBin(firstByte);

        /*string flagsFrame = ffunc::HexToBin(secondByte);
        reverse(flagsFrame.begin(), flagsFrame.end());
        string direction = flagsFrame.substr(0, 2);*/

        string typeBin = binFrame.substr(4, 2);
        string subtypeBin = binFrame.substr(0, 4);

        int typeDec = ffunc::BinToDec(typeBin);
        int subtypeDec = ffunc::BinToDec(subtypeBin);

        auto it = subtypes.at(typeDec).find(subtypeDec);
        if (it == subtypes.at(typeDec).end())
        {
            cout << "Тип фрейма #" + f.getFrameName() + " не удалось распознать!\n";
            continue;
        }

        string RA = f.GetAddress(f.getFrameHex(), 8);
        string TA = "";
        //string BSSID = "";
        string SSID = "";
        int SSID_length = 0;

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

        if (typeDec == 0 && it->first == 8)
        {
            if (find(addresses.begin(), addresses.end(), f.GetAddress(f.getFrameHex(), 32)) == addresses.end())
            {
                addresses.push_back(f.GetAddress(f.getFrameHex(), 32));
            }

            //BSSID = f.GetAddress(f.getFrameHex(), 32);
            SSID_length = ffunc::HexToDec(f.getFrameHex().substr(72, 4));

            if (SSID_length == 0)
            {
                SSID = "-";
            }
            else
            {
                SSID = ffunc::HexToASCII(f.getFrameHex().substr(76, SSID_length * 2));
            }

            GraphFunction(g, TA, RA, SSID);

            //if (BSSID == TA)
            //{
            //    //cout << "BSSID = " << BSSID << "\tTA = " << TA << endl;
            //    GraphFunction(g, BSSID, RA, SSID);
            //}
            //else
            //{
            //    GraphFunction(g, TA, RA, SSID);
            //}


            //cout << "BSSID=" << BSSID << endl;                
            //cout << "SSID=" << SSID << endl;
        }
        else
        {
            GraphFunction(g, TA, RA);
        }

        /*if (typeDec == 2 && direction == "11")
        {
            if (find(addresses.begin(), addresses.end(), f.GetAddress(f.getFrameHex(), 46)) == addresses.end())
            {
                addresses.push_back(f.GetAddress(f.getFrameHex(), 46));
            }

            cout << "DA=" << f.GetAddress(f.getFrameHex(), 32) << endl;
            cout << "SA=" << f.GetAddress(f.getFrameHex(), 46) << endl;
        }*/

        //cout << endl << endl;
    }

    /*cout << "Адреса участников:\n\n";
    for (string str : addresses)
    {
        cout << str << endl;
    }*/

    

    return g;
}

void LogReader(fs::path file)
{
    vector<Frame> frames;
    fs::path lastPath = file.parent_path();

    if (file.filename().string().find("_phy") != -1)
    {
        string file1 = file.string();
        string file2 = file1;
        file2.replace(file2.rfind("phy"), 3, "parser");

        LogsTwoFiles(file1, file2, lastPath, frames);
    }
    else if (file.filename().string().find("_parser") != -1)
    {
        string file2 = file.string();
        string file1 = file2;
        file1.replace(file1.rfind("parser"), 6, "phy");

        LogsTwoFiles(file1, file2, lastPath, frames);
    }
    else
    {
        LogsOneFile(file.string(), frames);
    }

    //PrintFrames(frames);

    DefineDevice(frames);

    int countNoAddress;
    Graph g = MACReader(frames, countNoAddress);

    

    cout << endl;

    PrintStatistics(frames, countNoAddress);
    //cout << endl;
    //PrintGraph(g);
    cout << endl;

    vector<Network> networks = CreateNetworks(g);
    PrintNetworkGraph(g, networks);


    cout << endl;
}

void LogsTwoFiles(string phyName, string parserName, fs::path lastPath, vector<Frame>& frames)
{
    string file1 = phyName;
    string file2 = parserName;
    
    ifstream phyIn;
    ifstream parserIn;
    ofstream fout;

    fs::path currentDir = lastPath;
    currentDir /= "tempFile.log";
    string tempfile = currentDir.filename().string();

    fout.open(tempfile);

    phyIn.open(file1);
    parserIn.open(file2);

    while (!phyIn.eof() && !parserIn.eof())
    {
        string str;
        string str1;
        string str2;
        string tempstring;

        getline(phyIn, str);

        if (str.empty())
            break;

        str1 = str.substr(0, str.find("Bits") - 1);
        str2 = str.substr(str.find("Bits"));
        parserIn >> tempstring;
        getline(parserIn, tempstring);

        fout << str1 << "\n\t\t" << str2 << "\n\t" << tempstring << "\n\n";
    }
    parserIn.close();
    phyIn.close();

    fout.close();
    LogsOneFile(tempfile, frames);

    remove(tempfile.c_str());
}

void LogsOneFile(string fileName, vector<Frame>& frames)
{
    //vector<Frame> frames;
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
            buffStr[i] = ffunc::EraseSymbol(buffStr[i], '\t');
        }

        for (string b : buffStr)
        {
            if (!b.empty())
            {
                string param[2];
                ffunc::Separator(b, param[0], param[1]);

                if (param[0] == "Frame" || param[0] == "Bits")    // записывается отдельно, чтобы не нагружать буфер и стек
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
                        ffunc::Separator(value, param[0], param[1]);

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
}