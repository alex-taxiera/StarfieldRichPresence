#pragma once

namespace UIMenu
{
    struct MenuEntry
    {
        const char* menuName           = NULL;
        const char* menuText           = NULL;
        bool        shouldShowLocation = true;
        bool        shouldShowDetails  = true;
    };

    static MenuEntry menuEntries[16] = {
        { "LoadingMenu", "" },
        { "MainMenu", "In Main Menu", false, false },
        { "PauseMenu", "Paused", false }, // System Menu
        { "SleepWaitMenu", "Sleeping" },
        { "PickpocketMenu", "Pickpocketing" },
        { "LockpickingMenu", "Lockpicking" },
        { "SkillsMenu",     "Checking perks" },
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
} // namespace UIMenu
