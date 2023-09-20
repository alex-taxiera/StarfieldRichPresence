#pragma once

class Settings : public Singleton<Settings>
{
public:
    static void LoadSettings() noexcept;

    inline static bool bEnableDebug{};

    inline static bool bShowTimeElapsed{};
    inline static bool bShowShipName{};
    inline static bool bShowCharacterName{};
    inline static bool bShowCharacterLevel{};
    inline static bool bShowLocationInMenu{};
};
