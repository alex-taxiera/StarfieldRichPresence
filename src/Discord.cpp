#include "Discord.h"
#include "Settings.h"
#include "Constants.h"
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
        DiscordRichPresence presence = {
            .state = state.c_str(),
            .details = details.c_str(),
            .largeImageKey = Resources::StarfieldLogo,
        };

        if (Settings::bShowTimeElapsed)
        {
            if (!startTime)
            {
                startTime = time(0);
            }
            presence.startTimestamp = startTime;
        }

        if (!state.empty())
        {
            logger::debug("Presence state: {}", state);
        }
        if (!details.empty())
        {
            logger::debug("Presence details: {}", details);
        }

        Discord_UpdatePresence(&presence);
    }

    bool InitializePresence()
    {
        DiscordEventHandlers handlers;
        memset(&handlers, 0, sizeof(handlers));
        Discord_Initialize(Constants::discordAppId.c_str(), &handlers, 1, NULL);
        logger::info("Discord Rich Presence initialized");

        SetDetails(Translations::Text::LaunchingGame);

        return true;
    }
} // namespace Discord
