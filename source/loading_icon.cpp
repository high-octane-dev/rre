#include "loading_icon.hpp"
#include "data_access.hpp"
#include <stdio.h>

LoadingIcon::LoadingIcon()
{
	unk1 = 0;
	unk2 = 0;
	unk3 = 1;
	unk4 = 0;
	unk6 = 0;
	unk5 = 0;
	unk7 = 0;
	unk8 = 0;
	unk9 = 0;
}

void LoadingIcon::Create(char* a1)
{
	ParameterBlock local_960;
	char filename[260]{};

	snprintf(filename, sizeof(filename), "%s.%s.lsi", a1, "x360");
	if ((lpDataAccess->flags & 0x2000) != 0)
	{
		lpDataAccess->flags = lpDataAccess->flags & 0xffffdfff;
	}
	lpDataAccess->LoadResourceFile(filename, 1, 0, -1, 0, 0, 0, 0, 0, 0);

	if (local_960.OpenFile("icon_info.ini", 0, -1, NULL, -1) != 0)
	{
		if (Load(&local_960) != 0)
		{
			lpDataAccess->DropDevice(filename, 1);
		}
	}
}

void LoadingIcon::Enable()
{
	//unk9 = timeGetTime();
	unk4 = 1;
}

void LoadingIcon::Disable()
{
	unk4 = 0;
	unk9 = 0;
	unk7 = 0;
	unk8 = 0;
}

int LoadingIcon::Load(ParameterBlock*)
{
	return 0;
}
