#include "cars_game.hpp"

CarsGame* lpCarsGame = new CarsGame();

// OFFSET: 0x004f50a0
int CarsGame::InitializeRenderer(const char*)
{
    return 0;
}

// OFFSET: 0x0051c130
int CarsGame::Initialize()
{
    return 0;
}

// OFFSET: 0x0043fe80
int CarsGame::PreGameInitialize(DisplayMode*)
{
    X360Game::PreGameInitialize(nullptr);
    /*
    _timeGameInit = timeGetTime();
    RSStringUtil::Ssnprintf(%sDebugStr.mst,0x104,"%sDebugStr.mst",s_C\Debug\_006f4970);
    (*((lpMaterialStringTable->StringTable).vtable)->Load)(%sDebugStr.mst);
    RSStringUtil::Ssnprintf(%s%sMaterialTemplate.%s.res,0x104,"%s%sMaterialTemplate.%s.res",s_Res\Data\_006f73d8,s_C\AppStart\_006f6da8,"x360");
    bVar2 = g_ResBuilding == 0;
    if (bVar2) {
        ResourceSetup::ResourceSetup(%s%sMaterialTemplate.%s.res,0xffffffff,1,8,0,0,-1,1,0);
    }
    RSStringUtil::Ssnprintf(&stack0xfffffdf0,0x104,"%sDfltMT",s_C\AppStart\_006f6da8);
    X360MaterialTemplate::LoadFromFile(lpSceneObjectMaterialTemplate,&stack0xfffffdf0);
    RSStringUtil::Ssnprintf(&stack0xfffffdf0,0x104,"%sIconMT",s_C\AppStart\_006f6da8);
    X360MaterialTemplate::LoadFromFile(lpIconMaterialTemplate,&stack0xfffffdf0);
    if (bVar2) {
        ResourceSetup::ResourceFinish(%s%sMaterialTemplate.%s.res,1);
    }
    */
    return 1;
}

// OFFSET: 0x00440360
void CarsGame::LoadConfigFile(ParameterBlock*)
{
}

// OFFSET: 0x00440e00
void CarsGame::BeginAutoTest()
{
}

// OFFSET: 0x004412b0
void CarsGame::EndAutoTest(int)
{
}

// OFFSET: 0x004f51d0
int CarsGame::Tick()
{
    return 0;
}

// OFFSET: 0x00440320
int CarsGame::KeyUpHandler(ProcessNode*, KeyInfo*)
{
    return 0;
}

// OFFSET: 0x00440340
int CarsGame::KeyDownHandler(ProcessNode*, KeyInfo*)
{
    return 0;
}

// OFFSET: 0x0051c730
int CarsGame::Terminate()
{
    return 0;
}

// OFFSET: 0x00441b70
void CarsGame::ReportMemoryUsage(char*)
{
}

// OFFSET: 0x00487b80
void CarsGame::UpdateLocalizedPaths()
{
}

// OFFSET: 0x00441d80
void CarsGame::UpdateTextureContentDirectories()
{
}

// OFFSET: 0x00441840
void CarsGame::ReloadMaterials()
{
    Game::ReloadMaterials(0);
}

// OFFSET: 0x00441850
void CarsGame::ReloadMaterialsAndTextures()
{
    Game::ReloadMaterialsAndTextures(0);
}

// OFFSET: 0x004237a0
void CarsGame::PresentFrame(int)
{
}
