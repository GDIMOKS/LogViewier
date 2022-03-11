#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <vector>
#include <filesystem>
#include <windows.h>

#include "Frame.h"
#include "Param.h"

//#include <list>
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

int CountCorrect(vector<Frame>* frames) //list<Frame> *frames)
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

void PrintFrames(vector<Frame>* frames) //list<Frame> *frames)
{
    for (Frame f : *frames)
    {
        cout << f.getFrameName() << "\tOffset=" << f.getOffset() << ", BW=" << f.getBW() << ", MCS=" << f.getMCS() << ", Size=" << f.getSize() << endl;
        cout << "\t\tFrame=" << f.getFrameHex() << "\n\t\t";
        f.PrintParams();
        cout << "\n\n";
    }
}

/*
void PrintStatistics(vector<Frame> *frames) //list<Frame> *frames)
{
    int correctFrames = CountCorrect(frames);
    double correctPercent = correctFrames * 100.0 / (*frames).size();

    cout << "\nВсего фреймов: " << (*frames).size() << endl;
    cout << "Из них корректных: " << correctFrames << endl;
    cout << "Процент корректности фреймов: " << correctPercent << "%\n\n";
}
*/

void PrintStatistics(int framesCount, int framesIncorrect) //list<Frame> *frames)
{
    //int correctFrames = CountCorrect(frames);
    double correctPercent = (framesCount - framesIncorrect) * 100.0 / framesCount;

    cout << "\nВсего фреймов: " << framesCount << endl;
    cout << "Из них корректных: " << framesIncorrect << endl;
    cout << "Процент корректности фреймов: " << correctPercent << "%\n\n";
}

void LogReader(string fileName)
{
    //list<Frame> frames;
    //vector<Frame> frames;
    int countFrames = 0;
    int incorrectFrames = 0;
    ifstream fin;

    fin.open(fileName);
    while (!fin.eof())
    {
        //Frame frame;
        string buffStr[4];
        string name;
        string value;
        char buffChar[10000] = { 0 };

        fin >> name;
        //frame.setFrameName(name);

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
                if (b == "FCS=Fail")
                {
                    incorrectFrames++;
                }

                //for (int i = 0, j = 0; i <= b.length(); i++)
                //{
                //    if (b[i] == ',' || b[i] == '\0')
                //    {
                //        value = buffChar;
                //        memset(buffChar, 0, strlen(buffChar));
                //        j = 0;
                //
                //        size_t pos = value.find("=");
                //        string param[2];
                //
                //        param[0] = value.substr(0, pos);
                //        param[1] = value.substr(pos + 1, value.length() - 1);
                //        /*
                //        if (param[0] == "Offset")
                //        {
                //            frame.setOffset(param[1]);
                //            continue;
                //        }
                //        if (param[0] == "BW")
                //        {
                //            frame.setBW(param[1]);
                //            continue;
                //        }
                //        if (param[0] == "MCS")
                //        {
                //            frame.setMCS(param[1]);
                //            continue;
                //        }
                //        if (param[0] == "Size")
                //        {
                //            frame.setSize(param[1]);
                //            continue;
                //        }
                //        if (param[0] == "Frame")
                //        {
                //            frame.setFrameHex(param[1]);
                //            continue;
                //        }
                //
                //        if (param[0] == "FCS" && param[1] == "Fail")
                //        {
                //            frame.setCorrect(false);
                //        }
                //
                //        frame.MakeParam(param[0], param[1]);
                //        */
                //
                //        if (param[0] == "FCS" && param[1] == "Fail")
                //        {
                //            incorrectFrames++;
                //            //frame.setCorrect(false);
                //        }
                //        continue;
                //    }
                //    buffChar[j] = b[i];
                //    j++;
                //}
            }
        }

        countFrames++;
        //if (frame.getFrameName() != "")
            //frames.push_back(frame);
    }
    fin.close();

    PrintStatistics(countFrames, incorrectFrames);

    //PrintFrames(&frames);
}

void Dir(fs::path* currentDir)
{
    for (fs::directory_iterator it(*currentDir), end; it != end; ++it)
    {
        fs::path pth = it->path();

        try
        {
            if (fs::is_directory(pth))
            {
                cout << "\t<DIR>\t";
            }
            else if (fs::is_regular_file(pth))
            {
                cout << "\t<" << pth.extension().string() << ">\t";

            }
            cout << pth.filename().string() << endl;
        }
        catch (fs::filesystem_error) // из-за наличия временных файлов (например, DumpStack.log.tmp) выдает исключение
        {

        }
    }
}

void Cd(fs::path* currentDir, string command)
{
    try
    {
        if (command == "..")
        {
            *currentDir = (*currentDir).parent_path();
        }
        else if (fs::is_directory(*currentDir / command)) // проверка на директорию
        {
            *currentDir /= command;
        }
        else if ((*currentDir / command).has_extension()) // проверка на файл
        {
            cout << "\nНевозможно открыть файл командой cd! Воспользуйтесь командой type." << endl;
        }
        else
        {
            cout << "\nТакой директории не существует! Введите dir для просмотра директорий." << endl;
            //cout << *currentDir << endl;
            //cout << command << endl;
        }
    }
    catch (fs::filesystem_error)
    {
        cout << "\nНевозможно открыть директорию или доступ к ней затруднен." << endl;
    }
}

bool Type(fs::path* currentDir, fs::path* fileDir, string fileName)
{
    fs::path file = *currentDir / fileName;

    if (fs::exists(file) && (file.extension() == ".log" || file.extension() == ".pcap"))
    {
        *fileDir = file;
        cout << "\nПроисходит обработка файла " << (*fileDir).filename() << "..." << endl;

        return true;
    }
    else
    {
        cout << "\nВыбран неверный файл! Выберете файл с расширением .pcap или .log!" << endl;

        return false;
    }
}

void Help()
{
    cout << "Команды:\n\ttype <имя_файла> - открыть файл;\n\t" <<
        "dir - посмотреть содержимое текущей директории;\n\t" <<
        "cd <имя_директории> - перейти в указанную директорию;\n\t" <<
        "help - вывод справки по командам\n\t" <<
        "exit - выход из программы\n" << endl;
}

fs::path FileManager(fs::path* lastPath)
{
    fs::path workDir = fs::current_path();
    fs::path currentDir = workDir;

    if (!(*lastPath).empty())
    {
        currentDir = *lastPath;
    }

    fs::path fileDir;

    string inputCommand = "";

    Help();

    while (inputCommand != "exit")
    {
        string fullCommand[2];
        size_t pos;

        cout << currentDir.string() << ">";
        getline(cin, inputCommand);

        if (strchr(inputCommand.c_str(), ' '))
        {
            pos = inputCommand.find(' ');
            fullCommand[0] = inputCommand.substr(0, pos);
            fullCommand[1] = inputCommand.substr(pos + 1, inputCommand.length() - 1);
        }
        else
        {
            fullCommand[0] = inputCommand;
        }

        if (inputCommand == "dir")
        {
            Dir(&currentDir);
        }

        if (fullCommand[0] == "cd" && !fullCommand[1].empty())
        {
            Cd(&currentDir, fullCommand[1]);
        }
        else if (inputCommand == "cd")
        {

        }

        if (fullCommand[0] == "type")
        {
            if (Type(&currentDir, &fileDir, fullCommand[1]))
                break;
        }

        if (inputCommand == "help")
        {
            Help();
        }
    }

    return fileDir;
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
        fs::path file = FileManager(&lastPath);
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

