#include "Hooks.h"
#include "PresenceManager.h"
#include "Constants.h"

namespace Hooks
{
    namespace Presence
    {
        plf::nanotimer timer;

        uintptr_t addresses[2] = {
            REL::Relocation(REL::ID(148991), 0xCD).address(), // when menu is opened
            REL::Relocation(REL::ID(146554), 0x10).address()  // when traveling to a new location (interior/exterior/planet)
        };

        bool ShouldTriggerChange()
        {
            if (timer.get_elapsed_ms() < Constants::updateDebounceTime)
            {
                return false;
            }

            timer.start(); // restart timer
            return true;
        }

        struct Hook
        {
            static void thunk(__int64 unk)
            {
                auto shouldCheck = ShouldTriggerChange();

                if (shouldCheck)
                {
                    logger::debug("PresenceHook Thunk triggering");
                    PresenceManager::HandleUpdate();
                }

                func(unk);
            }

            static inline REL::Relocation<decltype(thunk)> func;
        };
    }

    void Install() noexcept
    {
        logger::info("Installing hooks");
        Presence::timer.start();

        // setup hooks
        for (const auto& address : Presence::addresses)
        {
            logger::debug("Installing Presence hook at {:x}", address);
            SFSE::stl::write_thunk_call<Presence::Hook>(address);
        }

        logger::info("Installed hooks");
    }
} // namespace Hooks
