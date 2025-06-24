#pragma once
#include "../../Includes.h"

class COffsets : public CSingleton<COffsets>
{
public:
	bool ReadOffsets( );

	std::unordered_map<std::string, std::unordered_map<std::string, int>> clientOffsets;
	std::unordered_map<std::string, std::unordered_map<std::string, uint32_t>> baseOffsets;
};

#define m_pOffsets COffsets::GetInstance()