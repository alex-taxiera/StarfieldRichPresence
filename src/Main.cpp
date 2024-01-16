#include "Discord.h"
#include "Hooks.h"
#include "Settings.h"
#include "Translations.h"
#include "PresenceManager.h"

void Listener(SFSE::MessagingInterface::Message* message) noexcept
{
	switch (message->type) {
	case SFSE::MessagingInterface::kPostPostLoad:
		{
			Settings::LoadSettings();
			Translations::LoadTranslations();

			auto ready = Discord::InitializePresence();
			if (!ready) {
				logger::error("Failed to initialize Discord Rich Presence");
				return;
			}

			Hooks::InstallMenuHook();
			break;
		}
		case SFSE::MessagingInterface::kPostPostDataLoad:
		{
			Hooks::InstallMainHook();
			break;
		}
	}
}

SFSEPluginLoad(const SFSE::LoadInterface* sfse)
{
	Init(sfse);

	logger::info("{} {} is loading...", Plugin::Name,
		Plugin::Version.string("."sv));

	if (const auto messaging{ SFSE::GetMessagingInterface() };
		!messaging->RegisterListener(Listener))
		return false;

	logger::info("{} has finished loading.", Plugin::Name);

	return true;
}
