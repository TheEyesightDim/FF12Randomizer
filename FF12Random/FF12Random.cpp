// FF12Random.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <time.h>
#include <windows.h>
#include <cstdio>
#include <functional>
#include <sstream>
#include <experimental/filesystem>
#include "FlagGroup.h"
#include "LicenseRand.h"
#include "LicenseBoardRand.h"
#include "MagicRand.h"
#include "ItemRand.h"
#include "EquipRand.h"
#include "ShopRand.h"
#include "CharRand.h"
#include "AugmentRand.h"
#include "RenameMain.h"
#include "FileSizeTableFix.h"
#include "TreasureRand.h"
#include "RewardRand.h"
#include "EnemyRand.h"
#include "CommandPackRand.h"
#include "NoChallengeRand.h"
#include "DocumentationGen.h"
#include "MusicRand.h"

using namespace std;

class Preset
{
public:
	string name, flags, description;
	Preset(string name, string flags, string description)
	{
		this->name = name;
		this->flags = flags;
		this->description = description;
	}
};

// trim from start (in place)
static inline void ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(),
		std::not1(std::ptr_fun<int, int>(std::isspace))));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(),
		std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
	ltrim(s);
	rtrim(s);
}

// trim from start (copying)
static inline std::string ltrim_copy(std::string s) {
	ltrim(s);
	return s;
}

// trim from end (copying)
static inline std::string rtrim_copy(std::string s) {
	rtrim(s);
	return s;
}

// trim from both ends (copying)
static inline std::string trim_copy(std::string s) {
	trim(s);
	return s;
}

vector<string> split(const std::string &s, char delim) {
	vector<string> elems;
	stringstream ss(s);
	string str;
	while (getline(ss, str, delim)) {
		elems.push_back(str);
	}
	return elems;
}
/*
ITEM, 0
EQUIP, 1
ABILITY, 2
AUGMENT, 3
CHARACTER, 4
ENEMY, 5
SHOP, 6
TREASURE, 7
REWARD, 8
LICENSE, 9
LICENSEBOARD 10
MUSIC 14
*/

int main(int argc, char* argv[])
{

	cout << "Processing..." << endl;
	system("pause");
	//try {
	string inputs[7];

	//0-seed
	//1-flags
	//2-ps2data path
	//3-fst folder path
	//4-version
	//5-music pack path
	//6-music only
	string line;
	ifstream myfile("inputs.txt");
	if (myfile.is_open())
	{
		for (int i = 0; i < 7; i++)
		{
			getline(myfile, line);
			inputs[i] = line;
		}
		myfile.close();
	}

	size_t seed = time(NULL);
	if (!inputs[0].empty())
	{
		try
		{
			seed = stol(inputs[0]);
		}
		catch (const exception &e)
		{
			seed = hash<string>{}(inputs[0]);
		}
	}
	srand(seed);

	Helpers::rng.seed(seed);

	Helpers::version = inputs[4];

	vector<string> inputFlags = split(inputs[1], '$');
	inputFlags.erase(inputFlags.begin());

	vector<FlagGroup> inputGroups = vector<FlagGroup>();
	for (string flag : inputFlags)
	{
		inputGroups.push_back(FlagGroup(flag));
	}

	string newFolder = "FF12Randomized" + to_string(seed);
	if (Helpers::fileExists(newFolder))
	{
		int num = 0;
		do
		{
			num++;
			newFolder = "FF12Randomized" + to_string(seed) + " (" + to_string(num) + ")";
		} while (Helpers::fileExists(newFolder));
	}
	if (inputs[2].empty())
	{
		std::experimental::filesystem::copy("data\\FF12RandomizedBase", newFolder, std::experimental::filesystem::copy_options::recursive);
	}
	else
	{
		std::experimental::filesystem::copy(inputs[2], newFolder, std::experimental::filesystem::copy_options::recursive);
	}
	Helpers::mainPS2DataFolder = newFolder + "\\ps2data\"";
	if (Helpers::mainPS2DataFolder.substr(0, 1) == "\"")
		Helpers::mainPS2DataFolder.replace(0, 1, "");
	if (Helpers::mainPS2DataFolder.substr(Helpers::mainPS2DataFolder.length() - 1, 1) == "\"")
		Helpers::mainPS2DataFolder.replace(Helpers::mainPS2DataFolder.length() - 1, 1, "");
	string bpFileName = Helpers::mainPS2DataFolder + "\\image\\ff12\\test_battle\\" + Helpers::language + "\\binaryfile\\battle_pack.bin";


	if (inputs[6] != "true")
	{

		MagicRand mRand;
		ItemRand iRand;
		EquipRand eqRand;
		AugmentRand aRand;
		LicenseRand lRand;
		ShopRand sRand;
		CharRand cRand;
		RewardRand rRand;
		CommandPackRand cpRand;

		StatusValue::readFlags(StatusValue::onEquipWeights, inputGroups[11]);
		StatusValue::readFlags(StatusValue::onHitWeights, inputGroups[12]);
		StatusValue::readFlags(StatusValue::immuneWeights, inputGroups[13]);

		if (Helpers::fileExists(bpFileName))
		{
			mRand.load();
			mRand.process(inputGroups[2]);

			iRand.load();
			iRand.process(inputGroups[0]);

			eqRand.load();
			eqRand.process(inputGroups[1]);

			aRand.load();
			aRand.process(inputGroups[3]);

			lRand.load();
			lRand.process(inputGroups[9]);

			sRand.load();
			sRand.process(inputGroups[6]);

			cRand.load();
			cRand.process(inputGroups[4]);

			rRand.load();
			rRand.process(inputGroups[8]);

			cpRand.load();

		}
		LicenseBoardRand bRand;
		bool hasBoardData = true;
		for (int i = 1; i <= 12; i++)
		{
			if (!Helpers::fileExists(Helpers::mainPS2DataFolder + "\\image\\ff12\\test_battle\\in\\binaryfile\\board_" + to_string(i) + ".bin"))
			{
				hasBoardData = false;
				break;
			}
		}
		if (hasBoardData)
		{
			bRand.load();
			bRand.process(inputGroups[10]);
		}
		RenameMain rename;
		if (Helpers::fileExists(bpFileName))
		{
			rename.load();
			rename.process(inputs[0].empty() ? to_string(seed) : inputs[0], inputs[1], inputGroups[5].hasFlag("N"), inputGroups[10].hasFlag("N"));
		}

		EnemyRand eRand;
		eRand.load();
		eRand.process(inputGroups[5]);

		TreasureRand tRand;
		tRand.load();
		tRand.process(inputGroups[7]);

		NoChallengeRand ncRand;
		ncRand.process(inputGroups[1].hasFlag("A"), inputGroups[1].hasFlag("C"), inputGroups[0].hasFlag("I"), inputGroups[2].hasFlag("M"), inputGroups[2].hasFlag("T"), inputGroups[4].hasFlag("S"));


		if (Helpers::fileExists(bpFileName))
		{
			cpRand.process(inputGroups[5]);

			mRand.save();
			iRand.save();
			eqRand.save();
			aRand.save();
			lRand.save();
			sRand.save();
			cRand.save();
			rRand.save();
			cpRand.save();

			rename.save();

			DocumentationGen dGen(rename, newFolder, inputs[0].empty() ? to_string(seed) : inputs[0], inputs[1]);
			dGen.process();
			dGen.save();
		}
		if (hasBoardData)
			bRand.save();

		eRand.save();
		tRand.save();

		/*FileSizeTableFix fFix = FileSizeTableFix();
		fFix.load(inputs[3]);
		fFix.process();
		fFix.save();
		std::experimental::filesystem::copy_file(inputs[3] + "\\FileSizeTable_US.fst", newFolder + "\\FileSizeTable_US.fst");*/
	}
	else
	{
		std::experimental::filesystem::remove(newFolder + "\\FileSizeTable_US.fst");
		std::experimental::filesystem::remove_all(newFolder + "\\ps2data\\image");
		std::experimental::filesystem::remove_all(newFolder + "\\ps2data\\plan_master");
	}
	if (inputGroups[14].hasFlag("m"))
	{
		MusicRand muRand;
		muRand.load(split(inputs[5],'\t'), inputGroups[14]);
		muRand.process();
		muRand.save(inputGroups[14]);
	}
	{
		ofstream oFile;
		oFile.open(newFolder + "\\readme.txt");
		if (!inputs[0].empty())
			oFile << "The seed is: " + inputs[0] << endl;
		else
			oFile << "The seed is: " + to_string(seed) << endl;
		oFile << "The flags used are: " + inputs[1] << endl << endl;
		oFile << "For info on the bazaars, treasures, enemies or equipment, check the documentation.html file!" << endl;
		oFile.close();
	}
	{
		ofstream oFile;
		oFile.open(newFolder + "\\mod.ini");
		oFile << "author = Bartz24" << endl;
		oFile << "title = FF12 Randomized" << endl;
		oFile << "version = " + Helpers::version << endl;
		oFile.close();
	}
	{
		ofstream oFile;
		oFile.open(newFolder + "\\randomizer.ini");
		oFile.close();
	}
	{
		ofstream oFile;
		oFile.open("output.txt");
		oFile << newFolder;
		oFile.close();
	}
	/*}
	catch (...)
	{
		cerr << "ERROR" << endl;
		system("pause");
		throw;
	}*/

	return 0;
}
