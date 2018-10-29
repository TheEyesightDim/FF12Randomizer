// FF12Random.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <time.h>
#include <windows.h>
#include <cstdio>
#include <experimental/filesystem>
#include "LicenseRand.h"
#include "LicenseBoardRand.h"
#include "MagicRand.h"
#include "ItemRand.h"
#include "LootRand.h"
#include "EquipRand.h"
#include "ShopRand.h"
#include "CharRand.h"
#include "LicenseRename.h"

using namespace std;


int main(int argc, char* argv[])
{
	long seed = time(NULL);
	cout << "If you want to use a seed put that NUMBER here (Leave blank for random seed): "; 
	std::string inputSeed;
	getline(std::cin, inputSeed);
	if (!inputSeed.empty())
		seed = stol(inputSeed);
	srand(seed);
	cout << "The seed being used is: " << seed << endl;

	if (Helpers::getYesNo("Use default mod template (Choose n for your own)? (y/n) "))
	{
		std::experimental::filesystem::copy("data\\FF12RandomizedBase", "FF12Randomized" + to_string(seed), std::experimental::filesystem::copy_options::recursive);
		Helpers::mainPS2DataFolder = "\"FF12Randomized" + to_string(seed) + "\\ps2data\"";
		if (Helpers::mainPS2DataFolder.substr(0, 1) == "\"")
			Helpers::mainPS2DataFolder.replace(0, 1, "");
		if (Helpers::mainPS2DataFolder.substr(Helpers::mainPS2DataFolder.length() - 1, 1) == "\"")
			Helpers::mainPS2DataFolder.replace(Helpers::mainPS2DataFolder.length() - 1, 1, "");
	}
	else
	{
		cout << "Select the folder that contains the ps2data folder (this folder will be copied to a new folder)" << endl;
		string folder = Helpers::loadFile();
		std::experimental::filesystem::copy(folder, "FF12Randomized" + to_string(seed), std::experimental::filesystem::copy_options::recursive);
		Helpers::mainPS2DataFolder = "\"FF12Randomized" + to_string(seed) + "\\ps2data\"";
		if (Helpers::mainPS2DataFolder.substr(0, 1) == "\"")
			Helpers::mainPS2DataFolder.replace(0, 1, "");
		if (Helpers::mainPS2DataFolder.substr(Helpers::mainPS2DataFolder.length() - 1, 1) == "\"")
			Helpers::mainPS2DataFolder.replace(Helpers::mainPS2DataFolder.length() - 1, 1, "");
	}
	string bpFileName = Helpers::mainPS2DataFolder + "\\image\\ff12\\test_battle\\" + Helpers::language + "\\binaryfile\\battle_pack.bin";
	if (Helpers::fileExists(bpFileName))
	{
		srand(seed);
		ItemRand iRand{};
		iRand.load();
		if (Helpers::getYesNo("Would you like to randomize the item data (gil value only)? (y/n) "))
		{
			iRand.process();
			iRand.save();
		}

		srand(seed << 1);
		LootRand ltRand{};
		ltRand.load();
		if (Helpers::getYesNo("Would you like to randomize the loot (gil value only)? (y/n) "))
		{
			ltRand.process();
			ltRand.save();
		}

		srand(seed << 2);
		EquipRand eRand{};
		eRand.load();
		if (Helpers::getYesNo("Would you like to randomize the equipment data? (y/n) "))
		{
			eRand.process();
			eRand.save();
		}

		srand(seed << 3);
		MagicRand mRand{};
		mRand.load();
		if (Helpers::getYesNo("Would you like to randomize the magic/technick/enemy ability data? (y/n) "))
		{
			mRand.process();
			mRand.save();
		}

		srand(seed << 4);
		LicenseRand lRand{};
		lRand.load();
		if (Helpers::getYesNo("Would you like to randomize the licenses (not the board layouts, that's later)? (y/n) "))
		{
			lRand.process();
			lRand.save();
		}

		srand(seed << 5);
		ShopRand sRand{};
		sRand.load();
		if (Helpers::getYesNo("Would you like to randomize the shops? (y/n) "))
		{
			sRand.process();
			sRand.save();
		}

		srand(seed << 6);
		CharRand cRand{};
		cRand.load();
		if (Helpers::getYesNo("Would you like to randomize and update character data? (y/n) "))
		{
			cRand.process();
			cRand.save();
		}
	}
	else
		cout << "Battle_pack.bin not found for the " + Helpers::language + " language!" << endl;
	srand(seed << 7);
	LicenseBoardRand bRand{};
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
		if (Helpers::getYesNo("Would you like to randomize the license board layouts? (y/n) "))
		{
			bRand.load();
			bRand.process();
			bRand.save();
		}
	}
	else
		cout << "Board_X.bin files not found!" << endl;
	if (Helpers::fileExists(bpFileName))
	{
		srand(seed << 8);
		LicenseRename rename = LicenseRename();
		rename.load();
		if (Helpers::getYesNo("Would you like to rename the licenses to match their contents better and to provide more info? (y/n) "))
		{
			rename.process();
			rename.save();
		}
	}

	cout << "Randomization Complete!" << endl;
	cout << "The folder is called FF12Randomized" + to_string(seed) + " and can be found in the same folder as this program!" << endl;
	cout << "Load the mod using DrakLab." << endl;

	system("pause");

	return 0;
}

