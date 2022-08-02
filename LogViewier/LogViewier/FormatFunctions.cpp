#include "FormatFunctions.h"

namespace formatfunctions {
    std::string FormatFunctions::EraseSymbol(std::string str, char symbol)
    {
        while (str.find(symbol) != -1)
        {
            str.erase(str.find(symbol), 1);
        }

        return str;
    }

    void FormatFunctions::Separator(std::string value, std::pair<std::string, std::string>& params, std::string symbol)
    {
        size_t pos = value.find(symbol);

        params.first = value.substr(0, pos);
        params.second = value.substr(pos + 1);
    }

    void FormatFunctions::Separator(std::string value, std::vector<std::string>& fragmentedValues, std::string symbol)
    {
        fragmentedValues.clear();
        size_t pos;

        do {
            pos = value.find(symbol);

            if (pos == -1)
                break;

            fragmentedValues.push_back(value.substr(0, pos));
            value = value.substr(pos + 1);
        } while (pos != -1);

        fragmentedValues.push_back(value);
    }



    std::string FormatFunctions::HexToBin(std::string str)
    {
        std::map <std::string, std::string> hexBin
        {
            {"0", "0000"},
            {"1", "0001"},
            {"2", "0010"},
            {"3", "0011"},
            {"4", "0100"},
            {"5", "0101"},
            {"6", "0110"},
            {"7", "0111"},
            {"8", "1000"},
            {"9", "1001"},
            {"A", "1010"},
            {"B", "1011"},
            {"C", "1100"},
            {"D", "1101"},
            {"E", "1110"},
            {"F", "1111"}
        };

        std::string binStr = "";

        for (int i = 0; i < str.size(); i++)
        {
            std::string temp = "";
            temp += toupper(str[i]);

            if (hexBin.count(temp))
            {
                binStr += hexBin.at(temp);
            }
        }

        return binStr;
    }

    int FormatFunctions::BinToDec(std::string str)
    {
        int value = 0;

        for (int i = str.size() - 1, j = 0; i >= 0; i--, j++)
        {
            int temp = str.c_str()[i] - '0';

            value += temp * pow(2, j);

        }

        return value;
    }

    int FormatFunctions::HexToDec(std::string str)
    {
        return BinToDec(HexToBin(str));
    }

    std::string FormatFunctions::HexToASCII(std::string str)
    {
        std::string outputString;
        
        for (int i = 0; i < str.length(); i += 2)
        {
            int intStr;
            std::stringstream ss(str.substr(i, 2));
            ss >> std::hex >> intStr;
            outputString += (char)intStr;
        }

        return outputString;
    }

    std::string FormatFunctions::ToUpper(std::string str) {
        std::string outString = str;
        transform(outString.begin(), outString.end(), outString.begin(), toupper);
        
        return outString;
    }

    std::string FormatFunctions::ToLower(std::string str) {
        std::string outString = str;
        transform(outString.begin(), outString.end(), outString.begin(), tolower);

        return outString;
    }

}