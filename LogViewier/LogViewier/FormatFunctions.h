#pragma once

#ifndef FORMAT_FUNCTIONS_H_
#define FORMAT_FUNCTIONS_H_

#include <string>
#include <iostream>
#include <map>

namespace formatfunctions {
	class FormatFunctions
	{
	public:
		static std::string EraseSymbol(std::string, char);
		static void Separator(std::string, std::string*, std::string*, std::string = "=");
		static std::string HexToBin(std::string);
		static int BinToDec(std::string str);
	};
}

#endif
