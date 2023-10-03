#pragma once

class Settings : public Singleton<Settings>
{
public:
	static void LoadSettings() noexcept;

	// Display
	inline static bool bShowTimeElapsed{};
	inline static bool bShowShipName{};
	inline static bool bShowCharacterName{};
	inline static bool bShowCharacterLevel{};
	inline static bool bShowLocationInMenu{};
	inline static bool bShowPlanetWhileOutside{};

	// Log
	inline static bool bEnableDebug{};
};
