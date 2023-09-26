#include "Translations.h"

void Translations::LoadTranslations()
{
    logger::info("Loading translation");

    CSimpleIniA ini;

    ini.SetUnicode();
    auto rc = ini.LoadFile(R"(.\Data\SFSE\Plugins\StarfieldRichPresenceTranslation.ini)");
    //if (rc < 0)
    //{
    //    logger::error("ERROR: Failed to load ini");
    //    return;
    //}
    if (rc == SI_OK)
    {
        // loaded translation file
        Text::LaunchingGame = ini.GetStringValue("Translation", "LaunchingGame", Text::LaunchingGame);
        Text::Exploring = ini.GetStringValue("Translation", "Exploring", Text::Exploring);
        Text::Fighting = ini.GetStringValue("Translation", "Fighting", Text::Fighting);
        Text::InSpace = ini.GetStringValue("Translation", "InSpace", Text::InSpace);
        Text::InSpaceship = ini.GetStringValue("Translation", "InSpaceship", Text::InSpaceship);
        Text::PilotingSpaceship = ini.GetStringValue("Translation", "PilotingSpaceship", Text::PilotingSpaceship);
        Text::Level = ini.GetStringValue("Translation", "Level", Text::Level);
        Text::In = ini.GetStringValue("Translation", "In", Text::In);
        Text::On = ini.GetStringValue("Translation", "On", Text::On);
        Text::At = ini.GetStringValue("Translation", "At", Text::At);
        Text::InMainMenu = ini.GetStringValue("Translation", "InMainMenu", Text::InMainMenu);
        Text::PauseMenu = ini.GetStringValue("Translation", "PauseMenu", Text::PauseMenu);
        Text::SleepWaitMenu = ini.GetStringValue("Translation", "SleepWaitMenu", Text::SleepWaitMenu);
        Text::PickpocketMenu = ini.GetStringValue("Translation", "PickpocketMenu", Text::PickpocketMenu);
        Text::LockpickingMenu = ini.GetStringValue("Translation", "LockpickingMenu", Text::LockpickingMenu);
        Text::SkillsMenu = ini.GetStringValue("Translation", "SkillsMenu", Text::SkillsMenu);
        Text::GalaxyStarMapMenu = ini.GetStringValue("Translation", "GalaxyStarMapMenu", Text::GalaxyStarMapMenu);
        Text::PhotoModeMenu = ini.GetStringValue("Translation", "PhotoModeMenu", Text::PhotoModeMenu);
        Text::PhotoGalleryMenu = ini.GetStringValue("Translation", "PhotoGalleryMenu", Text::PhotoGalleryMenu);
        Text::BSMissionMenu = ini.GetStringValue("Translation", "BSMissionMenu", Text::BSMissionMenu);
        Text::PowersMenu = ini.GetStringValue("Translation", "PowersMenu", Text::PowersMenu);
        Text::InventoryMenu = ini.GetStringValue("Translation", "InventoryMenu", Text::InventoryMenu);
        Text::ChargenMenu = ini.GetStringValue("Translation", "ChargenMenu", Text::ChargenMenu);
        Text::SpaceshipEditorMenu = ini.GetStringValue("Translation", "SpaceshipEditorMenu", Text::SpaceshipEditorMenu);
        Text::DataMenu = ini.GetStringValue("Translation", "DataMenu", Text::DataMenu);
    }

    logger::info("Loaded translation");
}
