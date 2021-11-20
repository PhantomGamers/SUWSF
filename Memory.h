#pragma once
#include "external/Blackbone/src/BlackBone/Include/Types.h"
class Memory
{
public:
	static void WriteFloat(float* address, float value);
	static void WriteFloat(blackbone::ptr_t address, float value);
};