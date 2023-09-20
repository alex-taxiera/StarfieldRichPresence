#pragma once
#include "discord_register.h"
#include "discord_rpc.h"

namespace Discord
{
    bool InitializePresence();
    void SetState(std::string state);
    void SetDetails(std::string details);
    void SetPresence(std::string state, std::string details);
} // namespace Discord
