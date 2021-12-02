#include "Memory.h"
#include <Windows.h>

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