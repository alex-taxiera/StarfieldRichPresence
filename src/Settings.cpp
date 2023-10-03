#include "Settings.h"

void Settings::LoadSettings() noexcept
{
    logger::info("Loading settings");

    CSimpleIniA ini;

    ini.SetUnicode();
    auto rc = ini.LoadFile(R"(.\Data\SFSE\Plugins\StarfieldRichPresence.ini)");
    if (rc < 0) {
        logger::error("ERROR: Failed to load ini");
        return;
    }
    assert(rc == SI_OK);

    bShowTimeElapsed        = ini.GetBoolValue("Display", "bShowTimeElapsed", true);
    bShowShipName           = ini.GetBoolValue("Display", "bShowShipName", true);
    bShowCharacterName      = ini.GetBoolValue("Display", "bShowCharacterName", true);
    bShowCharacterLevel     = ini.GetBoolValue("Display", "bShowCharacterLevel", true);
    bShowLocationInMenu     = ini.GetBoolValue("Display", "bShowLocationInMenu", true);
    bShowPlanetWhileOutside = ini.GetBoolValue("Display", "bShowPlanetWhileOutside", true);

    bEnableDebug = ini.GetBoolValue("Log", "bEnableDebug", false);

    if (bEnableDebug) {
        spdlog::get("Global")->set_level(spdlog::level::level_enum::debug);
        logger::debug("Debug logging enabled");
    }

    // Load settings (see simpleini readme: https://github.com/brofield/simpleini)

    logger::info("Loaded settings");
}
