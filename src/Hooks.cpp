#include "Hooks.h"

#include "Constants.h"
#include "PresenceManager.h"

namespace Hooks
{
	namespace Presence
	{
		plf::nanotimer timer;

		auto menuThreadAddress =
			REL::Relocation(REL::ID(148991), 0xCD).address();

		auto mainThreadAddress =
			REL::Relocation(REL::ID(146554), 0x10).address();

		bool ShouldTriggerChange()
		{
			if (timer.get_elapsed_ms() < Constants::updateDebounceTime) {
				return false;
			}

			timer.start();  // restart timer
			return true;
		}

		struct Hook
		{
			static void thunk(__int64 unk)
			{
				auto shouldCheck = ShouldTriggerChange();

				if (shouldCheck) {
					logger::debug("PresenceHook Thunk triggering");
					PresenceManager::HandleUpdate();
				}

				func(unk);
			}

			inline static REL::Relocation<decltype(thunk)> func;
		};
	}  // namespace Presence

	void InstallMenuHook() noexcept
	{
		Presence::timer.start();

		logger::debug("Installing Presence hook at {:x}",
			Presence::menuThreadAddress);
		SFSE::stl::write_thunk_call<Presence::Hook>(
			Presence::menuThreadAddress);
	}

	void InstallMainHook() noexcept
	{
		Presence::timer.start();

		logger::debug("Installing Presence hook at {:x}",
			Presence::mainThreadAddress);
		SFSE::stl::write_thunk_call<Presence::Hook>(
			Presence::mainThreadAddress);
	}
}  // namespace Hooks
