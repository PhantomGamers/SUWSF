#pragma once
#include "../external/Blackbone/src/BlackBone/Include/Types.h"

class Memory
{
public:
	static void Write(BYTE* address, BYTE* bytes, int num);
	static void WriteFloat(blackbone::ptr_t address, float value);
	static void Write(blackbone::ptr_t address, BYTE* bytes, int num);
};
