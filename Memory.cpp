#include "Memory.h"

void Memory::WriteFloat(float* address, float value)
{
	unsigned long OldProtection;
	VirtualProtect((LPVOID)(address), sizeof(float), PAGE_EXECUTE_READWRITE, &OldProtection);

	*address = value;

	VirtualProtect((LPVOID)(address), sizeof(float), OldProtection, _Post_ _Notnull_ NULL);
}

void Memory::WriteFloat(blackbone::ptr_t address, float value)
{
	WriteFloat(reinterpret_cast<float*>(address), value);
}