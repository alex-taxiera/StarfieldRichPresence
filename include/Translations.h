#pragma once

namespace Translations
{
    void LoadTranslations() noexcept;

    namespace Text
    {
        static const char* LaunchingGame = "Launching Game";
        static const char* Exploring     = "Exploring";
        static const char* Fighting      = "Fighting";
        static const char* InSpace       = "In outer space";
        static const char* InSpaceship   = "In ship";
        static const char* PilotingSpaceship = "Piloting ship";
        static const char* Level         = "Lv";
        static const char* In            = "in";
        static const char* On            = "on";
        static const char* At            = "at";
        // Menu specific
        static const char* InMainMenu    = "In Main Menu";
        static const char* PauseMenu    = "Paused";
        static const char* SleepWaitMenu    = "Sleeping";
        static const char* PickpocketMenu    = "Pickpocketing";
        static const char* LockpickingMenu    = "Lockpicking";
        static const char* SkillsMenu    = "Checking perks";
        static const char* GalaxyStarMapMenu    = "Gazing at starmap";
        static const char* PhotoModeMenu    = "Taking a photo";
        static const char* PhotoGalleryMenu    = "Looking at photos";
        static const char* BSMissionMenu    = "Looking at the quest log";
        static const char* PowersMenu    = "Looking at powers";
        static const char* InventoryMenu    = "Checking inventory";
        static const char* ChargenMenu    = "Creating a character";
        static const char* SpaceshipEditorMenu    = "Looking at ships";
        static const char* DataMenu    = "In menus";
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
