#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <vector>
#include <filesystem>
#include <windows.h>
#include <stdio.h>

#include "Frame.h"
#include "Param.h"
#include "ExtFrame.h"
#include "FileManager.h"
#include "FormatFunctions.h"
#include "Graph.h"
#include "Features.h"
#include "Device.h"
#include "Record.h"

namespace fs = std::filesystem;
using namespace std;
using Dataset = vector<Record>;
using Sample = vector<Frame>;

Graph MACReader(vector<Frame>&, int&);

void LogReader(fs::path);

void LogsTwoFiles(string, string, vector<Frame>&);

void LogsOneFile(string, vector<Frame>&);

void LogsToFrames(vector<Frame>&, Frame&, string[], const int);

vector<Sample> CreateSamples(vector<Device>&);

vector<Device> CreateDevices(vector<ExtFrame>&);

Dataset CreateRecords(vector<Sample>&);

vector<Device> CreateDataset();

vector<ExtFrame> RemoveDuplicates(vector<Frame>&);

vector<ExtFrame> DefragmentFrames(vector<ExtFrame>&);

//void KNN(DataSet&);

//double euclidian_distance(pair<double, double>&, pair<double, double>&);

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    //setlocale(LC_ALL, ""); //Cyrillic input from the console does not work

    //CreateDataset();

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

vector<ExtFrame> DefragmentFrames(vector<ExtFrame>& frames)
{
    
    vector<ExtFrame*> fullFrames;
    vector<ExtFrame*> heads;
    vector<ExtFrame*> bodies;
    vector<ExtFrame*> tails;

    for (ExtFrame& f : frames)
    {
        if (f.getHFlags()[5] == '0' && f.getFrNum() == 0)
        {
            fullFrames.push_back(&f);
        }
        else if (f.getHFlags()[5] != '0' && f.getFrNum() == 0)
        {
            heads.push_back(&f);
            
            fullFrames.push_back(&f);
        }
        else if (f.getHFlags()[5] != '0' && f.getFrNum() != 0)
        {
            bodies.push_back(&f);
        }
        else if (f.getHFlags()[5] == '0' && f.getFrNum() != 0)
        {
            tails.push_back(&f);
        }
    }

    if (!heads.empty() && !bodies.empty())
        for (ExtFrame* head : heads)
        {
            for (ExtFrame* body : bodies)
                if ((*head).getSeqNum() == (*body).getSeqNum() && (*head).getMac() == (*body).getMac())
                {
                    (*head).setSize(to_string(atoi((*head).getSize().c_str()) + atoi((*body).getSize().c_str())));
                    //head.setOffset(to_string(atof(head.getOffset().c_str()) + atof(body.getOffset().c_str())));

                    break;
                }
        }

    if (!heads.empty() && !tails.empty())
        for (ExtFrame* head : heads)
            for (ExtFrame* tail : tails)
                if ((*head).getSeqNum() == (*tail).getSeqNum() && (*head).getMac() == (*tail).getMac())
                {
                    (*head).setSize(to_string(atoi((*head).getSize().c_str()) + atoi((*tail).getSize().c_str())));
                    //head.setOffset(to_string(atof(head.getOffset().c_str()) + atof(tail.getOffset().c_str())));

                    string flags = (*head).getHFlags();
                    flags[5] = (*tail).getHFlags()[5];
                    (*head).setHFlags(flags);
                    (*head).setFrNum((*tail).getFrNum());

                    break;
                }

 

    vector<ExtFrame> newFrames;
    for (ExtFrame* f : fullFrames)
    {
        newFrames.push_back(*f);
    }
    
    frames.clear();

    return newFrames;
}

vector<Device> CreateDataset()
{
    vector<ExtFrame> extF;

    ifstream fin;
    fin.open("1.txt");
    while (!fin.eof())
    {
        string mac;
        string size;
        string offset;
        string flags;
        string seqnum;
        string frnum;
        fin >> mac >> size >> offset >> flags >> seqnum >> frnum;
        //cout << mac << "\t" << size << "\t" << offset << "\t" << flags << "\t" << seqnum << "\t" << frnum << endl;
        ExtFrame ex;
        ex.setMac(mac);
        ex.setSize(size);
        ex.setOffset(offset);
        ex.setHFlags(flags);
        ex.setSeqNum(atoi(seqnum.c_str()));
        ex.setFrNum(atoi(frnum.c_str()));
        extF.push_back(ex);
    }
    fin.close();

    vector<ExtFrame> fullFrames = DefragmentFrames(extF);

    vector<Device> devices;
    
    for (ExtFrame frame : fullFrames)
    {

        bool isExist = false;
        int j;

        for (int i = 0; i < devices.size(); i++)
        {
            if (devices[i].getMac() == frame.getMac())
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
            Device device;
            device.setMac(frame.getMac());
            device.frames.push_back(frame);
            devices.push_back(device);
        }
    }

    return devices;
}

vector<ExtFrame> RemoveDuplicates(vector<Frame>& frames)
{
    vector<ExtFrame> extF;
    vector<ExtFrame> buffer;

    for (int i = 0; i < frames.size(); i++)
    {
        if (!frames[i].getCorrect())
            continue;

        int type = frames[i].getType();
        int subType = frames[i].getSubtype();

        if (type == -2)
            continue;

        if (type == 1 && (subType == 6 || subType == 7 || subType == 12 || subType == 13))
            continue;

        ExtFrame tf = ExtFrame(frames[i]);//, to_string(offset));

        bool isExist = false;

        for (ExtFrame& e : buffer)
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
    buffer.clear();

    //ofstream fout;
    //fout.open("1.txt");
    //for (ExtFrame& frame : extF)
    //{
    //    fout << frame.getMac() << endl;
    //    fout << frame.getSize() << endl;
    //    fout << frame.getOffset() << endl;
    //    fout << frame.getHFlags() << endl;
    //    fout << frame.getSeqNum() << endl;
    //    fout << frame.getFrNum() << endl;
    //}

    //fout.close();

    return extF;
}

Dataset CreateDataset(vector<Sample>& samples)
{
    vector<Record> records;
    for (Sample& sample : samples)
    {
        Record record;

        record.set_features(sample);
        records.push_back(record);
    }

    return records;
}

vector<Sample> CreateSamples(vector<Device>& devices)
{
    vector<Sample> samples;
    for (Device& d : devices)
    {
        Sample sample;

        for (Frame& frame : d.frames)
        {
            sample.push_back(frame);
            if (sample.size() == 20)
            {
                samples.push_back(sample);
                sample.erase(sample.begin(), sample.begin() + 7);
            }
        }
    }
    
    return samples;


}

vector<Device> CreateDevices(vector<ExtFrame>& frames)
{   
    vector<Device> devices;
    
    for (ExtFrame& frame : frames)
    {
        bool isExist = false;
        int j;

        for (int i = 0; i < devices.size(); i++)
        {
            if (devices[i].getMac() == frame.getMac())
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
            Device device;
            device.setMac(frame.getMac());
            device.frames.push_back(frame);
            devices.push_back(device);
        }
    }

    return devices;
}

Graph MACReader(vector<Frame>& frames, int& countNoAddress)
{
    Graph g;
    countNoAddress = 0;

    vector<string> addresses;
    std::vector<string>::iterator iter;

    addresses.clear();

    for (Frame f : frames)
    {
        if (!f.getCorrect())
            continue;
                            
        int type = f.getType();
        int subType = f.getSubtype();

        if (type == -2)
        {
            cout << "Тип фрейма #" + f.getFrameName() + " не удалось распознать!\n";
            continue;
        }

        string RA = f.GetAddress(f.getFrameHex(), 8);
        string TA = "";
        //string BSSID = "";
        string SSID = "";
        int SSID_length = 0;

        iter = find(addresses.begin(), addresses.end(), RA);
        if (iter == addresses.end())
        {
            addresses.push_back(RA);
        }

        if (!(type == 1 && (subType == 6 || subType == 7 || subType == 12 || subType == 13)))
        {
            TA = f.GetAddress(f.getFrameHex(), 20);

            if (find(addresses.begin(), addresses.end(), TA) == addresses.end())
            {
                addresses.push_back(f.GetAddress(f.getFrameHex(), 20));
            }
        }
        else
        {
            countNoAddress++;
        }

        if (type == 0 && subType == 8)
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
            //    GraphFunction(g, BSSID, RA, SSID);
            //}
            //else
            //{
            //    GraphFunction(g, TA, RA, SSID);
            //}


        }
        else
        {
            GraphFunction(g, TA, RA);
        }

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

    string file1 = file.string();
    string file2 = file1;

    if (file.filename().string().find("_phy") != -1)
    {
        file2.replace(file2.rfind("phy"), 3, "parser");

        LogsTwoFiles(file1, file2, frames);
    }
    else if (file.filename().string().find("_parser") != -1)
    {
        file2.replace(file2.rfind("parser"), 6, "phy");

        LogsTwoFiles(file2, file1, frames);
    }
    else
    {
        LogsOneFile(file.string(), frames);
    }

    //PrintFrames(frames);

    vector<ExtFrame> extended_frames = RemoveDuplicates(frames);
    vector<ExtFrame> defragmented_frames = DefragmentFrames(extended_frames);
    vector<Device> devices = CreateDevices(defragmented_frames);
    vector<Sample> samples = CreateSamples(devices);
    Dataset dataser = CreateDataset(samples);

    

    //KNN(dataset);

    //int countNoAddress;
    //Graph g = MACReader(frames, countNoAddress);

    //

    //cout << endl;

    //PrintStatistics(frames, countNoAddress);
    ////cout << endl;
    ////PrintGraph(g);
    //cout << endl;

    //vector<Network> networks = CreateNetworks(g);
    //PrintNetworkGraph(g, networks);


    cout << endl;
}

//void KNN(DataSet& ds) {
//
//    vector<pair<double, double>> train, test;
//    //vector<string> x_train, x_test, y_train, y_test;
//
//    
//    for (Device d : ds)
//    {
//        for (int i = 0; i < size(d.frames); i++)
//        {
//            if (i <= size(d.frames) *2/3)
//            {
//                pair<double, double> p_train;
//                p_train.first = atof(d.frames[i].getSize().c_str());
//                p_train.second = atof(d.frames[i].getOffset().c_str());
//                //x_train.push_back(d.frames[i].getSize());
//                //y_train.push_back(d.frames[i].getOffset());
//
//                train.push_back(p_train);
//            }
//            else
//            {
//                pair<double, double> p_test;
//                p_test.first = atof(d.frames[i].getSize().c_str());
//                p_test.second = atof(d.frames[i].getOffset().c_str());
//                //x_test.push_back(d.frames[i].getSize());
//                //y_test.push_back(d.frames[i].getOffset());
//                test.push_back(p_test);
//            }
//        }
//    }
//
//    vector<double> distances;
//    for (pair<double, double> p1 : test)
//    {
//        for (pair<double, double> p2 : train)
//        {
//            distances.push_back(euclidian_distance(p1, p2));
//        }
//    }
//
//}

//double euclidian_distance(pair<double,double>& data1, pair<double, double>& data2)
//{
//    double distance = 0;
//
//    distance = pow(data1.first - data2.first, 2) + pow(data1.second - data2.second, 2);
//
//    return sqrt(distance);
//}


void LogsTwoFiles(string phyName, string parserName, vector<Frame>& frames)
{
    string file1 = phyName;
    string file2 = parserName;
    
    ifstream phyIn;
    ifstream parserIn;

    phyIn.open(file1);
    parserIn.open(file2);

    while (!phyIn.eof() && !parserIn.eof())
    {
        Frame frame;
        string buffStr[2];
        string name1;
        string name2;

        phyIn >> name1;
        parserIn >> name2;

        if (name1 != name2)
        {
           //something wrong
        }

        frame.setFrameName(name1);

        getline(phyIn, buffStr[0]);
        getline(parserIn, buffStr[1]);

        LogsToFrames(frames, frame, buffStr, size(buffStr));

    }
    parserIn.close();
    phyIn.close();
}

void LogsOneFile(string fileName, vector<Frame>& frames)
{
    ifstream fin;

    fin.open(fileName);
    while (!fin.eof())
    {
        Frame frame;
        string buffStr[4];
        string name;

        fin >> name;
        frame.setFrameName(name);

        for (int i = 0; i < size(buffStr); i++)
            getline(fin, buffStr[i]);

        LogsToFrames(frames, frame, buffStr, size(buffStr));
    }

    fin.close();
}

void LogsToFrames(vector<Frame>& frames, Frame& frame, string b[], const int size)
{
    for (int i = 0; i < size; i++)
    {
        if (b[i].empty())
            continue;
        
        b[i] = ffunc::EraseSymbol(b[i], '\t');

        pair<string, string> params;
        vector<string> parameters;

        ffunc::Separator(b[i], parameters, ",");

        for (string p : parameters)
        {
            ffunc::Separator(p, params);
            frame.ChoiceParam(params);
        }
    }

    if (frame.getFrameName() != "")
        frames.push_back(frame);
}