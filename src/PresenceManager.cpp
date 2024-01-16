#include "PresenceManager.h"

#include "Constants.h"
#include "Discord.h"
#include "Settings.h"
#include "Translations.h"

namespace PresenceManager
{
	namespace
	{
		RE::TESActorBaseData* GetPlayerActorBaseData()
		{
			RE::TESActorBaseData* playerActorBaseData{};
			auto playerFormBASE = RE::TESForm::LookupByID(0x00000007);

			if (playerFormBASE) {
				playerActorBaseData =
					starfield_cast<RE::TESActorBaseData*>(playerFormBASE);
			}

			return playerActorBaseData;
		}

		RE::Actor* GetActiveCompanion()
		{
			RE::Actor* companion{};

			for (const auto& companionRefID :
				std::views::keys(Resources::companionDataMap)) {
				auto actor =
					(RE::Actor*)RE::TESForm::LookupByID(companionRefID);
				if (actor && actor->boolBits.all(
								 RE::Actor::BOOL_BITS::kPlayerTeammate)) {
					companion = actor;
					break;
				}
			}

			if (companion) {
				logger::debug("companion: {}, id {:x}",
					companion->GetDisplayFullName(),
					companion->GetFormID());
			}

			return companion;
		}

		RE::BGSLocation* GetLocation()
		{
			RE::BGSLocation* location{};
			auto             playerActor = RE::PlayerCharacter::GetSingleton();

			if (playerActor) {
				location = playerActor->GetCurrentLocation();
			}

			if (location && Settings::bEnableDebug) {
				// DEBUG TREE

				logger::info("location: {}, type: {}, id: {:x}",
					location->GetFullName(), location->GetFormType(),
					location->GetFormID());
				auto parentLocation = location->parentLocation;
				if (parentLocation) {
					logger::info("parentLocation: {}, type: {}, id: {:x}",
						parentLocation->GetFullName(),
						parentLocation->GetFormType(),
						parentLocation->GetFormID());
					auto grandParentLocation = parentLocation->parentLocation;
					if (grandParentLocation) {
						logger::info(
							"grandParentLocation: {}, type: {}, id: {:x}",
							grandParentLocation->GetFullName(),
							grandParentLocation->GetFormType(),
							grandParentLocation->GetFormID());
						auto greatGrandparentLocation =
							grandParentLocation->parentLocation;
						if (greatGrandparentLocation) {
							logger::info(
								"greatGrandparentLocation: {}, type: {}, id: "
								"{:x}",
								greatGrandparentLocation->GetFullName(),
								greatGrandparentLocation->GetFormType(),
								greatGrandparentLocation->GetFormID());
							auto greatGreatGrandparentLocation =
								greatGrandparentLocation->parentLocation;
							if (greatGreatGrandparentLocation) {
								logger::info(
									"greatGreatGrandparentLocation: {}, type: "
									"{}, id: {:x}",
									greatGreatGrandparentLocation
										->GetFullName(),
									greatGreatGrandparentLocation
										->GetFormType(),
									greatGreatGrandparentLocation->GetFormID());
								auto greatGreatGreatGrandparentLocation =
									greatGreatGrandparentLocation
										->parentLocation;
								if (greatGreatGreatGrandparentLocation) {
									logger::info(
										"greatGreatGreatGrandparentLocation: "
										"{}, type: {}, id: {:x}",
										greatGreatGreatGrandparentLocation
											->GetFullName(),
										greatGreatGreatGrandparentLocation
											->GetFormType(),
										greatGreatGreatGrandparentLocation
											->GetFormID());
								}
							}
						}
					}
				}
			}

			if (location &&
				strcmp(location->GetFullName(), "Landing Area") == 0 &&
				location->parentLocation) {
				location = location->parentLocation.get();
			}

			return location;
		}

		RE::BGSLocation* GetParentLocation(RE::BGSLocation* location)
		{
			auto        parentLocation = location->parentLocation.get();
			std::string locationName = location->GetFullName();

			while (parentLocation &&
				   locationName.find(parentLocation->GetFullName()) !=
					   std::string::npos) {
				parentLocation = parentLocation->parentLocation.get();
			}
			return parentLocation;
		}

		std::string GetParentLocationName(RE::BGSLocation* location)
		{
			std::string parentLocationName{};

			auto parentLocation = GetParentLocation(location);

			if (parentLocation) {
				parentLocationName = parentLocation->GetFullName();

				// check if parent location is a system and add "System" to the name
				if (parentLocation->HasKeywordString("LocTypeStarSystem")) {
						parentLocationName = Translations::fmt(
							Translations::Keys::SystemNameTemplate,
							parentLocationName,
							Translations::strings[Translations::Keys::System]);
				}
			}

			return parentLocationName;
		}

		std::string BuildDetailsString(
			RE::TESActorBaseData* playerActorBaseData)
		{
			std::string details{};

			std::string playerName =
				RE::PlayerCharacter::GetSingleton()->GetDisplayFullName();

			if (Settings::bShowCharacterName) {
				details = playerName;
			}
			if (Settings::bShowCharacterLevel) {
				if (Settings::bShowCharacterName) {
					details = Translations::fmt(
						Translations::Keys::CharacterNameAndLevelTemplate,
						playerName,
						Translations::strings[Translations::Keys::Level],
						playerActorBaseData->actorData.level);
				} else {
					details =
						Translations::fmt(Translations::Keys::LevelTemplate,
							Translations::strings[Translations::Keys::Level],
							playerActorBaseData->actorData.level);
				}
			}

			return details;
		}

		std::string BuildStateString()
		{
			std::string state{};

			auto        playerActor = RE::PlayerCharacter::GetSingleton();
			auto        spaceship = playerActor->GetSpaceship();
			bool        isInCombat = playerActor->IsInCombat();
			auto        playerLocation = GetLocation();
			auto        parentLocation = GetParentLocation(playerLocation);
			std::string playerLocationName =
				playerLocation ? playerLocation->GetFullName() : "";
			std::string playerParentLocationName =
				GetParentLocationName(playerLocation);

			/*if (playerActor->IsInSpace(false))
            {
                logger::debug("player is in space");
                auto playerLocationName         = GetPlayerLocationName(playerActor);
                auto playerParentWorldSpaceName = GetPlayerParentWorldSpaceName(playerActor);
                logger::debug("playerLocationName: {}", playerLocationName);
                logger::debug("playerParentWorldSpaceName: {}", playerParentWorldSpaceName);

                state = Translations::strings[Translations::Keys::InSpace];
            }*/

			if (spaceship) {  // player is in a spaceship or station, if location name is Ship then it is the player's ship
				std::string shipName = spaceship->GetDisplayFullName();
				auto        pilot = spaceship->GetSpaceshipPilot();
				bool        isPiloting = pilot && pilot == playerActor;
				bool        isInPlayerShip = playerLocationName == "Ship";

				if (isInPlayerShip) {
					playerLocationName =
						Settings::bShowShipName ?
							shipName :
							Translations::strings[Translations::Keys::Ship];
				}

				if (isInCombat) {
					state = Translations::fmt(
						Translations::Keys::FightingOnShipTemplate,
						Translations::strings[Translations::Keys::Fighting],
						Translations::strings[Translations::Keys::On],
						playerLocationName);
				} else if (isPiloting) {
					state =
						isInPlayerShip && !Settings::bShowShipName ?
						Translations::strings[Translations::Keys::PilotingSpaceship] :
							Translations::fmt(
								Translations::Keys::PilotingShipNameTemplate,
								Translations::strings
									[Translations::Keys::PilotingSpaceship],
								playerLocationName);
				} else if (isInPlayerShip) {
					if (Settings::bShowShipName) {
						state = Translations::fmt(
							Translations::Keys::InSpaceshipNameTemplate,
							Translations::strings
								[Translations::Keys::InSpaceship],
							playerLocationName);
					} else {
						state = Translations::strings
							[Translations::Keys::InSpaceship];
					}
				} else {
					if (!playerParentLocationName.empty()) {
						state = Translations::fmt(
							Translations::Keys::
								ExploringSpaceshipNameWithParentTemplate,
							Translations::strings
								[Translations::Keys::Exploring],
							playerLocationName, playerParentLocationName);
					} else {
						state = Translations::fmt(
							Translations::Keys::ExploringSpaceshipNameTemplate,
							Translations::strings
								[Translations::Keys::Exploring],
							playerLocationName);
					}
				}
			} else {  // player is on a planet
				if (isInCombat) {
					state = Translations::fmt(
						Translations::Keys::FightingOnPlanetTemplate,
						Translations::strings[Translations::Keys::Fighting],
						parentLocation->HasKeywordString("LocTypeStarSystem") ?
							Translations::strings[Translations::Keys::On] :
							Translations::strings[Translations::Keys::At],
						playerLocationName, playerParentLocationName);
				} else {
					if (!playerParentLocationName.empty()) {
						state = Translations::fmt(
							Translations::Keys::
								ExploringLocationWithParentTemplate,
							Translations::strings
								[Translations::Keys::Exploring],
							playerLocationName, playerParentLocationName);
					} else {
						state = Translations::fmt(
							Translations::Keys::ExploringLocationTemplate,
							Translations::strings
								[Translations::Keys::Exploring],
							playerLocationName);
					}
				}
			}

			return state;
		}

		void SetPresenceWithCompanion(std::string state, std::string details, RE::Actor* companion)
		{
			if (companion) {
				auto& companionData = Resources::companionDataMap.at(companion->GetFormID());
				if (!companionData.smallImageKey.empty()) {
					Discord::SetPresence(state, details,
						companionData.smallImageKey,
						Translations::fmt(
							Translations::Keys::ActiveFollowerTemplate,
							Translations::strings
								[Translations::Keys::TravellingWith],
							companion->GetDisplayFullName()));
					return;
				}
			}

			Discord::SetPresence(state, details);
		}

		bool SetPresenceBasedOnGameState(RE::Actor* companion)
		{
			auto playerActor = RE::PlayerCharacter::GetSingleton();

			if (!playerActor) {
				return false;
			}

			auto playerActorBaseData = GetPlayerActorBaseData();

			if (!playerActorBaseData) {
				return false;
			}

			if (companion) {
				SetPresenceWithCompanion(BuildStateString(),
					BuildDetailsString(playerActorBaseData), companion);
			} else {
				Discord::SetPresence(BuildStateString(),
					BuildDetailsString(playerActorBaseData));
			}

			return true;
		}

		bool SetPresenceBasedOnMenuState(RE::Actor* companion)
		{
			auto ui = RE::UI::GetSingleton();

			for (const auto& menuEntry : Translations::menuEntries) {
				if (ui->IsMenuOpen(menuEntry.menuName)) {
					logger::debug("Menu {} is open", menuEntry.menuName);

					std::string state = menuEntry.menuText;

					if (state.empty()) {
						// do not change presence
						return true;
					}

					if (menuEntry.shouldShowLocation &&
						Settings::bShowLocationInMenu) {
						auto location = GetLocation();

						if (location) {
							state = Translations::fmt(
								Translations::Keys::MenuWithLocationTemplate,
								state, location->GetFullName());
						}
					}

					if (menuEntry.shouldShowDetails) {
						auto playerActor = RE::PlayerCharacter::GetSingleton();
						auto playerActorBaseData = GetPlayerActorBaseData();

						if (!playerActorBaseData || !playerActor) {
							Discord::SetState(state);
						} else if (companion) {
							SetPresenceWithCompanion(state,
								BuildDetailsString(playerActorBaseData),
								companion);
						} else {
							Discord::SetPresence(state,
								BuildDetailsString(playerActorBaseData));
						}
					} else {
						Discord::SetState(state);
					}

					// handled and in menu
					return true;
				}
			}

			return false;
		}
	}  // namespace

	void HandleUpdate()
	{
		logger::info("Handling update");
		auto companion = GetActiveCompanion();

		auto handled = SetPresenceBasedOnMenuState(companion);
		if (!handled) {
			// not in a menu, handle as load
			SetPresenceBasedOnGameState(companion);
		}
	}
}  // namespace PresenceManager

/*
#include "PresenceManager.h"

#include "Constants.h"
#include "Discord.h"
#include "Settings.h"
#include "Translations.h"

static RE::TESActorBaseData* GetPlayerActorBaseData()
{
	RE::TESActorBaseData* playerActorBaseData{};
	auto                  playerFormBASE = RE::TESForm::LookupByID(0x00000007);

	if (playerFormBASE) {
		playerActorBaseData =
			starfield_cast<RE::TESActorBaseData*>(playerFormBASE);
	}

	return playerActorBaseData;
}

static RE::Actor* GetActiveCompanion()
{
	RE::Actor* companion{};

	for (const auto& companionRefID :
		std::views::keys(Resources::companionDataMap)) {
		auto actor = (RE::Actor*)RE::TESForm::LookupByID(companionRefID);
		if (actor &&
			actor->boolBits.all(RE::Actor::BOOL_BITS::kPlayerTeammate)) {
			companion = actor;
			break;
		}
	}

	if (companion) {
		logger::debug("companion: {}, id {:x}", companion->GetDisplayFullName(),
			companion->GetFormID());
	}

	return companion;
}

static RE::BGSLocation* GetLocation()
{
	RE::BGSLocation* location{};
	auto             playerActor = RE::PlayerCharacter::GetSingleton();

	if (playerActor) {
		location = playerActor->GetCurrentLocation();
	}

	if (location && Settings::bEnableDebug) {
		// DEBUG TREE

		logger::info("location: {}, type: {}, id: {:x}",
			location->GetFullName(), location->GetFormType(),
			location->GetFormID());
		auto parentLocation = location->parentLocation;
		if (parentLocation) {
			logger::info("parentLocation: {}, type: {}, id: {:x}",
				parentLocation->GetFullName(), parentLocation->GetFormType(),
				parentLocation->GetFormID());
			auto grandParentLocation = parentLocation->parentLocation;
			if (grandParentLocation) {
				logger::info("grandParentLocation: {}, type: {}, id: {:x}",
					grandParentLocation->GetFullName(),
					grandParentLocation->GetFormType(),
					grandParentLocation->GetFormID());
				auto greatGrandparentLocation =
					grandParentLocation->parentLocation;
				if (greatGrandparentLocation) {
					logger::info(
						"greatGrandparentLocation: {}, type: {}, id: "
						"{:x}",
						greatGrandparentLocation->GetFullName(),
						greatGrandparentLocation->GetFormType(),
						greatGrandparentLocation->GetFormID());
					auto greatGreatGrandparentLocation =
						greatGrandparentLocation->parentLocation;
					if (greatGreatGrandparentLocation) {
						logger::info(
							"greatGreatGrandparentLocation: {}, type: "
							"{}, id: {:x}",
							greatGreatGrandparentLocation->GetFullName(),
							greatGreatGrandparentLocation->GetFormType(),
							greatGreatGrandparentLocation->GetFormID());
						auto greatGreatGreatGrandparentLocation =
							greatGreatGrandparentLocation->parentLocation;
						if (greatGreatGreatGrandparentLocation) {
							logger::info(
								"greatGreatGreatGrandparentLocation: "
								"{}, type: {}, id: {:x}",
								greatGreatGreatGrandparentLocation
									->GetFullName(),
								greatGreatGreatGrandparentLocation
									->GetFormType(),
								greatGreatGreatGrandparentLocation
									->GetFormID());
						}
					}
				}
			}
		}
	}

	if (location && strcmp(location->GetFullName(), "Landing Area") == 0 &&
		location->parentLocation) {
		location = location->parentLocation.get();
	}

	return location;
}

static RE::BGSLocation* GetParentLocation(RE::BGSLocation* location)
{
	auto        parentLocation = location->parentLocation.get();
	std::string locationName = location->GetFullName();

	while (parentLocation && locationName.find(parentLocation->GetFullName()) !=
								 std::string::npos) {
		parentLocation = parentLocation->parentLocation.get();
	}
	return parentLocation;
}

static std::string GetParentLocationName(RE::BGSLocation* location)
{
	std::string parentLocationName{};

	auto parentLocation = GetParentLocation(location);

	if (parentLocation) {
		parentLocationName = parentLocation->GetFullName();

		// check if parent location is a system and add "System" to the name
		if (parentLocation->HasKeywordString("LocTypeStarSystem")) {
			parentLocationName = Translations::fmt(
				Translations::Keys::SystemNameTemplate, parentLocationName,
				Translations::strings[Translations::Keys::System]);
		}
	}

	return parentLocationName;
}

static std::string BuildDetailsString(RE::TESActorBaseData* playerActorBaseData)
{
	std::string details{};

	std::string playerName =
		RE::PlayerCharacter::GetSingleton()->GetDisplayFullName();

	if (Settings::bShowCharacterName) {
		details = playerName;
	}
	if (Settings::bShowCharacterLevel) {
		if (Settings::bShowCharacterName) {
			details = Translations::fmt(
				Translations::Keys::CharacterNameAndLevelTemplate, playerName,
				Translations::strings[Translations::Keys::Level],
				playerActorBaseData->actorData.level);
		} else {
			details = Translations::fmt(Translations::Keys::LevelTemplate,
				Translations::strings[Translations::Keys::Level],
				playerActorBaseData->actorData.level);
		}
	}

	return details;
}

static std::string BuildStateString()
{
	std::string state{};

	auto        playerActor = RE::PlayerCharacter::GetSingleton();
	auto        spaceship = playerActor->GetSpaceship();
	bool        isInCombat = playerActor->IsInCombat();
	auto        playerLocation = GetLocation();
	auto        parentLocation = GetParentLocation(playerLocation);
	std::string playerLocationName =
		playerLocation ? playerLocation->GetFullName() : "";
	std::string playerParentLocationName =
		GetParentLocationName(playerLocation);

	/*if (playerActor->IsInSpace(false))
    {
        logger::debug("player is in space");
        auto playerLocationName         = GetPlayerLocationName(playerActor);
        auto playerParentWorldSpaceName = GetPlayerParentWorldSpaceName(playerActor);
        logger::debug("playerLocationName: {}", playerLocationName);
        logger::debug("playerParentWorldSpaceName: {}", playerParentWorldSpaceName);

        state = Translations::strings[Translations::Keys::InSpace];
    }*

	if (spaceship) {  // player is in a spaceship or station, if location name is Ship then it is the player's ship
		std::string shipName = spaceship->GetDisplayFullName();
		auto        pilot = spaceship->GetSpaceshipPilot();
		bool        isPiloting = pilot && pilot == playerActor;
		bool        isInPlayerShip = playerLocationName == "Ship";

		if (isInPlayerShip) {
			playerLocationName =
				Settings::bShowShipName ?
					shipName :
					Translations::strings[Translations::Keys::Ship];
		}

		if (isInCombat) {
			state =
				Translations::fmt(Translations::Keys::FightingOnShipTemplate,
					Translations::strings[Translations::Keys::Fighting],
					Translations::strings[Translations::Keys::On],
					playerLocationName);
		} else if (isPiloting) {
			state = isInPlayerShip && !Settings::bShowShipName ?
			            Translations::strings
			                [Translations::Keys::PilotingSpaceship] :
			            Translations::fmt(
							Translations::Keys::PilotingShipNameTemplate,
							Translations::strings
								[Translations::Keys::PilotingSpaceship],
							playerLocationName);
		} else if (isInPlayerShip) {
			if (Settings::bShowShipName) {
				state = Translations::fmt(
					Translations::Keys::InSpaceshipNameTemplate,
					Translations::strings[Translations::Keys::InSpaceship],
					playerLocationName);
			} else {
				state = Translations::strings[Translations::Keys::InSpaceship];
			}
		} else {
			if (!playerParentLocationName.empty()) {
				state = Translations::fmt(
					Translations::Keys::
						ExploringSpaceshipNameWithParentTemplate,
					Translations::strings[Translations::Keys::Exploring],
					playerLocationName, playerParentLocationName);
			} else {
				state = Translations::fmt(
					Translations::Keys::ExploringSpaceshipNameTemplate,
					Translations::strings[Translations::Keys::Exploring],
					playerLocationName);
			}
		}
	} else {  // player is on a planet
		if (isInCombat) {
			state =
				Translations::fmt(Translations::Keys::FightingOnPlanetTemplate,
					Translations::strings[Translations::Keys::Fighting],
					parentLocation->HasKeywordString("LocTypeStarSystem") ?
						Translations::strings[Translations::Keys::On] :
						Translations::strings[Translations::Keys::At],
					playerLocationName, playerParentLocationName);
		} else {
			if (!playerParentLocationName.empty()) {
				state = Translations::fmt(
					Translations::Keys::ExploringLocationWithParentTemplate,
					Translations::strings[Translations::Keys::Exploring],
					playerLocationName, playerParentLocationName);
			} else {
				state = Translations::fmt(
					Translations::Keys::ExploringLocationTemplate,
					Translations::strings[Translations::Keys::Exploring],
					playerLocationName);
			}
		}
	}

	return state;
}

static void SetPresenceWithCompanion(std::string state, std::string details,
	RE::Actor* companion)
{
	if (companion) {
		auto& companionData =
			Resources::companionDataMap.at(companion->GetFormID());
		if (!companionData.smallImageKey.empty()) {
			Discord::SetPresence(state, details, companionData.smallImageKey,
				Translations::fmt(Translations::Keys::ActiveFollowerTemplate,
					Translations::strings[Translations::Keys::TravellingWith],
					companion->GetDisplayFullName()));
			return;
		}
	}

	Discord::SetPresence(state, details);
}

static bool SetPresenceBasedOnGameState(RE::Actor* companion)
{
	auto playerActor = RE::PlayerCharacter::GetSingleton();

	if (!playerActor) {
		return false;
	}

	auto playerActorBaseData = GetPlayerActorBaseData();

	if (!playerActorBaseData) {
		return false;
	}

	if (companion) {
		SetPresenceWithCompanion(BuildStateString(),
			BuildDetailsString(playerActorBaseData), companion);
	} else {
		Discord::SetPresence(BuildStateString(),
			BuildDetailsString(playerActorBaseData));
	}

	return true;
}

static bool SetPresenceBasedOnMenuState(RE::Actor* companion)
{
	auto ui = RE::UI::GetSingleton();

	for (const auto& menuEntry : Translations::menuEntries) {
		if (ui->IsMenuOpen(menuEntry.menuName)) {
			logger::debug("Menu {} is open", menuEntry.menuName);

			std::string state = menuEntry.menuText;

			if (state.empty()) {
				// do not change presence
				return true;
			}

			if (menuEntry.shouldShowLocation && Settings::bShowLocationInMenu) {
				auto location = GetLocation();

				if (location) {
					state = Translations::fmt(
						Translations::Keys::MenuWithLocationTemplate, state,
						location->GetFullName());
				}
			}

			if (menuEntry.shouldShowDetails) {
				auto playerActor = RE::PlayerCharacter::GetSingleton();
				auto playerActorBaseData = GetPlayerActorBaseData();

				if (!playerActorBaseData || !playerActor) {
					Discord::SetState(state);
				} else if (companion) {
					SetPresenceWithCompanion(state,
						BuildDetailsString(playerActorBaseData), companion);
				} else {
					Discord::SetPresence(state,
						BuildDetailsString(playerActorBaseData));
				}
			} else {
				Discord::SetState(state);
			}

			// handled and in menu
			return true;
		}
	}

	return false;
}

void PresenceManager::HandleUpdate()
{
	logger::info("Handling update");
	auto companion = GetActiveCompanion();

	auto handled = SetPresenceBasedOnMenuState(companion);
	if (!handled) {
		// not in a menu, handle as load
		SetPresenceBasedOnGameState(companion);
	}
}
*/
