#include "Memory.h"

void Memory::Write(BYTE* address, BYTE* bytes, int num)
{
	unsigned long OldProtection;
	VirtualProtect(address, num, PAGE_EXECUTE_READWRITE, &OldProtection);
	for (int i = 0; i < num; i++)
	{
		*(address + i) = *(bytes + i);
	}
	VirtualProtect(address, num, OldProtection, _Post_ _Notnull_ nullptr);
}

void Memory::Write(blackbone::ptr_t address, BYTE* bytes, int num)
{
	Write(reinterpret_cast<BYTE*>(address), bytes, num);
}

void Memory::WriteFloat(blackbone::ptr_t address, float value)
{
	Write(reinterpret_cast<BYTE*>(address), reinterpret_cast<BYTE*>(&value), sizeof(float));
}
