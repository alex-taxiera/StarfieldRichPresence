#include "PresenceManager.h"
#include "Settings.h"
#include "Discord.h"
#include "Translations.h"

namespace PresenceManager
{
    namespace
    {
        typedef bool(__fastcall* _GetItemCount)(RE::TESObjectREFR** TargetREF, RE::TESForm* MyForm, __int64 pad, float* result);
        REL::Relocation<_GetItemCount> GetItemCount{ REL::ID(108751) };

        typedef bool(__fastcall* _GetActorValuePercentage)(RE::Actor** TargetREF, RE::ActorValueInfo* MyForm, __int64 pad, float* result);
        REL::Relocation<_GetActorValuePercentage> GetActorValuePercentage{ REL::ID(108598) };

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
                details += std::format("{} {}", Translations::Text::Level, playerActorBaseData->actorData.level);
            }

            return details;
        }

        std::string GetPlayerLocationName(RE::Actor* playerActor)
        {
            std::string locationName{};
            auto location = playerActor->GetCurrentLocation();
            if (location)
            {
                locationName = location->GetFullName();
            }
            return locationName;
        }

        std::string GetPlayerParentWorldSpaceName(RE::Actor* playerActor)
        {
            std::string parentWorldSpaceName{};
            auto actorParentWorldSpace = playerActor->GetParentWorldSpace();
            if (actorParentWorldSpace)
            {
                parentWorldSpaceName = actorParentWorldSpace->GetFullName();
            }
            return parentWorldSpaceName;
        }

        std::string BuildStateString(RE::Actor* playerActor)
        {
            std::string state{};

            if (playerActor->IsInSpace(false))
            {
                logger::debug("player is in space");
                auto playerLocationName         = GetPlayerLocationName(playerActor);
                auto playerParentWorldSpaceName = GetPlayerParentWorldSpaceName(playerActor);
                logger::debug("playerLocationName: {}", playerLocationName);
                logger::debug("playerParentWorldSpaceName: {}", playerParentWorldSpaceName);

                state = Translations::Text::InSpace;
            }

            if (state.empty())
            {
                auto playerShip = playerActor->GetSpaceship();

                if (playerShip)
                {
                    logger::debug("player is in spaceship");
                    state = Translations::Text::InSpaceship;
                    std::string shipName = playerShip->GetDisplayFullName();

                    if (Settings::bShowShipName && !shipName.empty())
                    {
                        logger::debug("shipName: {}", shipName.c_str());
                        state = std::format("{} ({})", state, shipName);
                    }

                    auto playerLocationName         = GetPlayerLocationName(playerActor);
                    auto playerParentWorldSpaceName = GetPlayerParentWorldSpaceName(playerActor);
                    logger::debug("playerLocationName: {}", playerLocationName);
                    logger::debug("playerParentWorldSpaceName: {}", playerParentWorldSpaceName);

                    //state = playerLocationName.empty()
                    //    ? state
                    //    : std::format("{} | {}", state, playerLocationName);
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
                    bool playerIsOutside = playerActor->parentCell;

                    state = playerLocationName.empty()
                        ? Translations::Text::Fighting
                        : std::format(
                            "{} {} {}",
                            Translations::Text::Fighting,
                            playerIsOutside ? Translations::Text::On : Translations::Text::In, playerLocationName
                        );
                }
                else
                {
                    logger::debug("got combat status false");
                    auto playerParentWorldSpaceName = GetPlayerParentWorldSpaceName(playerActor);

                    logger::debug("{} | {}", playerLocationName, playerParentWorldSpaceName);

                    if (!playerParentWorldSpaceName.empty() && Settings::bShowPlanetWhileOutside)
                    {
                        state = std::format("{} {} | {}", Translations::Text::Exploring, playerLocationName, playerParentWorldSpaceName);
                    }
                    else
                    {
                        state = std::format("{} {}", Translations::Text::Exploring, playerLocationName);
                    }
                }
            }

            return state;
        }

        bool SetPresenceBasedOnGameState()
        {
            auto playerActor = RE::Actor::PlayerCharacter();

            if (!playerActor)
            {
                logger::debug("playerActor is not valid");
                return false;
            }

            auto playerActorBaseData = GetPlayerActorBaseData();

            if (!playerActorBaseData)
            {
                logger::debug("playerActorBaseData is not valid");
                return false;
            }

            logger::debug("All valid - continuing");

            Discord::SetPresence(BuildStateString(playerActor), BuildDetailsString(playerActor, playerActorBaseData));

            return true;
        }

        bool SetPresenceBasedOnMenuState()
        {
            auto ui = RE::UI::GetSingleton();

            for (const auto& menuEntry : Translations::UIMenu::menuEntries)
            {
                if (ui->IsMenuOpen(menuEntry.menuName))
                {
                    logger::debug("Menu {} is open", menuEntry.menuName);

                    std::string state = menuEntry.menuText;

                    if (state.empty())
                    {
                        // do not change presence
                        return true;
                    }

                    auto playerActor = RE::Actor::PlayerCharacter();

                    if (menuEntry.shouldShowLocation && playerActor)
                    {
                        auto locationName = GetPlayerLocationName(playerActor);
                        if (!locationName.empty())
                        {
                            bool playerIsOutside = playerActor->parentCell;
                            state = std::format(
                                "{} {} {}",
                                state,
                                playerIsOutside ? Translations::Text::On : Translations::Text::In,
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
                    return true;
                }
            }

            return false;
        }
    }

    void HandleUpdate()
    {
        auto handled = SetPresenceBasedOnMenuState();
        if (!handled)
        {
            // not in a menu, handle as load
            SetPresenceBasedOnGameState();
        }
    }
} // namespace PresenceManager
