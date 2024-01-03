#include "Settings.h"

void Settings::LoadSettings() noexcept
{
	CSimpleIniA ini;

	logger::info("Loading settings");

	ini.SetUnicode();
	auto rc = ini.LoadFile(R"(.\Data\SFSE\Plugins\StarfieldRichPresence.ini)");
	if (rc < 0) {
		logger::error("ERROR: Failed to load ini");
		return;
	}
	assert(rc == SI_OK);

	// Display
	bShowCharacterLevel =
		ini.GetBoolValue("Display", "bShowCharacterLevel", true);
	bShowCharacterName =
		ini.GetBoolValue("Display", "bShowCharacterName", true);
	bShowLocationInMenu =
		ini.GetBoolValue("Display", "bShowLocationInMenu", true);
	bShowShipName = ini.GetBoolValue("Display", "bShowShipName", true);
	bShowTimeElapsed = ini.GetBoolValue("Display", "bShowTimeElapsed", true);

	// Log
	bEnableDebug = ini.GetBoolValue("Log", "bEnableDebug", false);

	if (bEnableDebug) {
		spdlog::get("Global")->set_level(spdlog::level::level_enum::debug);
		logger::debug("Debug logging enabled");
	}

	logger::info("Loaded settings");
}
