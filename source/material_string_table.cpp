#include "material_string_table.hpp"
#include "util/rsstring_util.hpp"
#include "cars_game.hpp"

void MaterialStringTable::ModifyFilename(char* a1)
{
	char local_18[20];
	RSStringUtil::Ssnprintf(local_18, sizeof(local_18), ".%s.mst", "x360");
	strcat(a1, local_18);
}

int MaterialStringTable::GetVariableValue(char* a1, char* a2)
{
	char* pcVar1;
	char cVar2;

	if (_stricmp(a2, "FontTextureContentDirectory") == 0)
	{
		strncpy(a1, g_FontTextureContentDirectory, sizeof(g_FontTextureContentDirectory));
	}
	else
	{
		if (_stricmp(a2, "UITextureContentDirectory") == 0)
		{
			strncpy(a1, g_UITextureContentDirectory, sizeof(g_UITextureContentDirectory));
		}
		else
		{
			if (_stricmp(a2, "UILocalizedTextureContentDirectory") == 0)
			{
				strncpy(a1, g_UILocalizedTextureContentDirectory, sizeof(g_UILocalizedTextureContentDirectory));
			}
			else
			{
				*a1 = '\0';
			}
		}
	}
	pcVar1 = a1 + 1;
	do {
		cVar2 = *a1;
		a1 = a1 + 1;
	} while (cVar2 != '\0');
	return a1 - pcVar1;
}
