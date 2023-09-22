#pragma once

namespace Translations
{
    static void LoadTranslations() noexcept;

    namespace Text
    {
        static constexpr const char* LaunchingGame = "Launching Game";
        static constexpr const char* Exploring     = "Exploring";
        static constexpr const char* Fighting      = "Fighting";
        static constexpr const char* InSpace       = "In Space";
        static constexpr const char* InSpaceship   = "In Spaceship";
        static constexpr const char* Level         = "Lv";
        static constexpr const char* In            = "in";
        static constexpr const char* On            = "on";
        static constexpr const char* At            = "at";
        // Menu specific
        static constexpr const char* InMainMenu    = "In Main Menu";
        static constexpr const char* PauseMenu    = "Paused";
        static constexpr const char* SleepWaitMenu    = "Sleeping";
        static constexpr const char* PickpocketMenu    = "Pickpocketing";
        static constexpr const char* LockpickingMenu    = "Lockpicking";
        static constexpr const char* SkillsMenu    = "Checking perks";
        static constexpr const char* GalaxyStarMapMenu    = "Gazing at starmap";
        static constexpr const char* PhotoModeMenu    = "Taking a photo";
        static constexpr const char* PhotoGalleryMenu    = "Looking at photos";
        static constexpr const char* BSMissionMenu    = "Looking at the quest log";
        static constexpr const char* PowersMenu    = "Looking at powers";
        static constexpr const char* InventoryMenu    = "Checking inventory";
        static constexpr const char* ChargenMenu    = "Creating a character";
        static constexpr const char* SpaceshipEditorMenu    = "Looking at ships";
        static constexpr const char* DataMenu    = "In menus";
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
            { "MainMenu", Text::InMainMenu, false, false },
            { "PauseMenu", Text::PauseMenu, false }, // System Menu
            { "SleepWaitMenu", Text::SleepWaitMenu },
            { "PickpocketMenu", Text::PickpocketMenu },
            { "LockpickingMenu", Text::LockpickingMenu },
            { "SkillsMenu", Text::SkillsMenu },
            { "GalaxyStarMapMenu", Text::GalaxyStarMapMenu, false },
            { "PhotoModeMenu", Text::PhotoModeMenu },
            { "PhotoGalleryMenu", Text::PhotoGalleryMenu },
            { "BSMissionMenu", Text::BSMissionMenu },
            { "PowersMenu", Text::PowersMenu },
            { "InventoryMenu", Text::InventoryMenu },
            { "ChargenMenu", Text::ChargenMenu, false },
            { "SpaceshipEditorMenu", Text::SpaceshipEditorMenu },
            { "DataMenu", Text::DataMenu }, // last because there are sub menus
        };
    }
} // namespace Translations
