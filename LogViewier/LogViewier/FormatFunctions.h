#pragma once

#ifndef FORMAT_FUNCTIONS_H
#define FORMAT_FUNCTIONS_H

#include <string>
#include <iostream>
#include <map>
#include <sstream>
#include <algorithm>

namespace formatfunctions {
	class FormatFunctions
	{
	public:
		static std::string EraseSymbol(std::string, char);
		static void Separator(std::string, std::string&, std::string&, std::string = "=");
		static std::string HexToBin(std::string);
		static int BinToDec(std::string);
		static int HexToDec(std::string);
		static std::string HexToASCII(std::string);
		static std::string ToUpper(std::string);
		static std::string ToLower(std::string);
	};
}

using ffunc = formatfunctions::FormatFunctions;

#endif //FORMAT_FUNCTIONS_H
