#include "PresenceManager.h"
#include "Settings.h"
#include "Constants.h"
#include "Discord.h"

typedef RE::TESObjectCELL*(__fastcall* _GetRefCell)(RE::TESObjectREFR* TargetRef);
REL::Relocation<_GetRefCell> GetRefCell{ REL::Offset(0x01A093BC) };

namespace PresenceManager
{
    namespace
    {
        std::string Get_PlayerInfo(RE::TESObjectREFR* playerRef, RE::TESActorBaseData* playerActorBaseData)
        {
            std::string details{};

            logger::debug("getting player name");
            std::string playerName = playerRef->GetDisplayFullName();

            if (Settings::bShowCharacterName)
            {
                if (playerName != "Player")
                {
                    logger::debug("got player name: {}", playerName);
                    details = playerName;
                }
                else
                {
                    details = "Godd Howard";
                }
            }
            if (Settings::bShowCharacterLevel && playerName != "Player")
            {
                if (Settings::bShowCharacterName)
                {
                    details += " | ";
                }
                details += std::format("{} {}", text.Level, playerActorBaseData->actorData.level);
            }

            return details;
        }

        std::string Get_PlayerState(RE::TESObjectREFR* playerRef, RE::Actor* playerActor)
        {
            logger::debug("getting player is in space");
            if (playerRef->IsInSpace())
            {
                logger::debug("player is in space");
                return text.InSpace;
            }

            std::string state{};

            logger::debug("trying to get spaceship");
            auto playerShip = playerRef->GetAttachedSpaceship();

            if (playerShip)
            {
                logger::debug("got spaceship");
                state = text.InSpaceship;

                if (Settings::bShowShipName)
                {
                    std::string shipName = playerShip->GetDisplayFullName();
                    logger::debug("got ship name: {}", shipName.c_str());

                    if (shipName.length() > 0)
                    {
                        state += std::format("({})", shipName);
                    }
                }

                return state;
            }

            logger::debug("trying to get combat status");
            bool isInCombat = playerActor->IsInCombat();

            if (isInCombat)
            {
                logger::debug("got combat status true");
                state = text.Fighting + " ";
            }
            else
            {
                logger::debug("got combat status false");
                state = text.Exploring + " ";
            }

            logger::debug("trying to get ref cell");
            auto RefCell = GetRefCell(playerRef);

            if (RefCell)
            {
                logger::debug("got ref cell");
                std::string cellName = RefCell->GetFullName();

                if (cellName.length() > 0)
                {
                    logger::debug("got cell name: {}", cellName.c_str());
                    state += cellName;
                }
            }
            else if (playerRef->parentCell)
            {
                logger::debug("got parent cell");
                std::string parentCellName = playerRef->parentCell->GetFullName(); // for interiors

                if (parentCellName.length() > 0)
                {
                    logger::debug("got parent cell name: {}", parentCellName.c_str());
                    state += parentCellName;
                }
            }

            return state;
        }

        void MainLoop()
        {
            (*thread).detach();
            Settings::LoadSettings();

            for (;;)
            {
                logger::debug("sleeping for {}", sleepTime);
                Sleep(sleepTime);

                auto playerRefAsForm = RE::TESForm::LookupByID(0x00000014);

                if (!playerRefAsForm)
                {
                    logger::debug("playerRefAsForm is not valid");
                    continue;
                }

                auto playerRef = playerRefAsForm->AsReference();

                if (!playerRef)
                {
                    logger::debug("playerRef is not valid");
                    continue;
                }

                if (!playerRef->parentCell)
                {
                    logger::debug("no parentCell, setting presence to main menu");

                    Discord::SetPresence({ .state = text.InMainMenu.c_str() });
                    continue;
                }

                auto playerActor = (RE::Actor*)playerRef;

                if (!playerActor)
                {
                    logger::debug("playerActor is not valid");
                    continue;
                }

                auto playerFormBASE = RE::TESForm::LookupByID(0x00000007);

                if (!playerFormBASE)
                {
                    logger::debug("playerFormBASE is not valid");
                    continue;
                }

                auto playerActorBaseData = starfield_cast<RE::TESActorBaseData*>(playerFormBASE);

                if (!playerActorBaseData)
                {
                    logger::debug("playerActorBaseData is not valid");
                    continue;
                }

                logger::debug("All valid - continuing");

                auto sState   = Get_PlayerState(playerRef, playerActor);
                auto sDetails = Get_PlayerInfo(playerRef, playerActorBaseData);

                Discord::SetPresence({ .state = sState.c_str(), .details = sDetails.c_str() });
            }
        }
    }

    void Start() noexcept
    {
        auto ready = Discord::InitializePresence();
        if (!ready)
        {
            logger::error("Failed to initialize Discord Rich Presence");
            return;
        }

        thread = new std::thread(MainLoop);
    }
} // namespace PresenceManager
