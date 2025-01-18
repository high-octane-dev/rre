#include "resource_setup.hpp"
#include "data_access.hpp"
#include "cars_game.hpp"

const char** UNK_0071626c = nullptr;
int UNK_00716270;

int ResourceSetup::UNK_0055de70(const char* param_1)
{
	int iVar2;
	if ((UNK_0071626c != nullptr) && (iVar2 = 0, 0 < UNK_00716270))
	{
		do
		{
			if (_stricmp(param_1, UNK_0071626c[iVar2]) == 0)
			{
				return 1;
			}
		} while (iVar2 < UNK_00716270);
	}
	return 0;
}

void ResourceSetup::UNK_005dd580(const char* param_1, int param_2)
{
	if (g_ResBuilding == 3)
	{
		if (UNK_0055de70(param_1) == 0)
		{
			return;
		}
	}
	else if ((g_BuildNewResourcesOnly != 0) && (lpDataAccess->FileExists((char*)param_1) != 0))
	{
		return;
	}
	if (param_2 != 0)
	{
		lpDataAccess->Initialize(8, 0xc0, 0x100000, 8, -1);
	}
	if ((lpDataAccess->flags & 0x2000) == 0)
	{
		lpDataAccess->flags = lpDataAccess->flags | 0x2000;
	}
}

void ResourceSetup::ResourceSetup(const char* path, int a2, int a3, int a4, int a5, unsigned int* a6, int a7, int a8, int a9)
{
	if (g_EnableNonStreamingResFiles != 0)
	{
		if ((g_EnableResourceBuilding != 0) && (g_ResBuilding != 0))
		{
			UNK_005dd580(path,a8);
		}
		if ((-2 < a2) && (0 < a2))
		{
			lpDataAccess->SetNumberOfDeviceCachePages(a4);
			lpDataAccess->ResizeDeviceCache(a2);
		}
		if ((lpDataAccess->LoadResourceFile((char*)path, a3, a6, a7, 0, 0, 0, 0, 0, a9) != 0) && (a5 != 0))
		{
			lpDataAccess->flags = lpDataAccess->flags | 0x1000;
		}
	}
}

void ResourceSetup::ResourceFinish(char* a1, int a2)
{
	if (g_EnableNonStreamingResFiles == 0)
	{
		return;
	}
	if ((g_EnableResourceBuilding != 0) && (g_ResBuilding != 0))
	{
		if (g_ResBuilding == 3)
		{
			if (UNK_0055de70(a1) == 0)
			{
				return;
			}
			if ((lpDataAccess->flags & 0x2000) != 0)
			{
				lpDataAccess->flags = lpDataAccess->flags & 0xffffdfff;
			}
			lpDataAccess->SaveResourceFile(a1, 1, 1, 0, 0, 0, 0, 1);
		}
		else
		{
			if ((g_BuildNewResourcesOnly != 0) && (lpDataAccess->FileExists(a1) != 0))
			{
				return;
			}
			if ((lpDataAccess->flags & 0x2000) != 0)
			{
				lpDataAccess->flags = lpDataAccess->flags & 0xffffdfff;
			}
			if (g_ResBuilding == 1)
			{
				lpDataAccess->SaveResourceFileList(a1, g_IniFileOutputDirectory);
			}
			else
			{
				if (g_ResBuilding != 2)
				{
					return;
				}
			}
		}
		lpDataAccess->Initialize(8, 0xc0, 0x100000, 8, -1);
	}
	lpDataAccess->DropDevice(a1, a2);
}
