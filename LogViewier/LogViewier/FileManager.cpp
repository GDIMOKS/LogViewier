#include "FileManager.h"

namespace filemanager {

    void FileManager::Dir(fs::path& currentDir)
    {
        for (fs::directory_iterator it(currentDir), end; it != end; ++it)
        {
            fs::path pth = it->path();

            try
            {
                if (fs::is_directory(pth))
                {
                    std::cout << "\t<DIR>\t";
                }
                else if (fs::is_regular_file(pth))
                {
                    std::cout << "\t<" << pth.extension().string() << ">\t";

                }
                std::cout << pth.filename().string() << std::endl;
            }
            catch (fs::filesystem_error) // temp files throw exceptions (for example, DumpStack.log.tmp)
            {

            }
        }
    }

    void FileManager::Cd(fs::path& currentDir, std::string command)
    {
        try
        {
            if (command == "..")
            {
                currentDir = currentDir.parent_path();
            }
            else if (fs::is_directory(currentDir / command)) // check directory
            {
                currentDir /= command;
            }
            else if ((currentDir / command).has_extension()) // check file
            {
                std::cout << "\nНевозможно открыть файл командой cd! Воспользуйтесь командой type." << std::endl;
            }
            else
            {
                std::cout << "\nТакой директории не существует! Введите dir для просмотра директорий." << std::endl;
                //cout << currentDir << endl;
                //cout << command << endl;
            }
        }
        catch (fs::filesystem_error)
        {
            std::cout << "\nНевозможно открыть директорию или доступ к ней затруднен." << std::endl;
        }
    }

    bool FileManager::Type(fs::path& currentDir, fs::path& fileDir, std::string fileName)
    {
        fs::path file = currentDir / fileName;

        if (fs::exists(file) && (file.extension() == ".log" || file.extension() == ".pcap"))
        {
            fileDir = file;
            std::cout << "\nПроисходит обработка файла " << fileDir.filename() << "...\n" << std::endl;

            return true;
        }
        else
        {
            std::cout << "\nВыбран неверный файл! Выберете файл с расширением .pcap или .log!" << std::endl;

            return false;
        }
    }

    void FileManager::Help()
    {
        std::cout << "Команды:\n\ttype <имя_файла> - открыть файл;\n\t" <<
            "dir - посмотреть содержимое текущей директории;\n\t" <<
            "cd <имя_директории> - перейти в указанную директорию;\n\t" <<
            "help - вывод справки по командам\n\t" <<
            "exit - выход из программы\n" << std::endl;
    }

    fs::path FileManager::FileManagerFunc(fs::path& lastPath)
    {
        fs::path workDir = fs::current_path();
        fs::path currentDir = workDir;

        if (!lastPath.empty())
        {
            currentDir = lastPath;
        }

        fs::path fileDir;

        std::string inputCommand = "";

        Help();

        while (inputCommand != "exit")
        {
            std::string fullCommand[2];

            std::cout << currentDir.string() << ">";
            getline(std::cin, inputCommand);

            if (strchr(inputCommand.c_str(), ' '))
            {
                ffunc::Separator(inputCommand, fullCommand[0], fullCommand[1], " ");
            }
            else
            {
                fullCommand[0] = inputCommand;
            }

            if (inputCommand == "dir")
            {
                Dir(currentDir);
            }

            if (fullCommand[0] == "cd" && !fullCommand[1].empty())
            {
                Cd(currentDir, fullCommand[1]);
            }
            else if (inputCommand == "cd")
            {
                std::cout << currentDir.string() << "\n\n";
            }

            if (fullCommand[0] == "type")
            {
                if (Type(currentDir, fileDir, fullCommand[1]))
                    break;
            }

            if (inputCommand == "help")
            {
                Help();
            }
        }

        return fileDir;
    }
}

