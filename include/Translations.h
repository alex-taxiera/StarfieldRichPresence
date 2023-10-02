#pragma once

class Translations : public Singleton<Translations>
{
public:
    static void LoadTranslations() noexcept;

    enum Keys
    {
        LaunchingGame,
        Exploring,
        Fighting,
        InSpace,
        InSpaceship,
        PilotingSpaceship,
        TravellingWith,
        System,
        Level,
        In,
        On,
        At,
        Ship,
        InMainMenu,
        PauseMenu,
        SleepWaitMenu,
        PickpocketMenu,
        LockpickingMenu,
        SkillsMenu,
        GalaxyStarMapMenu,
        PhotoModeMenu,
        PhotoGalleryMenu,
        BSMissionMenu,
        PowersMenu,
        InventoryMenu,
        ChargenMenu,
        SpaceshipEditorMenu,
        DataMenu,
        CharacterNameAndLevelTemplate,
        LevelTemplate,
        SystemNameTemplate,
    };

    inline static const std::map<Keys, const char*> translationKeys = {
        {LaunchingGame,                  "sLaunchingGame"                },
        { Exploring,                     "sExploring"                    },
        { Fighting,                      "sFighting"                     },
        { InSpace,                       "sInSpace"                      },
        { InSpaceship,                   "sInSpaceship"                  },
        { PilotingSpaceship,             "sPilotingSpaceship"            },
        { TravellingWith,                "sTravellingWith"               },
        { System,                        "sSystem"                       },
        { Level,                         "sLevel"                        },
        { In,                            "sIn"                           },
        { On,                            "sOn"                           },
        { At,                            "sAt"                           },
        { Ship,                          "sShip"                         },
        { InMainMenu,                    "sInMainMenu"                   },
        { PauseMenu,                     "sPauseMenu"                    },
        { SleepWaitMenu,                 "sSleepWaitMenu"                },
        { PickpocketMenu,                "sPickpocketMenu"               },
        { LockpickingMenu,               "sLockpickingMenu"              },
        { SkillsMenu,                    "sSkillsMenu"                   },
        { GalaxyStarMapMenu,             "sGalaxyStarMapMenu"            },
        { PhotoModeMenu,                 "sPhotoModeMenu"                },
        { PhotoGalleryMenu,              "sPhotoGalleryMenu"             },
        { BSMissionMenu,                 "sBSMissionMenu"                },
        { PowersMenu,                    "sPowersMenu"                   },
        { InventoryMenu,                 "sInventoryMenu"                },
        { ChargenMenu,                   "sChargenMenu"                  },
        { SpaceshipEditorMenu,           "sSpaceshipEditorMenu"          },
        { DataMenu,                      "sDataMenu"                     },
        { CharacterNameAndLevelTemplate, "sCharacterNameAndLevelTemplate"},
        { LevelTemplate,                 "sLevelTemplate"                },
        { SystemNameTemplate,            "sSystemNameTemplate"           },
    };
    inline static std::map<Keys, std::string> strings = {
        {LaunchingGame,                  "Launching game"          },
        { Exploring,                     "Exploring"               },
        { Fighting,                      "Fighting"                },
        { InSpace,                       "In outer space"          },
        { InSpaceship,                   "In ship"                 },
        { PilotingSpaceship,             "Piloting ship"           },
        { TravellingWith,                "Travelling with"         },
        { System,                        "System"                  },
        { Level,                         "Lv"                      },
        { In,                            "in"                      },
        { On,                            "on"                      },
        { At,                            "at"                      },
        { Ship,                          "ship"                    },
        { InMainMenu,                    "In main menu"            },
        { PauseMenu,                     "Paused"                  },
        { SleepWaitMenu,                 "Sleeping"                },
        { PickpocketMenu,                "Pickpocketing"           },
        { LockpickingMenu,               "Lockpicking"             },
        { SkillsMenu,                    "Checking perks"          },
        { GalaxyStarMapMenu,             "Gazing at starmap"       },
        { PhotoModeMenu,                 "Taking a photo"          },
        { PhotoGalleryMenu,              "Looking at photos"       },
        { BSMissionMenu,                 "Looking at the quest log"},
        { PowersMenu,                    "Looking at powers"       },
        { InventoryMenu,                 "Checking inventory"      },
        { ChargenMenu,                   "Creating a character"    },
        { SpaceshipEditorMenu,           "Looking at ships"        },
        { DataMenu,                      "In menus"                },
        { CharacterNameAndLevelTemplate, "{0} | {1} {2}"           },
        { LevelTemplate,                 "{0} {1}"                 },
        { SystemNameTemplate,            "{0} {1}"                 },
    };

    struct MenuEntry
    {
        const char* menuName           = NULL;
        std::string menuText           = "";
        bool        shouldShowLocation = true;
        bool        shouldShowDetails  = true;
    };

    inline static const MenuEntry menuEntries[16] = {
        { "LoadingMenu" }, // no menu text because it's only shown for a few seconds
        { "MainMenu", strings[InMainMenu], false, false },
        { "PauseMenu", strings[PauseMenu], false }, // System Menu
        { "SleepWaitMenu", strings[SleepWaitMenu] },
        { "PickpocketMenu", strings[PickpocketMenu] },
        { "LockpickingMenu", strings[LockpickingMenu] },
        { "SkillsMenu", strings[SkillsMenu] },
        { "GalaxyStarMapMenu", strings[GalaxyStarMapMenu], false },
        { "PhotoModeMenu", strings[PhotoModeMenu] },
        { "PhotoGalleryMenu", strings[PhotoGalleryMenu] },
        { "BSMissionMenu", strings[BSMissionMenu] },
        { "PowersMenu", strings[PowersMenu] },
        { "InventoryMenu", strings[InventoryMenu] },
        { "ChargenMenu", strings[ChargenMenu], false },
        { "SpaceshipEditorMenu", strings[SpaceshipEditorMenu] },
        { "DataMenu", strings[DataMenu] }, // last because there are sub menus
    };
};
