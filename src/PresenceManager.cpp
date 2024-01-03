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

			// auto eliteCrewList = (RE::BGSListForm*)RE::TESForm::LookupByID(0x00133A67);

			for (const auto& companionRefId :
				std::views::keys(Resources::companionDataMap)) {
				auto actor =
					(RE::Actor*)RE::TESForm::LookupByID(companionRefId);
				if (actor && actor->boolBits.all(
								 RE::Actor::BOOL_BITS::kPlayerTeammate)) {
					companion = actor;
					break;
				}
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
				location = location->parentLocation;
			}

			return location;
		}

		RE::BGSLocation* GetParentLocation(RE::BGSLocation* location)
		{
			auto        parentLocation = location->parentLocation;
			std::string locationName = location->GetFullName();

			while (parentLocation &&
				   locationName.find(parentLocation->GetFullName()) !=
					   std::string::npos) {
				parentLocation = parentLocation->parentLocation;
			}
			return parentLocation;
		}

		bool IsLocationSystem(RE::BGSLocation* location)
		{
			return location && location->parentLocation &&
			       strcmp(location->parentLocation->GetFullName(),
					   "Universe") == 0;
		}

		std::string GetParentLocationName(RE::BGSLocation* location)
		{
			std::string parentLocationName{};

			auto parentLocation = GetParentLocation(location);

			if (parentLocation) {
				parentLocationName = parentLocation->GetFullName();

				// check if parent location is a system and add "System" to the name
				//auto tbo = RE::TBO_InstanceData();
				//auto IsSystemKeyword = (RE::BGSKeyword*)RE::TESForm::LookupByID(0x0000149F);
				//bool isSystem = parentLocation->HasKeyword(IsSystemKeyword, &tbo);
				//logger::info("isSystem: {}", isSystem);
				/*if (IsSystemKeyword &&
					parentLocation->AsReference()->HasKeyword(
						IsSystemKeyword))
				{
					parentLocationName = Translations::fmt(
						Translations::Keys::SystemNameTemplate,
						parentLocationName,
						Translations::strings[Translations::Keys::System]);
				}*/
				auto grandParentLocation = parentLocation->parentLocation;
				if (grandParentLocation) {
					std::string grandParentLocationName =
						grandParentLocation->GetFullName();
					if (IsLocationSystem(
							grandParentLocation))  // if the grand parent is the universe, then the parent is a system
					{
						parentLocationName = Translations::fmt(
							Translations::Keys::SystemNameTemplate,
							parentLocationName,
							Translations::strings[Translations::Keys::System]);
					}
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
					if (playerLocationName ==
						Translations::strings[Translations::Keys::Ship]) {
						state = Translations::strings
							[Translations::Keys::PilotingSpaceship];
					} else {
						state = Translations::fmt(
							Translations::Keys::PilotingShipNameTemplate,
							Translations::strings
								[Translations::Keys::PilotingSpaceship],
							playerLocationName);
					}
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
						IsLocationSystem(parentLocation) ?
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

		void SetPresenceWithCompanion(std::string state, std::string details,
			RE::Actor* companion)
		{
			std::uint32_t companionId = companion->GetFormID();
			auto          companionName = companion->GetDisplayFullName();

			logger::debug("companion: {}, id {:x}", companionName, companionId);

			if (companionId && companionName) {
				auto& companionData =
					Resources::companionDataMap.at(companionId);
				if (!companionData.smallImageKey.empty()) {
					Discord::SetPresence(state, details,
						companionData.smallImageKey,
						Translations::fmt(
							Translations::Keys::ActiveFollowerTemplate,
							Translations::strings
								[Translations::Keys::TravellingWith],
							companionName));
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
