#pragma once

class Settings : public Singleton<Settings>
{
public:
	static void LoadSettings() noexcept;

	// Display
	inline static bool bShowCharacterLevel{};
	inline static bool bShowCharacterName{};
	inline static bool bShowLocationInMenu{};
	inline static bool bShowShipName{};
	inline static bool bShowTimeElapsed{};

	// Log
	inline static bool bEnableDebug{};
};
