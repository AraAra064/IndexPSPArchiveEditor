#include <iostream>
#include <string>
#include <vector>
#include <direct.h>

#include "IndexPSP/AssetArchive.hpp"
#include "IndexLoader.hpp"
#include "StringParser.hpp"

namespace CLI
{
	std::string ReturnArgString(void)
	{
		std::string argString = "Available commands:";
		argString += "\n\nBatch Replace - Replaces multiple files contained within the Asset Archive";
		argString += "\nipae.exe BR AA_IN LISTNAME";// (AA_OUT)"; //AA_OUT
		argString += "\n[*]AA_IN = Asset Archive to be edited";
		argString += "\nLISTNAME = List to edit AA from";
		//argString += "\n(AA_OUT) = Name to save custom asset archive as (optional)";
		argString += "\n\n[*]If editing GRP.BIN, enter AA_IN should be GRP, DAT.BIN then DAT, etc";
		argString += "\nMore info can be found in README.txt";

		//Extract all
		argString += "\n\nExtract All - Extract all files from Asset Archive, saves extracted files to \".SavedFiles\" directory";
		argString += "\nipae.exe EA AA_IN"; // FOLDER_OUT";
		argString += "\nAA_IN = Asset Archive to be extracted";
		//argString += "\nFOLDER_OUT = Folder to save as";

		argString += "\n\nShows list of available commands";
		argString += "\nipae.exe ?";

		return argString;
	}

	std::string ReturnHelpString(void)
	{
		std::string s = "For help with commands type \"ipae.exe ?\".";
		return s;
	}

	int CommandLine(int nArgs, const char** arg)
	{
		std::vector<std::string> archiveNames = { "GRP", "DAT", "EVT", "ADV", "FARC", "PSPSND", "PSPBGM" };
		std::vector<bool> encodedArchives = { true, true, true, false, false, false };

		std::string type = arg[1];
		type = StringParser::ToUpper(type);

		if (nArgs >= 4 && type == "BR")
		{
			std::string aaIn = StringParser::ToUpper(arg[2]), listName = arg[3];
			listName = "Lists\\" + listName;
			std::string aaOut = aaIn;//(nArgs > 3 ? arg[3] : aaIn);

			//stuff
			int i = 0;
			for (auto s : archiveNames)
			{
				if (s == aaIn) {
					break;
				}
				i++;
			}
			aaIn = "OriginalArchives\\" + aaIn + ".BIN";
			aaOut = "CustomArchives\\" + aaOut + ".BIN";

			auto archiveData = LoadFile(aaIn);
			if (archiveData.empty())
			{
				std::cout << "File cannot be read or is not inputted correctly." << std::endl;
				std::cout << ReturnHelpString() << std::endl;

				return 2;
			}

			auto fileInfo = AssetArchive::GetFileInfo(archiveData);
			std::deque<std::vector<uint8_t>> newFiles(fileInfo.size());
			std::vector<std::string> listCom;//(fileInfo.size());
			listCom = IndexLoader::LoadList(archiveData, newFiles, fileInfo, encodedArchives[i], listName);

			if (listCom.empty())
			{
				std::cout << "List file can not be read." << std::endl;

				return 3;
			}

			bool e = AssetArchive::SaveArchive(archiveData, fileInfo, newFiles, aaOut, 3 | (encodedArchives[i] ? AssetArchive::IsEncoded : 0));

			if (!e)
			{
				std::cout << "An error occured while saving the asset archive." << std::endl;

				return 4;
			}
			else std::cout << "Successfully created \"" << aaOut << "\"." << std::endl;

			return 0;
		}

		if (nArgs >= 3 && type == "EA")
		{
			std::string aaIn = StringParser::ToUpper(arg[2]), folderName;

			int index = 0;
			for (auto s : archiveNames)
			{
				if (s == aaIn) {
					break;
				}
				index++;
			}
			folderName = ".SavedFiles\\" + aaIn;
			aaIn += ".BIN";
			aaIn = "OriginalArchives\\" + aaIn;
			

			auto archiveData = LoadFile(aaIn);
			if (archiveData.empty())
			{
				std::cout << "File cannot be read or is not inputted correctly." << std::endl;
				std::cout << ReturnHelpString() << std::endl;

				return 2;
			}

			if (_mkdir(folderName.c_str()) == -1)
			{
				std::cout << "Folder can not be created (errno=" << errno << ")." << std::endl;

				return 5;
			}

			auto fileInfo = AssetArchive::GetFileInfo(archiveData);

			uint32_t n = AssetArchive::GetNumberOfFiles(archiveData);
			for (uint32_t i = 0; i < n; i++)
			{
				auto fileData = AssetArchive::GetFileData(archiveData, fileInfo, i, encodedArchives[index]);
				std::string ext = IndexLoader::GetFileExtStr(fileData);
				std::string filePath = "File" + std::to_string(i) + ".";
				filePath = folderName + '\\' + filePath;

				const char* sht = "SHTX";
				//sht -> png
				if (*(uint32_t*)&fileData[0] == *(uint32_t*)sht)
				{
					filePath += "png";
					uint32_t w = 0, h = 0;
					auto pixData = IndexSHTXPS::GetPixelData(fileData, w, h, true);
					
					if (lodepng::encode(filePath, (uint8_t*)pixData.data(), w, h) == 0)
					{
						std::cout << "Saved \"" << filePath << "\"" << std::endl;
					} 
					else std::cout << "Could not save \"" << filePath << "\"" << std::endl;

					continue;
				}

				filePath += ext;
				if (MakeFile(filePath, fileData))
				{
					std::cout << "Saved \"" << filePath << "\"" << std::endl;
				} 
				else std::cout << "Could not save \"" << filePath << "\"" << std::endl;
			}

			return 0;
		}

		if (type == "?")
		{
			std::cout << ReturnArgString() << std::endl;
			std::cout << "Available AAs for AA_IN:" << std::endl;
			for (auto s : archiveNames)
			{
				std::cout << s << std::endl;
			}

			return 0;
		}

		std::cout << "\"" << type << "\" command not found or is not inputted correctly." << std::endl;
		std::cout << ReturnHelpString() << std::endl;
		return 1;
	}
}