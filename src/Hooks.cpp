#include "Hooks.h"
#include "Settings.h"

//using namespace RE;

// map of constant strings
struct Strings
{
    std::string LaunchingGame = "Launching Game";
    std::string Exploring     = "Exploring";
    std::string InMainMenu    = "In Main Menu";
    std::string Fighting      = "Fighting";
    std::string InSpace       = "In Space";
    std::string InSpaceship   = "In Spaceship";
};

struct Resources
{
    std::string StarfieldLogo = "sf_logo";
};

typedef RE::TESObjectCELL*(__fastcall* _GetRefCell)(RE::TESObjectREFR* TargetRef);
REL::Relocation<_GetRefCell> GetRefCell{ REL::Offset(0x01A093BC) };

namespace Hooks
{
    std::string discordAppId = "468591679599935490";
    std::string otherAppId   = "1151867904082587709";
    int64_t     startTime    = 0;
    int64_t     sleepTime    = 4000;

    Strings Text;
    Resources resources;

    RE::TESForm* playerRefAsForm = NULL;
    RE::TESForm* playerFormBASE = NULL;
    RE::TESObjectREFR* playerRefAsRef = NULL;
    RE::TESActorBaseData* playerActorBaseData = NULL;
    RE::Actor* playerActor = NULL;

    std::thread* discordThread;

    void SetDiscordPresence(const char* state, const char* details)
    {
        DiscordRichPresence discordPresence;
        memset(&discordPresence, 0, sizeof(discordPresence));
        discordPresence.state   = state;
        discordPresence.details = details;

        if (Settings::displayTimeElapsed)
        {
            discordPresence.startTimestamp = startTime;
        }

        discordPresence.largeImageKey = resources.StarfieldLogo.c_str();
        discordPresence.instance      = 0;

        logger::debug("Presence state: {}", discordPresence.state);
        logger::debug("Presence details: {}", discordPresence.details);
        Discord_UpdatePresence(&discordPresence);
    }

    bool InitializeDiscordPresence()
    {
        startTime = time(0);
        DiscordEventHandlers handlers;
        memset(&handlers, 0, sizeof(handlers));
        Discord_Initialize(otherAppId.c_str(), &handlers, 1, NULL);

        SetDiscordPresence("", Text.LaunchingGame.c_str());

        return true;
    }

    std::string Get_PlayerInfo()
    {
        std::string details{};
        if (Settings::displayCharacterName)
        {
            logger::debug("getting player name");
            const char* PName = playerRefAsRef->GetDisplayFullName();

            if (PName)
            {
                logger::debug("got player name: {}", PName);
                details += PName;
            }
            else
            {
                details = "No name";
            }

            /*if (!bShowLVL)
            {
            }
            else
            {
                s_Details += " | ";
            }*/
        }
        /*if (bShowLVL)
        {
            int16_t Level = g_PlayerBaseData->actorData.sLevel;
            s_Details += s_T_LVL + ":" + to_string(Level);
        }*/

        return details;
    }

    std::string Get_PlayerState()
    {
        logger::debug("getting player is in space");
        if (playerRefAsRef->IsInSpace())
        {
            logger::debug("player is in space");
            return "In space";
        }

        std::string state{};

        logger::debug("trying to get spaceship");
        auto MyShip = playerRefAsRef->GetAttachedSpaceship();

        if (MyShip)
        {
            logger::debug("got spaceship");
            state = "In spaceship";

            if (Settings::displayShipName)
            {
                std::string sTemp = MyShip->GetDisplayFullName();
                logger::debug("got ship name: {}", sTemp.c_str());

                if (sTemp.length() > 0)
                {
                    state += " (" + sTemp + ")";
                }
            }

            return state;
        }

        logger::debug("trying to get combat status");
        bool isInCombat = playerActor->IsInCombat();

        if (isInCombat)
        {
            logger::debug("got combat status true");
            state = Text.Fighting + " ";
        }
        else
        {
            logger::debug("got combat status false");
            state = Text.Exploring + " ";
        }

        logger::debug("trying to get ref cell");
        auto RefCell = GetRefCell(playerRefAsRef);

        if (RefCell)
        {
            logger::debug("got ref cell");
            std::string cellName = RefCell->GetFullName();

            if (cellName.length() > 0)
            {
                logger::debug("got cell name: {}", cellName.c_str());
                state += cellName;
            }
        } else if (playerRefAsRef->parentCell)
        {
            logger::debug("got parent cell");
            std::string parentCellName = playerRefAsRef->parentCell->GetFullName(); // for interiors

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
        (*discordThread).detach();
        Settings::LoadSettings();

        for (;;)
        {
            logger::debug("sleeping for {}", sleepTime);
            Sleep(sleepTime);

            playerRefAsForm = RE::TESForm::LookupByID(20);

            if (!playerRefAsForm)
            {
                logger::debug("PlayerRefAsForm is not valid");
                continue;
            }

            playerRefAsRef = playerRefAsForm->AsReference();

            if (!playerRefAsRef)
            {
                logger::debug("PlayerRefAsRef is not valid");
                continue;
            }

            playerActor = (RE::Actor*)playerRefAsRef;

            if (!playerActor)
            {
                logger::debug("PCActor is not valid");
                continue;
            }

            // PlayerFormBASE = LookupFormByID(7);
            playerFormBASE = RE::TESForm::LookupByID(7);

            if (!playerFormBASE)
            {
                logger::debug("PlayerFormBASE is not valid");
                continue;
            }

            playerActorBaseData = starfield_cast<RE::TESActorBaseData*>(playerFormBASE);

            if (!playerActorBaseData)
            {
                logger::debug("PlayerActorBaseData is not valid");
                continue;
            }

            logger::debug("All valid - continuing");

            if (!playerRefAsRef->parentCell)
            {
                logger::debug("no parentCell, setting presence to main menu");
                SetDiscordPresence("", Text.InMainMenu.c_str());
                continue;
            }

            std::string sState   = Get_PlayerState();
            std::string sDetails = Get_PlayerInfo();

            SetDiscordPresence(sState.c_str(), sDetails.c_str());
        }
    }

    void Install() noexcept
    {
        bool ready = InitializeDiscordPresence();
        if (!ready)
        {
            logger::error("Failed to initialize Discord Rich Presence");
            return;
        }
        discordThread = new std::thread(MainLoop);
    }
} // namespace Hooks
