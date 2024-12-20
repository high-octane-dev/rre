#include "cars_game.hpp"

CarsGame* lpCarsGame = new CarsGame();

int CarsGame::InitializeRenderer(const char*)
{
    return 0;
}

int CarsGame::Initialize()
{
    return 0;
}

int CarsGame::PreGameInitialize(DisplayMode*)
{
    return 0;
}

void CarsGame::LoadConfigFile(ParameterBlock*)
{
}

void CarsGame::BeginAutoTest()
{
}

void CarsGame::EndAutoTest(int)
{
}

int CarsGame::Tick()
{
    return 0;
}

int CarsGame::KeyUpHandler(ProcessNode*, KeyInfo*)
{
    return 0;
}

int CarsGame::KeyDownHandler(ProcessNode*, KeyInfo*)
{
    return 0;
}

int CarsGame::Terminate()
{
    return 0;
}

void CarsGame::ReportMemoryUsage(char*)
{
}

void CarsGame::UpdateLocalizedPaths()
{
}

void CarsGame::UpdateTextureContentDirectories()
{
}

void CarsGame::ReloadMaterials()
{
}

void CarsGame::ReloadMaterialsAndTextures()
{
}

void CarsGame::PresentFrame(int)
{
}
