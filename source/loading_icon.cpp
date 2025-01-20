#include "loading_icon.hpp"
#include "data_access.hpp"
#include <stdio.h>

// OFFSET: 0x005503b0, STATUS: COMPLETE
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

// OFFSET: 0x005ed180, STATUS: WIP
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

// OFFSET: 0x005503e0, STATUS: WIP
void LoadingIcon::Enable()
{
	//unk9 = timeGetTime();
	unk4 = 1;
}

// OFFSET: 0x00550400, STATUS: COMPLETE
void LoadingIcon::Disable()
{
	unk4 = 0;
	unk9 = 0;
	unk7 = 0;
	unk8 = 0;
}

// OFFSET: 0x005d6780, STATUS: TODO
int LoadingIcon::Load(ParameterBlock*)
{
	return 0;
}
