#include "PresenceManager.h"
#include "Settings.h"
#include "Discord.h"
#include "Translations.h"

namespace PresenceManager
{
    namespace
    {
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

            auto spaceship          = playerActor->GetSpaceship();
            bool isInCombat         = playerActor->IsInCombat();
            auto playerLocationName = GetPlayerLocationName(playerActor);

            /*if (playerActor->IsInSpace(false))
            {
                logger::debug("player is in space");
                auto playerLocationName         = GetPlayerLocationName(playerActor);
                auto playerParentWorldSpaceName = GetPlayerParentWorldSpaceName(playerActor);
                logger::debug("playerLocationName: {}", playerLocationName);
                logger::debug("playerParentWorldSpaceName: {}", playerParentWorldSpaceName);

                state = Translations::Text::InSpace;
            }*/

            if (spaceship)
            {
                std::string shipName       = spaceship->GetDisplayFullName();
                auto        pilot          = spaceship->GetSpaceshipPilot();
                bool        isPiloting     = pilot && pilot == playerActor;
                bool        isInPlayerShip = playerLocationName == "Ship";

                if (isInCombat)
                {
                    state = std::format("{} {} {}", Translations::Text::Fighting, Translations::Text::On, !isInPlayerShip || Settings::bShowShipName ? shipName : "ship");
                }
                else if (isPiloting)
                {
                    if (isInPlayerShip && !Settings::bShowShipName)
                    {
                        state = Translations::Text::PilotingSpaceship;
                    }
                    else
                    {
                        state = std::format("{} ({})", Translations::Text::PilotingSpaceship, shipName);
                    }
                }
                else if (isInPlayerShip)
                {
                    if (Settings::bShowShipName)
                    {
                        state = std::format("{} ({})", Translations::Text::InSpaceship, shipName);
                    }
                    else
                    {
                        state = Translations::Text::InSpaceship;
                    }
                }
                else
                {
                    state = std::format("{} {}", Translations::Text::Exploring, shipName);
                }
            }
            else
            {
                auto playerParentWorldSpaceName = GetPlayerParentWorldSpaceName(playerActor);

                if (isInCombat)
                {
                    if (!playerParentWorldSpaceName.empty() && Settings::bShowPlanetWhileOutside)
                    {
                        state = std::format("{} {} {} | {}", Translations::Text::Fighting, Translations::Text::At, playerLocationName, playerParentWorldSpaceName);
                    }
                    else
                    {
                        state = std::format("{} {} {}", Translations::Text::Fighting, Translations::Text::On, playerLocationName);
                    }
                }
                else
                {
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
                return false;
            }

            auto playerActorBaseData = GetPlayerActorBaseData();

            if (!playerActorBaseData)
            {
                return false;
            }

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
                            bool playerIsInside = playerActor->parentCell;
                            state = std::format(
                                "{} {} {}",
                                state,
                                playerIsInside ? Translations::Text::In : Translations::Text::On,
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
