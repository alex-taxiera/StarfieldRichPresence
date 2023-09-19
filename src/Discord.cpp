#include "Discord.h"
#include "Settings.h"
#include "Constants.h"

namespace Discord
{
    void SetPresence(DiscordRichPresence presence)
    {
        if (Settings::bShowTimeElapsed)
        {
            presence.startTimestamp = startTime;
        }

        if (!presence.largeImageKey)
        {
            presence.largeImageKey = resources.StarfieldLogo.c_str();
        }

        if (presence.state)
        {
            logger::debug("Presence state: {}", presence.state);
        }
        if (presence.details)
        {
            logger::debug("Presence details: {}", presence.details);
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

        SetPresence({ .details = text.LaunchingGame.c_str() });

        return true;
    }
} // namespace Discord
