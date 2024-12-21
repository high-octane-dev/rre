#include "language_configuration.hpp"
#include "game.hpp"

// OFFSET: 0x00616ec0
LanguageConfiguration::LanguageConfiguration()
{
    Name[0] = '\0';
    TextLanguageIndex = 0;
    DialogueLanguageIndex = 0;
    Flag[0] = '\0';
    UseSubtitles = 0;
    IsDistanceImperial = 0;
    IsSpeedImperial = 0;
    TimerHundrethsMarkerFlag = '.';
}

// OFFSET: 0x006183f0
void LanguageConfiguration::Read(ParameterBlock* file)
{
    char language[32]{};
    char units[64]{};

    file->ReadParameterBlock(Name);
    file->GetParameter("Text", "Eng", language, 0x20);
    TextLanguageIndex = lpGame->GetLanguageDefinitionIndex(language);
    file->GetParameter("Dialogue", "Eng", language, 0x20);
    DialogueLanguageIndex = lpGame->GetLanguageDefinitionIndex(language);
    
    file->GetParameter("Flag", "", Flag, 0x20);
    file->GetParameter("UseSubtitles", 0, &UseSubtitles);

    file->GetParameter("UnitsOfMeasure", "metric", units, 0x40);
    IsDistanceImperial = 0;
    if (_stricmp(units, "metric") != 0) {
        if (_stricmp(units, "imperial") == 0) {
            IsDistanceImperial = 1;
        }
    }
    
    file->GetParameter("UnitsOfSpeed", "metric", units, 0x40);
    IsSpeedImperial = 0;
    if (_stricmp(units, "metric") != 0) {
        if (_stricmp(units, "imperial") == 0) {
            IsSpeedImperial = 1;
        }
    }

    file->GetParameter("TimerHundrethsMarker", "", units, 0x40);
    if (units[0] == '\0') {
        units[0] = (-(_stricmp(Name, "Spanish") != 0) & 0xf4U) + ':';
    }
    TimerHundrethsMarkerFlag = units[0];
}
