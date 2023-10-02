#include "Translations.h"

#include "Settings.h"

void Translations::LoadTranslations() noexcept
{
    logger::info("Loading translation");

    CSimpleIniA ini;

    ini.SetUnicode();
    auto rc = ini.LoadFile(R"(.\Data\SFSE\Plugins\StarfieldRichPresenceTranslation.ini)");

    if (rc == SI_OK)
    {
        // loaded translation file
        for (auto const& key : std::views::keys(strings))
        {
            strings[key] = ini.GetValue("Translation", translationKeys.at(key), strings[key].c_str());
        }

        logger::info("Loaded translation");
    }
    else
    {
        // write default values to translation file
        for (auto const& key : std::views::keys(strings))
        {
            ini.SetValue("Translation", translationKeys.at(key), strings[key].c_str());
        }

        if (!Settings::bEnableDebug)
        {
            ini.SaveFile(R"(.\Data\SFSE\Plugins\StarfieldRichPresenceTranslation.ini)");
            logger::info("Created translation file using defaults (English)");
        }
    }
}
