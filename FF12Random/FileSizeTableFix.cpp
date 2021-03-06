#include "stdafx.h"
#include "FileSizeTableFix.h"


FileSizeTableFix::FileSizeTableFix()
{
}


FileSizeTableFix::~FileSizeTableFix()
{
}

void FileSizeTableFix::load(string path)
{
	folder = path;

	if (!Helpers::fileExists(folder + "\\FileSizeTable_US_beforeRando.fst"))
	{
		std::experimental::filesystem::copy_file(folder + "\\FileSizeTable_US.fst", folder + "\\FileSizeTable_US_beforeRando.fst");
	}

	string fsSource = Helpers::mainPS2DataFolder.substr(0, Helpers::mainPS2DataFolder.size() - 7) + "FileSizeTable_US.fst";
	ifstream source(fsSource, ios::in | ios::binary | ios::ate);
	ofstream dest(folder + "\\FileSizeTable_US.fst", ios::in | ios::binary | ios::ate);
	dest << source.rdbuf();
	source.close();
	dest.close();
	remove(fsSource.c_str());
}

void FileSizeTableFix::save()
{
	string bpFileName = Helpers::mainPS2DataFolder + "\\image\\ff12\\test_battle\\" + Helpers::language + "\\binaryfile\\battle_pack.bin";
	string lhAFileName = Helpers::mainPS2DataFolder + "\\image\\ff12\\test_battle\\" + Helpers::language + "\\binaryfile\\listhelp_action.bin";
	string hAFileName = Helpers::mainPS2DataFolder + "\\image\\ff12\\test_battle\\" + Helpers::language + "\\binaryfile\\help_action.bin";
	string mcFileName = Helpers::mainPS2DataFolder + "\\image\\ff12\\test_battle\\" + Helpers::language + "\\binaryfile\\menu_command.bin";
	string mctFileName = Helpers::mainPS2DataFolder + "\\image\\ff12\\test_battle\\" + Helpers::language + "\\binaryfile\\menu_command_template.bin";
	string hmFileName = Helpers::mainPS2DataFolder + "\\image\\ff12\\test_battle\\" + Helpers::language + "\\binaryfile\\help_menu.bin";
	string m00FileName = Helpers::mainPS2DataFolder + "\\image\\ff12\\test_battle\\" + Helpers::language + "\\binaryfile\\menu00.bin";
	string mmFileName = Helpers::mainPS2DataFolder + "\\image\\ff12\\test_battle\\" + Helpers::language + "\\binaryfile\\menu_message.bin";
	overwriteSize(FileSizeTableOffset::battle_pack, getFileSize(bpFileName));
	overwriteSize(FileSizeTableOffset::listhelp_action, getFileSize(lhAFileName));
	overwriteSize(FileSizeTableOffset::help_action, getFileSize(hAFileName));
	overwriteSize(FileSizeTableOffset::menu_command, getFileSize(mcFileName));
	overwriteSize(FileSizeTableOffset::menu_command_template, getFileSize(mctFileName));
	overwriteSize(FileSizeTableOffset::help_menu, getFileSize(hmFileName));
	overwriteSize(FileSizeTableOffset::menu00, getFileSize(m00FileName));
	overwriteSize(FileSizeTableOffset::menu_message, getFileSize(mmFileName));
}

void FileSizeTableFix::process()
{

}

void FileSizeTableFix::overwriteSize(FileSizeTableOffset position, unsigned long value)
{
	union U {
		unsigned long i;
		unsigned char c[4];
	}byte;

	char * buffer;
	long size = 4;
	fstream file(folder + "\\FileSizeTable_US.fst", ios::out | ios::in | ios::binary | ios::ate);
	file.seekp(int(position));
	buffer = new char[size];

	buffer[0] = U{ value }.c[0];
	buffer[1] = U{ value }.c[1];
	buffer[2] = U{ value }.c[2];
	buffer[3] = U{ value }.c[3];

	file.write(buffer, size);
	file.close();

	delete[] buffer;

	//std::experimental::filesystem::copy_file(folder + "\\FileSizeTable_US.fst", folder + "\\FileSizeTable_US_beforeRando.fst");
}

unsigned long FileSizeTableFix::getFileSize(string filePath)
{
	ifstream file(filePath, ios::in | ios::binary | ios::ate);
	unsigned long size = file.tellg();
	file.close();
	return size;
}
