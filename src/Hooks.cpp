#include "Hooks.h"
#include "PresenceManager.h"
#include "Constants.h"

namespace Hooks
{
    namespace
    {
        plf::nanotimer loopTimer;

        bool ShouldTriggerChange()
        {
            double elapsedTime = loopTimer.get_elapsed_ms();

            if (elapsedTime < updateDebounceTime)
            {
                return false;
            }

            loopTimer.start();

            return true;
        }

        class OnOpenMenu
        {
        public:
            static inline uintptr_t address = REL::Relocation(0x23F447C, 0xCD).address();

            static void Thunk(__int64 unk)
            {
                auto shouldCheck = ShouldTriggerChange();

                if (shouldCheck)
                {
                    logger::debug("OnOpenMenu Thunk triggering");
                    PresenceManager::HandleMenuInteract();
                }

                func(unk);
            }

            static inline REL::Relocation<decltype(Thunk)> func;
        };

        class OnLoadInstance
        {
        public:
            static inline uintptr_t address = REL::Relocation(0x23322B8, 0x10).address();

            static void Thunk(__int64 unk)
            {
                auto shouldCheck = ShouldTriggerChange();

                if (shouldCheck)
                {
                    logger::debug("OnLoadInstance Thunk triggering");
                    PresenceManager::HandleLoadInstance();
                }

                func(unk);
            }

            static inline REL::Relocation<decltype(Thunk)> func;
        };
    }

    void Install() noexcept
    {
        logger::info("Installing hooks");
        loopTimer.start();
        SFSE::stl::write_thunk_call<Hooks::OnOpenMenu>();
        SFSE::stl::write_thunk_call<Hooks::OnLoadInstance>();
    }
} // namespace Hooks
