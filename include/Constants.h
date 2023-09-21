#if !defined(MYLIB_CONSTANTS_H)
#define MYLIB_CONSTANTS_H 1

namespace Constants
{
    const std::string discordAppId = "1151867904082587709";
    const float updateDebounceTime = 3000;
}


namespace Text
{
    static constexpr const char* LaunchingGame = "Launching Game";
    static constexpr const char* InMainMenu    = "In Main Menu";
    static constexpr const char* Exploring     = "Exploring";
    static constexpr const char* Fighting      = "Fighting";
    static constexpr const char* InSpace       = "In Space";
    static constexpr const char* InSpaceship   = "In Spaceship";
    static constexpr const char* Level         = "Lv";
    static constexpr const char* In            = "in";
    static constexpr const char* On            = "on";
    static constexpr const char* At            = "at";
};

namespace Resources
{
    static constexpr const char* StarfieldLogo = "sf_logo";
};

namespace UIMenu
{
    struct MenuEntry
    {
        const char* menuName           = NULL;
        const char* menuText           = "";
        bool        shouldShowLocation = true;
        bool        shouldShowDetails  = true;
    };

    static MenuEntry menuEntries[16] = {
        { "LoadingMenu" }, // no menu text because it's only shown for a few seconds
        { "MainMenu", "In Main Menu", false, false },
        { "PauseMenu", "Paused", false }, // System Menu
        { "SleepWaitMenu", "Sleeping" },
        { "PickpocketMenu", "Pickpocketing" },
        { "LockpickingMenu", "Lockpicking" },
        { "SkillsMenu", "Checking perks" },
        { "GalaxyStarMapMenu", "Gazing at starmap", false },
        { "PhotoModeMenu", "Taking a photo" },
        { "PhotoGalleryMenu", "Looking at photos" },
        { "BSMissionMenu", "Looking at the quest log" },
        { "PowersMenu", "Looking at powers" },
        { "InventoryMenu", "Checking inventory" },
        { "ChargenMenu", "Creating a character", false },
        { "SpaceshipEditorMenu", "Looking at ships" },
        { "DataMenu", "In menus" }, // last because there are sub menus
    };
}

#endif
