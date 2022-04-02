#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <vector>
#include <filesystem>
#include <windows.h>

#include "Frame.h"
#include "Param.h"
#include "FileManager.h"


namespace fs = std::filesystem;
using namespace std;

string EraseSymbol(string str, char symbol)
{
    while (str.find(symbol) != -1)
    {
        str.erase(str.find(symbol), 1);
    }

    return str;
}

int CountCorrect(vector<Frame>* frames)
{
    int sum = 0;

    for (Frame f : *frames)
    {
        if (f.getCorrect() == true)
        {
            sum++;
        }
    }

    return sum;
}

void PrintFrames(vector<Frame>* frames)
{
    for (Frame f : *frames)
    {
        cout << f.getFrameName() << "\tOffset=" << f.getOffset() << ", BW=" << f.getBW() << ", MCS=" << f.getMCS() << ", Size=" << f.getSize() << endl;
        cout << "\t\tFrame=" << f.getFrameHex() << "\n\t\t";
        f.PrintParams();
        cout << "\n\n";
    }
}

void ChoiceParam(Frame *frame, string *paramName, string *paramValue)
{
    if (*paramName == "Offset")
    {
        (*frame).setOffset(*paramValue);
        return;
    }
    else if (*paramName == "BW")
    {
        (*frame).setBW(*paramValue);
        return;
    }
    else if (*paramName == "MCS")
    {
        (*frame).setMCS(*paramValue);
        return;
    }
    else if (*paramName == "Size")
    {
        (*frame).setSize(*paramValue);
        return;
    }
    else if (*paramName == "Frame")
    {
        (*frame).setFrameHex(*paramValue);
        return;
    }
    
    if (*paramName == "FCS" && *paramValue == "Fail")
    {
        (*frame).setCorrect(false);
    }

    (*frame).MakeParam(*paramName, *paramValue);
}

void PrintStatistics(vector<Frame> *frames)
{
    int correctFrames = CountCorrect(frames);
    double correctPercent = correctFrames * 100.0 / (*frames).size();

    cout << "\nВсего фреймов: " << (*frames).size() << endl;
    cout << "Из них корректных: " << correctFrames << endl;
    cout << "Процент корректности фреймов: " << correctPercent << "%\n\n";
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
        char buffChar[10000] = { 0 };

        fin >> name;
        frame.setFrameName(name);

        for (int i = 0; i < size(buffStr); i++)
        {
            getline(fin, buffStr[i]);
            buffStr[i] = EraseSymbol(buffStr[i], '\t');
        }

        for (string b : buffStr)
            //for (int k = 0; k < size(buffStr)-1; k++)
        {
            if (!b.empty())
            {
                for (int i = 0, j = 0; i <= b.length(); i++)
                {
                    if (b[i] == ',' || b[i] == '\0')
                    {
                        value = buffChar;
                        memset(buffChar, 0, strlen(buffChar));
                        j = 0;
                
                        size_t pos = value.find("=");
                        string param[2];
                
                        param[0] = value.substr(0, pos);
                        param[1] = value.substr(pos + 1, value.length() - 1);
                        
                        ChoiceParam(&frame, &param[0], &param[1]);

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

    PrintFrames(&frames);

    PrintStatistics(&frames);
}



int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    //setlocale(LC_ALL, "Russian"); Почему-то при введении в консоль данных на кириллице работает некорректно

    fs::path lastPath = "";
    string command = "";

    while (command != "exit")
    {
        fs::path file = filemanager::FileManager::FileManagerFunc(&lastPath);
        lastPath = file.parent_path();

        if (file.extension() == ".log")
        {
            LogReader(file.string());
        }
        else if (file.extension() == ".pcap")
        {

        }

        cout << "\nДля выхода введите exit, для просмотра еще одного файла нажмите любую клавишу\n";
        getline(cin, command);
    }

    system("pause");

    return 0;
}

