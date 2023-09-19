#pragma once

// Define your hook classes here
namespace Hooks
{
    void HandleDiscordReady(const DiscordUser* user);
    void HandleDiscordError(int errorCode, const char* message);
    void HandleDiscordDisconnect(int errorCode, const char* message);
    void SetDiscordPresence(const char* sState, const char* s_Details);

    void Install() noexcept;
} // namespace Hooks
