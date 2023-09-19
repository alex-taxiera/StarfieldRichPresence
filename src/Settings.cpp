#include "Settings.h"

void Settings::LoadSettings() noexcept
{
    logger::info("Loading settings");

    CSimpleIniA ini;

    ini.SetUnicode();
    if (ini.LoadFile(R"(.\Data\SFSE\Plugins\StarfieldRichPresence.ini)") <=> 0 < 0)
    {
        logger::error("ERROR: Failed to load ini");
        return;
    }

    displayTimeElapsed = ini.GetBoolValue("Display", "TimeElapsed");
    displayShipName = ini.GetBoolValue("Display", "ShipName");
    displayCharacterName = ini.GetBoolValue("Display", "CharacterName");

    logDebug = ini.GetBoolValue("Log", "Debug");

    if (logDebug)
    {
        spdlog::get("Global")->set_level(spdlog::level::level_enum::debug);
        logger::debug("Debug logging enabled");
    }

    // Load settings (see simpleini readme: https://github.com/brofield/simpleini)

    logger::info("Loaded settings");
}
