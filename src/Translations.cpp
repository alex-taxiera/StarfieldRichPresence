#include "Translations.h"
namespace Translations
{
    void LoadTranslations() noexcept
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
            Text::LaunchingGame       = ini.GetValue("Translation", "LaunchingGame", Text::LaunchingGame);
            Text::Exploring           = ini.GetValue("Translation", "Exploring", Text::Exploring);
            Text::Fighting            = ini.GetValue("Translation", "Fighting", Text::Fighting);
            Text::InSpace             = ini.GetValue("Translation", "InSpace", Text::InSpace);
            Text::InSpaceship         = ini.GetValue("Translation", "InSpaceship", Text::InSpaceship);
            Text::PilotingSpaceship   = ini.GetValue("Translation", "PilotingSpaceship", Text::PilotingSpaceship);
            Text::Level               = ini.GetValue("Translation", "Level", Text::Level);
            Text::In                  = ini.GetValue("Translation", "In", Text::In);
            Text::On                  = ini.GetValue("Translation", "On", Text::On);
            Text::At                  = ini.GetValue("Translation", "At", Text::At);
            Text::InMainMenu          = ini.GetValue("Translation", "InMainMenu", Text::InMainMenu);
            Text::PauseMenu           = ini.GetValue("Translation", "PauseMenu", Text::PauseMenu);
            Text::SleepWaitMenu       = ini.GetValue("Translation", "SleepWaitMenu", Text::SleepWaitMenu);
            Text::PickpocketMenu      = ini.GetValue("Translation", "PickpocketMenu", Text::PickpocketMenu);
            Text::LockpickingMenu     = ini.GetValue("Translation", "LockpickingMenu", Text::LockpickingMenu);
            Text::SkillsMenu          = ini.GetValue("Translation", "SkillsMenu", Text::SkillsMenu);
            Text::GalaxyStarMapMenu   = ini.GetValue("Translation", "GalaxyStarMapMenu", Text::GalaxyStarMapMenu);
            Text::PhotoModeMenu       = ini.GetValue("Translation", "PhotoModeMenu", Text::PhotoModeMenu);
            Text::PhotoGalleryMenu    = ini.GetValue("Translation", "PhotoGalleryMenu", Text::PhotoGalleryMenu);
            Text::BSMissionMenu       = ini.GetValue("Translation", "BSMissionMenu", Text::BSMissionMenu);
            Text::PowersMenu          = ini.GetValue("Translation", "PowersMenu", Text::PowersMenu);
            Text::InventoryMenu       = ini.GetValue("Translation", "InventoryMenu", Text::InventoryMenu);
            Text::ChargenMenu         = ini.GetValue("Translation", "ChargenMenu", Text::ChargenMenu);
            Text::SpaceshipEditorMenu = ini.GetValue("Translation", "SpaceshipEditorMenu", Text::SpaceshipEditorMenu);
            Text::DataMenu            = ini.GetValue("Translation", "DataMenu", Text::DataMenu);
        }

        logger::info("Loaded translation");
    }
}
