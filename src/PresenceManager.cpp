#include "PresenceManager.h"
#include "Settings.h"
#include "Constants.h"
#include "Discord.h"
#include "UIMenu.h"

//class TESWorldSpace : public RE::TESForm,    // 00
//                      public RE::TESFullName // 20
//{
//public:
//    // DEFINE_MEMBER_FN_0(GetWorldspaceName, const char*, 0x012EF550);
//};
//
//class MyCell : public RE::TESObjectCELL
//{
//public:
//    uint16_t       flags; // 48
//    bool           pad[54];
//    TESWorldSpace* CellWorldSpace; // 80
//};
//static_assert(offsetof(MyCell, flags) == 0x48);
//static_assert(offsetof(MyCell, pad) == 0x4A);
//static_assert(offsetof(MyCell, CellWorldSpace) == 0x80);

//typedef TESWorldSpace*(__fastcall* _GetWorldSpace)(RE::TESObjectREFR* TargetRef); // __int64 __fastcall sub_7FF695A3FC84(__int64 a1)
//REL::Relocation<_GetWorldSpace> GetWorldSpace{ REL::Offset(0x7FF695A3FC84) };

namespace PresenceManager
{
    namespace
    {
        typedef __int64(__fastcall* _sub_1423F985C)(__int64 unk);
        REL::Relocation<_sub_1423F985C> sub_1423F985C(0x023F985C); // Function to call later

        typedef RE::TESObjectCELL*(__fastcall* _GetRefCell)(RE::TESObjectREFR* TargetRef);
        REL::Relocation<_GetRefCell> GetRefCell{ REL::Offset(0x01A093BC) };

        typedef bool(__fastcall* _GetItemCount)(RE::TESObjectREFR** TargetREF, RE::TESForm* MyForm, __int64 pad, float* result);
        REL::Relocation<_GetItemCount> GetItemCount{ REL::Offset(0x01A7A6DC) };

        typedef bool(__fastcall* _GetActorValuePercentage)(RE::Actor** TargetREF, RE::ActorValueInfo* MyForm, __int64 pad, float* result);
        REL::Relocation<_GetActorValuePercentage> GetActorValuePercentage{ REL::Offset(0x01A733C8) };

        //typedef RE::TESForm* (*_TargetToTrampoline)(uint32_t id);
        //REL::Relocation<_TargetToTrampoline> targetToTrampoline{ REL::Offset(0x23F447C + 0xCD) };
        //REL::Relocation<_TargetToTrampoline> targetToTrampoline2{ REL::Offset(0x23322B8 + 0x10) };

        RE::TESActorBaseData* GetPlayerActorBaseData()
        {
            auto playerFormBASE = RE::TESForm::LookupByID(0x00000007);

            if (!playerFormBASE)
            {
                logger::debug("playerFormBASE is not valid");
                return NULL;
            }

            auto playerActorBaseData = starfield_cast<RE::TESActorBaseData*>(playerFormBASE);

            if (!playerActorBaseData)
            {
                logger::debug("playerActorBaseData is not valid");
                return NULL;
            }

            return playerActorBaseData;
        }

        std::string BuildDetailsString(RE::Actor* playerActor, RE::TESActorBaseData* playerActorBaseData)
        {
            std::string details{};

            logger::debug("getting player name");
            std::string playerName = playerActor->GetDisplayFullName();

            if (Settings::bShowCharacterName)
            {
                logger::debug("got player name: {}", playerName);
                details = playerName;
            }
            if (Settings::bShowCharacterLevel)
            {
                if (Settings::bShowCharacterName)
                {
                    details += " | ";
                }
                details += std::format("{} {}", Text::Level, playerActorBaseData->actorData.level);
            }

            return details;
        }

        std::string GetPlayerLocationName(RE::Actor* playerActor)
        {
            /*if (playerActor->parentCell)
            {
                auto myCell = (MyCell*)playerActor->parentCell;
                logger::info("my cell form id {} form type {}", myCell->GetFormID(), myCell->GetFormType());
                logger::info("trying to get world space");
                auto worldspace = mycell->cellworldspace;
                logger::info("got world space id {} name {}", worldSpace->GetFormID(), worldSpace->GetFullName());
            }*/

            /*logger::info("trying to get worldspace");
            auto worldSpace = GetWorldSpace(playerRef);
            if (worldSpace)
            {
                logger::info("got worldspace");
                std::string worldSpaceName = worldSpace->GetFullName();

                if (worldSpaceName.length() > 0)
                {
                    logger::debug("got worldspace name: {}", worldSpaceName.c_str());
                    state += worldSpaceName;
                }
            }*/

            std::string locationName{};

            logger::debug("trying to get ref cell");
            auto refCell = GetRefCell(playerActor);

            if (refCell)
            {
                logger::debug("got ref cell id {}", refCell->GetFormID());
                std::string cellName = refCell->GetFullName();

                if (cellName.length() > 0)
                {
                    logger::debug("got cell name: {}", cellName);
                    locationName = cellName;
                }
            }
            else if (playerActor->parentCell)
            {
                logger::debug("got parent cell id {}", playerActor->parentCell->GetFormID());
                std::string parentCellName = playerActor->parentCell->GetFullName(); // for interiors

                if (parentCellName.length() > 0)
                {
                    logger::debug("got parent cell name: {}", parentCellName);
                    locationName = parentCellName;
                }
            }

            return locationName;
        }

        std::string BuildStateString(RE::Actor* playerActor)
        {
            bool playerIsOutside = GetRefCell(playerActor);

            std::string state{};

            logger::debug("getting player is in space");
            if (playerActor->IsInSpace())
            {
                logger::debug("player is in space");
                state = Text::InSpace;
            }

            if (state.empty())
            {
                logger::debug("trying to get spaceship");
                auto playerShip = playerActor->GetAttachedSpaceship();

                if (playerShip)
                {
                    logger::debug("got spaceship");
                    state = Text::InSpaceship;
                    std::string shipName = playerShip->GetDisplayFullName();

                    if (Settings::bShowShipName && shipName.length() > 0)
                    {
                        logger::debug("got ship name: {}", shipName.c_str());
                        state = std::format("{} ({})", state, shipName);
                    }
                    auto playerLocationName = GetPlayerLocationName(playerActor);

                    state = playerLocationName.empty()
                        ? state
                        : std::format("{} | {}", state, playerLocationName);
                }
            }

            if (state.empty())
            {
                logger::debug("trying to get combat status");
                bool isInCombat = playerActor->IsInCombat();

                auto playerLocationName = GetPlayerLocationName(playerActor);

                if (isInCombat)
                {
                    logger::debug("got combat status true");

                    state = playerLocationName.empty()
                        ? Text::Fighting
                        : std::format(
                            "{} {} {}",
                            Text::Fighting,
                            playerIsOutside ? Text::On : Text::In, playerLocationName
                        );
                }
                else
                {
                    logger::debug("got combat status false");
                    state = std::format("{} {}", Text::Exploring, playerLocationName);
                }
            }

            return state;
        }
    }

    void HandleLoadInstance()
    {
        auto playerActor = RE::Actor::PlayerCharacter();

        if (!playerActor)
        {
            logger::debug("playerActor is not valid");
            return;
        }

        auto playerActorBaseData = GetPlayerActorBaseData();

        if (!playerActorBaseData)
        {
            logger::debug("playerActorBaseData is not valid");
            return;
        }

        logger::debug("All valid - continuing");

        Discord::SetPresence(BuildStateString(playerActor), BuildDetailsString(playerActor, playerActorBaseData));
    }

    void HandleMenuInteract()
    {
        auto ui = RE::UI::GetSingleton();

        for (const auto& menuEntry : UIMenu::menuEntries)
        {
            if (ui->IsMenuOpen(menuEntry.menuName))
            {
                logger::debug("Menu {} is open", menuEntry.menuName);

                auto playerActor = RE::Actor::PlayerCharacter();
                std::string state = menuEntry.menuText;

                if (state.empty())
                {
                    // do not change presence
                    return;
                }

                if (menuEntry.shouldShowLocation && playerActor)
                {
                    auto locationName = GetPlayerLocationName(playerActor);
                    if (!locationName.empty())
                    {
                        bool playerIsOutside = GetRefCell(playerActor);
                        state = std::format(
                            "{} {} {}",
                            state,
                            playerIsOutside ? Text::On : Text::In,
                            locationName
                        );
                    }
                }

                if (menuEntry.shouldShowDetails)
                {
                    auto playerActorBaseData = GetPlayerActorBaseData();

                    if (!playerActorBaseData || !playerActor)
                    {
                        Discord::SetState(state);
                    }
                    else
                    {
                        Discord::SetPresence(state, BuildDetailsString(playerActor, playerActorBaseData));
                    }
                }
                else
                {
                    Discord::SetState(state);
                }

                // handled and in menu
                return;
            }
        }

        // not in a menu, handle as load
        HandleLoadInstance();
    }
} // namespace PresenceManager
