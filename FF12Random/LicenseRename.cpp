#include "stdafx.h"
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
#include "LicenseRename.h"

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

LicenseRename::LicenseRename()
{
}


LicenseRename::~LicenseRename()
{
}

void LicenseRename::load()
{
	bpFileName = Helpers::mainPS2DataFolder + "\\image\\ff12\\test_battle\\" + Helpers::language + "\\binaryfile\\battle_pack.bin";

	string line;
	ifstream myfile("data\\licenseNames.txt");
	if (myfile.is_open())
	{
		for (int i = 0; i < 362; i++)
		{
			getline(myfile, line);
			data[i] = line;
		}
		myfile.close();
	}

	myfile = ifstream("data\\licenseSuffixes.txt");
	if (myfile.is_open())
	{
		for (int i = 0; i < 362; i++)
		{
			getline(myfile, line);
			suffix[i] = line;
		}
		myfile.close();
	}

	myfile = ifstream("data\\equipAbilityNames.txt");
	if (myfile.is_open())
	{
		while(getline(myfile, line))
		{
			string id = line.substr(0, line.find(','));
			trim(id);
			int numID = stoi(id);
			string name = line.substr(line.find(',') + 1, line.length());
			name.erase(
				remove(name.begin(), name.end(), '\"'),
				name.end()
			);
			trim(name);
			names.insert(pair <int, string>(numID, name));
		}
		myfile.close();
	}
}

void LicenseRename::save()
{
	ofstream myfile;
	myfile.open("temp.txt");
	for (int i = 0; i < 362; i++)
	{
		myfile << data[i];
		if (i < 361)
			myfile << endl;
	}
	myfile.close();

	HMODULE hModule = GetModuleHandleW(NULL);
	WCHAR path[MAX_PATH];
	GetModuleFileNameW(hModule, path, MAX_PATH);
	wstring ws(path);
	string strPath(ws.begin(), ws.end());
	strPath = strPath.substr(0, strPath.find_last_of("\\/"));
	string s = "\"" + bpFileName + "\" \"" + strPath + "\\temp.txt\" \"" + bpFileName + "\"";
	system(("LicenseNamePatcher.exe " + s).c_str());

	remove("temp.txt");
}

string LicenseRename::getNameFromID(int id)
{
	if (id < 100)
		id += 12288;
	else if (id < 4000)
		id += 16226;
	map<int, string>::iterator it = names.find(id);
	if (it != names.end())
	{
		return names[id];
	}
	return "";
}

string LicenseRename::getSameGroup(vector<int> nums)
{
	bool allSame = true;
	for (int i = 1; i < nums.size(); i++)
	{
		if (getGroup(nums[0]) != getGroup(nums[i]))
		{
			allSame = false;
			break;
		}
	}
	if (allSame)
		return getGroup(nums[0]);
	if (allMeetCondition(nums, 1, 4000))
	{
		if (allMeetCondition(nums, 3, 80))
		{
			return "Magick";
		}
		return "Abilities";
	}
	else
	{
		if (allMeetCondition(nums, 3, 4296))
		{
			return "Weapons";
		}
		else if (allMeetCondition(nums, 4, 4296) && allMeetCondition(nums, 3, 4435))
		{
			return "Armor";
		}
		return "Equipment";
	}

	return false;
}

bool LicenseRename::allMeetCondition(vector<int> nums, int type, int cond)
{
	for (int i = 0; i < nums.size(); i++)
	{
		switch (type)
		{
		case 0:
			if (nums[i] != cond) //equal
				return false;
			break;
		case 1:
			if (nums[i] >= cond) //less
				return false;
			break;
		case 2:
			if (nums[i] <= cond) //greater
				return false;
			break;
		case 3:
			if (nums[i] > cond) //less or equal
				return false;
			break;
		case 4:
			if (nums[i] < cond) //greater or equal
				return false;
			break;
		}
	}
	return true;
}

string LicenseRename::getGroup(int a)
{
	//Magick/Technick
	if (a < 4000)
	{
		if (a >= 0 && a <= 17 || a >= 51 && a <= 56 || a == 69)
			return "White Magick";
		else if (a >= 18 && a <= 33 || a >= 59 && a <= 65 || a == 35 || a == 37)
			return "Black Magick";
		else if (a >= 36 && a <= 50 || a == 67 || a == 71 || a == 72 || a == 79 || a == 80)
			return "Time Magick";
		else if (a >= 73 && a <= 75 || a == 58 || a == 66 || a == 70)
			return "Green Magick";
		else if (a >= 76 && a <= 78 || a == 34 || a == 68)
			return "Arcane Magick";
		else if (a >= 158 && a <= 181)
			return "Technicks";
	}
	else
	{
		if (a >= 4097 && a <= 4113 || a >= 4260 && a <= 4264 || a == 4256 || a == 4257 || a == 4273 || a == 4288 || a == 4290)
			return "Swords";
		else if (a >= 4114 && a <= 4120 || a == 4259 || a == 4259 || a == 4262 || a == 4265 || a == 4269 || a == 4289)
			return "Greatswords";
		else if (a >= 4121 && a <= 4130 || a == 4266)
			return "Katanas";
		else if (a >= 4131 && a <= 4137 || a == 4267)
			return "Ninja Swords";
		else if (a >= 4138 && a <= 4149 || a == 4270)
			return "Spears";
		else if (a >= 4150 && a <= 4161 || a == 4294)
			return "Poles";
		else if (a >= 4162 && a <= 4175 || a == 4274 || a == 4295)
			return "Bows";
		else if (a >= 4177 && a <= 4182 || a == 4272)
			return "Crossbows";
		else if (a >= 4183 && a <= 4193 || a == 4293)
			return "Guns";
		else if (a >= 4194 && a <= 4205 || a == 4292)
			return "Axes & Hammers";
		else if (a >= 4206 && a <= 4216 || a == 4268)
			return "Daggers";
		else if (a >= 4217 && a <= 4224)
			return "Rods";
		else if (a >= 4225 && a <= 4234)
			return "Staves";
		else if (a >= 435 && a <= 4243 || a == 4255 || a == 4271)
			return "Maces";
		else if (a >= 4244 && a <= 4249)
			return "Measures";
		else if (a >= 4250 && a <= 4254 || a == 4291)
			return "Hand-bombs";
		else if (a >= 4297 && a <= 4315)
			return "Shields";
		else if (a >= 4358 && a <= 4375 || a >= 4418 && a <= 4435)
			return "Heavy Armor";
		else if (a >= 4316 && a <= 4336 || a >= 4376 && a <= 4396)
			return "Light Armor";
		else if (a >= 4337 && a <= 4357 || a >= 4397 && a <= 4417)
			return "Mystic Armor";
		else if (a >= 4436 && a <= 4483)
			return "Accessories";
	}
	return string();
}

void LicenseRename::process()
{
	for (int i = 0; i < 361; i++)
	{
		if (i >= 300 && i <= 328 || i >= 219 && i <= 275)
			continue;
		vector<int> idsContained = vector<int>();
		for (int slot = 0; slot < 8; slot++)
		{
			if (LicenseRand::licenseData[i].otherData[slot] != 0xFFFF)
				idsContained.push_back(LicenseRand::licenseData[i].otherData[slot]);
		}
		if (idsContained.size() == 1)
		{
			string name = getNameFromID(idsContained[0]);
			if (name != "")
				data[i] = name;
		}
		else if (idsContained.size() >= 2)
		{
			bool canChange = true;
			for (int i = 0; i < idsContained.size(); i++)
			{
				if (getNameFromID(idsContained[i]) == "")
					canChange = false;
			}
			if (canChange)
			{
				string name = getSameGroup(idsContained);
				map<string, int>::iterator it = nameCount.find(name);
				if (it == nameCount.end())
				{
					nameCount.insert(pair <string, int>(name, 1));
				}
				else
				{
					nameCount[name] = nameCount[name] + 1;
				}
				data[i] = name;
			}
		}
	}
	map<string, int> namesSeen;

	vector<int> licenseOrder = vector<int>();
	licenseOrder.push_back(0);
	for (int i = 1; i < 361; i++)
	{
		bool added = false;
		for (int i2 = 0; i2 < licenseOrder.size(); i2++)
		{
			if (LicenseRand::licenseData[i].lpCost < LicenseRand::licenseData[licenseOrder[i2]].lpCost)
			{
				licenseOrder.insert(licenseOrder.begin() + i2, i);
				added = true;
				break;
			}
		}
		if (!added)
			licenseOrder.push_back(i);
	}

	for (int i = 0; i < 361; i++)
	{
		map<string, int>::iterator it = nameCount.find(data[licenseOrder[i]]);
		if (it != nameCount.end())
		{
			map<string, int>::iterator it2 = namesSeen.find(data[licenseOrder[i]]);
			if (it2 == namesSeen.end())
			{
				namesSeen.insert(pair <string, int>(data[licenseOrder[i]], 1));
			}
			else
			{
				namesSeen[data[licenseOrder[i]]] = namesSeen[data[licenseOrder[i]]] + 1;
			}
			data[licenseOrder[i]] = data[licenseOrder[i]] + " " + to_string(namesSeen[data[licenseOrder[i]]]);
		}
		if (suffix[licenseOrder[i]] != "")
			data[licenseOrder[i]] = data[licenseOrder[i]] + " " + suffix[licenseOrder[i]];
		data[licenseOrder[i]] = data[licenseOrder[i]] + " (" + to_string(LicenseRand::licenseData[licenseOrder[i]].lpCost) + " LP)";
	}
	for (int i = 18; i < 361; i++)
	{
		vector<string> boardsWithLicense = vector<string>();

		string names[12] = { "Wh", "Uh" , "Ma" , "Re" , "Kn" ,"Mo", "Ti" , "Fo" , "Ar" , "Bl" , "Bu" , "Sh" };

		for (int board = 0; board < 12; board++)
		{
			for (int y = 0; y < 24; y++)
			{
				for (int x = 0; x < 24; x++)
				{
					if (LicenseBoardRand::boards[board].board[y][x] == i)
						boardsWithLicense.push_back(names[board]);
				}
			}
		}
		data[i] = data[i] + " (";
		if (boardsWithLicense.size() == 0)
			data[i] = data[i] + "None";
		else if (boardsWithLicense.size() == 12)
			data[i] = data[i] + "All";
		else
		{
			for (int name = 0; name < boardsWithLicense.size(); name++)
			{
				data[i] = data[i] + boardsWithLicense[name];
			}
		}
		data[i] = data[i] + ")";
	}
}
