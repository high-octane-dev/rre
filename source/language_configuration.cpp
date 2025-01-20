#include "language_configuration.hpp"
#include "game.hpp"

// OFFSET: 0x00616ec0, STATUS: COMPLETE
LanguageConfiguration::LanguageConfiguration() {
    name[0] = '\0';
    text_language_index = 0;
    dialogue_language_index = 0;
    flag[0] = '\0';
    use_subtitles = 0;
    is_distance_imperial = 0;
    is_speed_imperial = 0;
    timer_hundreths_marker_flag = '.';
}

// OFFSET: 0x006183f0, STATUS: COMPLETE
void LanguageConfiguration::Read(ParameterBlock* file) {
    char language[32]{};
    char units[64]{};

    file->ReadParameterBlock(name);
    file->GetParameter("Text", "Eng", language, 0x20);
    text_language_index = lpGame->GetLanguageDefinitionIndex(language);
    file->GetParameter("Dialogue", "Eng", language, 0x20);
    dialogue_language_index = lpGame->GetLanguageDefinitionIndex(language);
    
    file->GetParameter("Flag", "", flag, 0x20);
    file->GetParameter("UseSubtitles", 0, &use_subtitles);

    file->GetParameter("UnitsOfMeasure", "metric", units, 0x40);
    is_distance_imperial = 0;
    if (_stricmp(units, "metric") != 0) {
        if (_stricmp(units, "imperial") == 0) {
            is_distance_imperial = 1;
        }
    }
    
    file->GetParameter("UnitsOfSpeed", "metric", units, 0x40);
    is_speed_imperial = 0;
    if (_stricmp(units, "metric") != 0) {
        if (_stricmp(units, "imperial") == 0) {
            is_speed_imperial = 1;
        }
    }

    file->GetParameter("TimerHundrethsMarker", "", units, 0x40);
    if (units[0] == '\0') {
        units[0] = (-(_stricmp(name, "Spanish") != 0) & 0xf4U) + ':';
    }
    timer_hundreths_marker_flag = units[0];
}
