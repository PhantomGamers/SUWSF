#include "Memory.h"

void Memory::WriteFloat(float* address, float value)
{
	unsigned long OldProtection;
	VirtualProtect(address, sizeof(float), PAGE_EXECUTE_READWRITE, &OldProtection);

	*address = value;

	VirtualProtect(address, sizeof(float), OldProtection, _Post_ _Notnull_ nullptr);
}

void Memory::WriteFloat(blackbone::ptr_t address, float value)
{
	WriteFloat(reinterpret_cast<float*>(address), value);
}
