#pragma once

class Settings : public Singleton<Settings>
{
public:
    static void LoadSettings() noexcept;

    inline static bool logDebug{};

    inline static bool displayTimeElapsed{};
    inline static bool displayShipName{};
    inline static bool displayCharacterName{};
};
