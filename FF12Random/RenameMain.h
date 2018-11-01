#pragma once
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <Windows.h>
#include "Helpers.h"
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
#include "LicenseRename.h"
#include "BazaarRename.h"
#include "AugmentRename.h"

using namespace std;

class RenameMain
{
private:
	LicenseRename lRename;
	BazaarRename bRename;
	AugmentRename aRename;
public:
	static string abilityNames[545];
	static string lootNames[280];
	static string equipNames[420];
	RenameMain();
	~RenameMain();
	void load();
	void save();
	void process();
};

