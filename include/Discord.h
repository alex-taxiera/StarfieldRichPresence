#pragma once
#include "Constants.h"

namespace Discord
{
    bool InitializePresence();
    void SetPresence(DiscordRichPresence presence);

    namespace
    {
        time_t startTime = 0;
    }
} // namespace Discord
