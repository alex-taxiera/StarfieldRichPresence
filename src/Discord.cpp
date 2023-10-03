#include "Discord.h"

#include "Constants.h"
#include "Settings.h"
#include "Translations.h"

namespace Discord
{
    namespace
    {
        time_t startTime{};
    }

    void SetState(std::string state)
    {
        SetPresence(state, "");
    }

    void SetDetails(std::string details)
    {
        SetPresence("", details);
    }

    void SetPresence(std::string state, std::string details)
    {
        SetPresence(state, details, "", "");
    }

    void SetPresence(std::string state, std::string details, std::string smallImageKey, std::string smallImageText)
    {
        DiscordRichPresence presence = {
            .state          = state.c_str(),
            .details        = details.c_str(),
            .largeImageKey  = Resources::StarfieldLogo,
            .smallImageKey  = smallImageKey.c_str(),
            .smallImageText = smallImageText.c_str(),
        };

        if (Settings::bShowTimeElapsed) {
            if (!startTime) {
                startTime = time(0);
            }
            presence.startTimestamp = startTime;
        }

        if (!state.empty()) {
            logger::debug("Presence state: {}", state);
        }
        if (!details.empty()) {
            logger::debug("Presence details: {}", details);
        }
        if (!smallImageKey.empty()) {
            logger::debug("Presence small image key: {}", smallImageKey);
        }
        if (!smallImageText.empty()) {
            logger::debug("Presence small image text: {}", smallImageText);
        }

        Discord_UpdatePresence(&presence);
    }

    bool InitializePresence()
    {
        DiscordEventHandlers handlers;
        memset(&handlers, 0, sizeof(handlers));
        Discord_Initialize(Constants::discordAppId.c_str(), &handlers, 1, NULL);
        logger::info("Discord Rich Presence initialized");

        SetDetails(Translations::strings[Translations::Keys::LaunchingGame]);

        return true;
    }
} // namespace Discord
