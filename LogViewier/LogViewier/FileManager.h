#pragma once

#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <iostream>
#include <filesystem>
#include <string.h>
#include <string>
#include <vector>

#include "FormatFunctions.h"

//namespace ffunc = formatfunctions;
namespace fs = std::filesystem;

namespace filemanager {
	class FileManager
	{
	private:
		FileManager();
	public:
		static void Dir(fs::path&);
		static void Cd(fs::path&, std::string);
		static bool Type(fs::path&, fs::path&, std::string);
		static void Help();
		static fs::path FileManagerFunc(fs::path&);

	};
}


#endif //FILE_MANAGER_H

