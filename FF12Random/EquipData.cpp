#include "stdafx.h"
#include "EquipData.h"


EquipDataOffset EquipData::getDataIndex()
{
	string language = Helpers::language;
	if (language == "us")
		return EquipDataOffset::us;
	return EquipDataOffset::us;
}

EquipData::EquipData(char data[52])
{
	int index = 0;
	for (int i = 0; i < 52; i++)
	{
		if (i >= 0x11 && i <= 0x13 || i == 0x1A || i >= 0x1E && i <= 0x23 || i == 0x07 || i >= 0x27 && i <= 0x2B)
			continue;
		this->unknown[index] = data[i];
		index++;
	}
	this->equipRequirements = data[0x11];
	char cost[] = { data[0x12] , data[0x13] };
	this->cost = *reinterpret_cast<unsigned short*>(cost);
	this->power = data[0x1A];
	this->element = data[0x1E];
	this->hitChance = data[0x1F];
	this->status1 = data[0x20];
	this->status2 = data[0x21];
	this->status3 = data[0x22];
	this->status4 = data[0x23];
	this->ct = data[0x27];
	char attr[] = { data[0x28] , data[0x29], data[0x2A], data[0x2B] };
	this->attribute = *reinterpret_cast<unsigned int*>(attr);
	this->itemFlag = data[0x07];
}

EquipData::EquipData()
{
}


EquipData::~EquipData()
{
}
