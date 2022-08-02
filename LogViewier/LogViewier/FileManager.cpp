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
                std::cout << "\n���������� ������� ���� �������� cd! �������������� �������� type." << std::endl;
            }
            else
            {
                std::cout << "\n����� ���������� �� ����������! ������� dir ��� ��������� ����������." << std::endl;
            }
        }
        catch (fs::filesystem_error)
        {
            std::cout << "\n���������� ������� ���������� ��� ������ � ��� ���������." << std::endl;
        }
    }

    bool FileManager::Type(fs::path& currentDir, fs::path& fileDir, std::string fileName)
    {
        fs::path file = currentDir / fileName;

        if (fs::exists(file) && (file.extension() == ".log" || file.extension() == ".pcap"))
        {
            fileDir = file;
            std::cout << "\n���������� ��������� ����� " << fileDir.filename() << "...\n" << std::endl;

            return true;
        }
        else
        {
            std::cout << "\n������ �������� ����! �������� ���� � ����������� .pcap ��� .log!" << std::endl;

            return false;
        }
    }

    void FileManager::Help()
    {
        std::cout << "�������:\n\ttype <���_�����> - ������� ����;\n\t" <<
            "dir - ���������� ���������� ������� ����������;\n\t" <<
            "cd <���_����������> - ������� � ��������� ����������;\n\t" <<
            "help - ����� ������� �� ��������\n\t" <<
            "exit - ����� �� ���������\n" << std::endl;
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
            std::pair <std::string, std::string> fullCommand;

            std::cout << currentDir.string() << ">";
            getline(std::cin, inputCommand);

            if (strchr(inputCommand.c_str(), ' '))
            {
                ffunc::Separator(inputCommand, fullCommand, " ");
            }
            else
            {
                fullCommand.first = inputCommand;
            }

            if (inputCommand == "dir")
            {
                Dir(currentDir);
            }

            if (fullCommand.first == "cd" && !fullCommand.second.empty())
            {
                Cd(currentDir, fullCommand.second);
            }
            else if (inputCommand == "cd")
            {
                std::cout << currentDir.string() << "\n\n";
            }

            if (fullCommand.first == "type")
            {
                if (Type(currentDir, fileDir, fullCommand.second))
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

