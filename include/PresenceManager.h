#pragma once

namespace PresenceManager
{
    namespace
    {
        std::thread* thread;

        std::string Get_PlayerInfo(RE::TESObjectREFR* playerRef, RE::TESActorBaseData* playerActorBaseData);
        std::string Get_PlayerState(RE::TESObjectREFR* playerRef, RE::Actor* playerActor);

        void MainLoop();
    }

    void Start() noexcept;
} // namespace PresenceManager
