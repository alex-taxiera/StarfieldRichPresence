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
			auto playerFormBASE = RE::TESForm::LookupByID(0x00000007);

			if (!playerFormBASE) {
				return NULL;
			}

			auto playerActorBaseData = starfield_cast<RE::TESActorBaseData*>(playerFormBASE);

			if (!playerActorBaseData) {
				return NULL;
			}

			return playerActorBaseData;
		}

		RE::Actor* GetActiveCompanion()
		{
			RE::Actor* companion{};

			// auto eliteCrewList = (RE::BGSListForm*)RE::TESForm::LookupByID(0x00133A67);

			for (const auto& companionRefId : std::views::keys(Resources::companionDataMap)) {
				auto actor = (RE::Actor*)RE::TESForm::LookupByID(companionRefId);
				if (actor && actor->boolBits.all(RE::Actor::BOOL_BITS::kPlayerTeammate)) {
					companion = actor;
					break;
				}
			}

			return companion;
		}

		RE::BGSLocation* GetRealLocation()
		{
			RE::BGSLocation* location = RE::PlayerCharacter::GetSingleton()->GetCurrentLocation();

			if (location) {
				// DEBUG TREE

				logger::debug("location: {}, type: {}, id: {:x}", location->GetFullName(), location->GetFormType(), location->GetFormID());
				auto parentLocation = location->parentLocation;
				if (parentLocation) {
					logger::debug("parentLocation: {}, type: {}, id: {:x}", parentLocation->GetFullName(), parentLocation->GetFormType(), parentLocation->GetFormID());
					auto grandParentLocation = parentLocation->parentLocation;
					if (grandParentLocation) {
						logger::debug("grandParentLocation: {}, type: {}, id: {:x}", grandParentLocation->GetFullName(), grandParentLocation->GetFormType(),
							grandParentLocation->GetFormID());
						auto greatGrandparentLocation = grandParentLocation->parentLocation;
						if (greatGrandparentLocation) {
							logger::debug("greatGrandparentLocation: {}, type: {}, id: {:x}", greatGrandparentLocation->GetFullName(), greatGrandparentLocation->GetFormType(),
								greatGrandparentLocation->GetFormID());
							auto greatGreatGrandparentLocation = greatGrandparentLocation->parentLocation;
							if (greatGreatGrandparentLocation) {
								logger::debug("greatGreatGrandparentLocation: {}, type: {}, id: {:x}", greatGreatGrandparentLocation->GetFullName(),
									greatGreatGrandparentLocation->GetFormType(), greatGreatGrandparentLocation->GetFormID());
								auto greatGreatGreatGrandparentLocation = greatGreatGrandparentLocation->parentLocation;
								if (greatGreatGreatGrandparentLocation) {
									logger::debug("greatGreatGreatGrandparentLocation: {}, type: {}, id: {:x}", greatGreatGreatGrandparentLocation->GetFullName(),
										greatGreatGreatGrandparentLocation->GetFormType(), greatGreatGreatGrandparentLocation->GetFormID());
								}
							}
						}
					}
				}
			}

			if (location && strcmp(location->GetFullName(), "Landing Area") == 0 && location->parentLocation) {
				location = location->parentLocation;
			}

			return location;
		}

		std::string GetPlayerLocationName()
		{
			std::string locationName{};

			auto location = GetRealLocation();

			if (location) {
				locationName = location->GetFullName();
			}
			return locationName;
		}

		std::string GetPlayerParentLocationName()
		{
			std::string parentLocationName{};

			auto location = GetRealLocation();

			if (location) {
				auto parentLocation = location->parentLocation;

				if (parentLocation) {
					parentLocationName = parentLocation->GetFullName();

					if (parentLocationName == location->GetFullName() && parentLocation->parentLocation) {
						parentLocation = parentLocation->parentLocation;
						parentLocationName = parentLocation->GetFullName();
					}

					// check if parent location is a system and add "System" to the name
					// auto IsSystemKeyword = (RE::BGSKeyword*)RE::TESForm::LookupByID(0x0000149F);
					// if (IsSystemKeyword && parentLocation->HasKeyword(IsSystemKeyword))
					//{
					//    parentLocationName = std::format("{} {}", parentLocationName, Translations::strings[Translations::Keys::System]);
					//}
					auto grandParentLocation = parentLocation->parentLocation;
					if (grandParentLocation) {
						std::string grandParentLocationName = grandParentLocation->GetFullName();
						if (strcmp(grandParentLocation->GetFullName(), "Universe") == 0)  // if the grand parent is the universe, then the parent is a system
						{
							parentLocationName = std::vformat(Translations::strings[Translations::Keys::SystemNameTemplate],
								std::make_format_args(parentLocationName, Translations::strings[Translations::Keys::System]));
						}
					}
				}
			}

			return parentLocationName;
		}

		std::string BuildDetailsString(RE::TESActorBaseData* playerActorBaseData)
		{
			std::string details{};

			std::string playerName = RE::PlayerCharacter::GetSingleton()->GetDisplayFullName();

			if (Settings::bShowCharacterName) {
				details = playerName;
			}
			if (Settings::bShowCharacterLevel) {
				if (Settings::bShowCharacterName) {
					details = std::vformat(Translations::strings[Translations::Keys::CharacterNameAndLevelTemplate],
						std::make_format_args(playerName, Translations::strings[Translations::Keys::Level], playerActorBaseData->actorData.level));
				} else {
					details = std::vformat(Translations::strings[Translations::Keys::LevelTemplate],
						std::make_format_args(Translations::strings[Translations::Keys::Level], playerActorBaseData->actorData.level));
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
			std::string playerLocationName = GetPlayerLocationName();
			std::string playerParentLocationName = GetPlayerParentLocationName();

			/*if (playerActor->IsInSpace(false))
            {
                logger::debug("player is in space");
                auto playerLocationName         = GetPlayerLocationName(playerActor);
                auto playerParentWorldSpaceName = GetPlayerParentWorldSpaceName(playerActor);
                logger::debug("playerLocationName: {}", playerLocationName);
                logger::debug("playerParentWorldSpaceName: {}", playerParentWorldSpaceName);

                state = Translations::strings[Translations::Keys::InSpace];
            }*/

			if (spaceship) {
				std::string shipName = spaceship->GetDisplayFullName();
				auto        pilot = spaceship->GetSpaceshipPilot();
				bool        isPiloting = pilot && pilot == playerActor;
				bool        isInPlayerShip = playerLocationName == "Ship";

				if (isInCombat) {
					state = std::format("{} {} {}", Translations::strings[Translations::Keys::Fighting], Translations::strings[Translations::Keys::On],
						!isInPlayerShip || Settings::bShowShipName ? shipName : Translations::strings[Translations::Keys::Ship]);
				} else if (isPiloting) {
					if (isInPlayerShip && Settings::bShowShipName) {
						state = std::vformat(Translations::strings[Translations::Keys::PilotingShipNameTemplate],
							std::make_format_args(Translations::strings[Translations::Keys::PilotingSpaceship], shipName));
					} else {
						state = Translations::strings[Translations::Keys::PilotingSpaceship];
					}
				} else if (isInPlayerShip) {
					if (Settings::bShowShipName) {
						state = std::vformat(Translations::strings[Translations::Keys::InSpaceshipNameTemplate],
							std::make_format_args(Translations::strings[Translations::Keys::InSpaceship], shipName));
					} else {
						state = Translations::strings[Translations::Keys::InSpaceship];
					}
				} else {
					state = std::vformat(Translations::strings[Translations::Keys::ExploringSpaceshipNameTemplate],
						std::make_format_args(Translations::strings[Translations::Keys::Exploring], shipName));
				}
			} else {
				if (isInCombat) {
					if (!playerParentLocationName.empty()) {
						state = std::format("{} {} {} | {}", Translations::strings[Translations::Keys::Fighting], Translations::strings[Translations::Keys::At], playerLocationName,
							playerParentLocationName);
					} else {
						state = std::format("{} {} {}", Translations::strings[Translations::Keys::Fighting], Translations::strings[Translations::Keys::On], playerLocationName);
					}
				} else {
					logger::debug("{} | {}", playerLocationName, playerParentLocationName);

					if (!playerParentLocationName.empty() && Settings::bShowPlanetWhileOutside) {
						state = std::format("{} {} | {}", Translations::strings[Translations::Keys::Exploring], playerLocationName, playerParentLocationName);
					} else {
						state = std::vformat(Translations::strings[Translations::Keys::ExploringLocationTemplate],
							std::make_format_args(Translations::strings[Translations::Keys::Exploring], playerLocationName));
					}
				}
			}

			return state;
		}

		void SetPresenceWithCompanion(std::string state, std::string details, RE::Actor* companion)
		{
			std::uint32_t companionId = companion->GetFormID();
			auto          companionName = companion->GetDisplayFullName();

			logger::debug("companion: {}, id {:x}", companionName, companionId);

			if (companionId && companionName) {
				auto& companionData = Resources::companionDataMap.at(companionId);
				if (!companionData.smallImageKey.empty()) {
					Discord::SetPresence(state, details, companionData.smallImageKey,
						std::vformat(Translations::strings[Translations::Keys::ActiveFollowerTemplate],
							std::make_format_args(Translations::strings[Translations::Keys::TravellingWith], companionName)));
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
				SetPresenceWithCompanion(BuildStateString(), BuildDetailsString(playerActorBaseData), companion);
			} else {
				Discord::SetPresence(BuildStateString(), BuildDetailsString(playerActorBaseData));
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

					auto playerActor = RE::PlayerCharacter::GetSingleton();

					if (menuEntry.shouldShowLocation && playerActor) {
						auto locationName = GetPlayerLocationName();

						if (!locationName.empty()) {
							bool playerIsInside = playerActor->parentCell;
							state = std::format("{} | {}", state, locationName);
						}
					}

					if (menuEntry.shouldShowDetails) {
						auto playerActorBaseData = GetPlayerActorBaseData();

						if (!playerActorBaseData || !playerActor) {
							Discord::SetState(state);
						} else if (companion) {
							SetPresenceWithCompanion(state, BuildDetailsString(playerActorBaseData), companion);
						} else {
							Discord::SetPresence(state, BuildDetailsString(playerActorBaseData));
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
