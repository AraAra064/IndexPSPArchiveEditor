#include <iostream>
#include <fstream>
#include <string>

#include "ConsoleGraphics.hpp"
#include "CLI.hpp"

int main(uint32_t nArgs, const char** arg)
{
	//ipae.exe CREATEAA AA_IN LISTNAME (AA_OUT??)
	//ipae.exe EXTRACT AA_IN FILEINDEX (FILE_OUT??)
	//ipae.exe EXTRACTALL AA_IN LISTNAME (AA_OUT??)
	//ipae.exe GRP_ENG AltTextureModB.txt (AA_OUT??/GRP.bin)

	//cg::ConsoleGraphics* graphics = new cg::ConsoleGraphics(1920, 1080, true, 3, true);
	//graphics->enableAlpha();
	//graphics->setTitle("IndexPSPArchiveEditor");

	if (nArgs > 1)
	{
		return CLI::CommandLine(nArgs, arg);
	}

	std::cout << "No GUI. Type \"ipae.exe ?\" to for help." << std::endl;

	return 0;
}
