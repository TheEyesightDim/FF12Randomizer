#pragma once
#include <string>

using namespace std;

class RandSpellData
{
public:
	RandSpellData();
	~RandSpellData();
	string name;
	string description;
	int weight;
	int magicType;
	int power;
	int ct;
	int mp;
	int accuracy;
	int aoe;
	int onHit;
	int target;
	int effect;
	unsigned int status;
	int element;
	int mType2;
	int animation;
	int specialType;
	int icon;
	int castAnimation;
	int enemyRarity;
};

