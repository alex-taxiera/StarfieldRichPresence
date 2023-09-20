#include "Discord.h"
#include "Settings.h"
#include "Constants.h"

namespace Discord
{
    namespace
    {
        time_t startTime = 0;
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
        logger::debug("Setting presence");
        DiscordRichPresence presence = {
            .state = state.c_str(),
            .details = details.c_str(),
        };

        if (Settings::bShowTimeElapsed)
        {
            presence.startTimestamp = startTime;
        }

        if (!presence.largeImageKey)
        {
            presence.largeImageKey = Resources::StarfieldLogo;
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
        startTime = time(0);
        DiscordEventHandlers handlers;
        memset(&handlers, 0, sizeof(handlers));
        Discord_Initialize(discordAppId.c_str(), &handlers, 1, NULL);

        logger::info("Discord Rich Presence initialized");

        SetDetails(Text::LaunchingGame);

        return true;
    }
} // namespace Discord
