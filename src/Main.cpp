#include "Hooks.h"
#include "Discord.h"
#include "Settings.h"
#include "Translations.h"

// SFSE message listener, use this to do stuff at specific moments during runtime
void Listener(SFSE::MessagingInterface::Message* message) noexcept
{
    if (message->type <=> SFSE::MessagingInterface::kPostPostLoad == 0)
    {
        Settings::LoadSettings();
        Translations::LoadTranslations();
        auto ready = Discord::InitializePresence();
        if (!ready)
        {
            logger::error("Failed to initialize Discord Rich Presence");
            return;
        }
        Hooks::Install();
    }
}

// Main SFSE plugin entry point, initialize everything here
SFSEPluginLoad(const SFSE::LoadInterface* sfse)
{
    Init(sfse);

    logger::info("{} {} is loading...", Plugin::Name, Plugin::Version.string("."sv));

    if (const auto messaging{ SFSE::GetMessagingInterface() }; !messaging->RegisterListener(Listener))
        return false;

    logger::info("{} has finished loading.", Plugin::Name);

    return true;
}
