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

typedef RE::TESObjectCELL*(__fastcall* _GetRefCell)(RE::TESObjectREFR* TargetRef);
REL::Relocation<_GetRefCell> GetRefCell(0x01A093BC);

namespace Hooks
{
    std::string discordAppId = "468591679599935490";
    std::string otherAppId   = "1151867904082587709";
    int64_t     StartTime    = 0;

    Strings Text;

    RE::TESForm* PlayerRefAsForm = NULL;
    RE::TESForm* PlayerFormBASE = NULL;
    RE::TESObjectREFR* PlayerRefAsRef = NULL;
    RE::TESActorBaseData* PlayerActorBaseData = NULL;
    RE::Actor* PlayerActor = NULL;

    std::thread* discordThread;

    void HandleDiscordReady(const DiscordUser* user)
    {
        logger::debug("Discord: ready");
        logger::debug("Discord: username: {}", user->username);
    }
    void HandleDiscordError(int errorCode, const char* message)
    {
        logger::debug("Discord: error {}: {}", errorCode, message);
    }
    void HandleDiscordDisconnect(int errorCode, const char* message)
    {
        logger::debug("Discord: disconnect {}: {}", errorCode, message);
    }

    void SetDiscordPresence(const char* state, const char* details)
    {
        DiscordRichPresence discordPresence;
        memset(&discordPresence, 0, sizeof(discordPresence));
        discordPresence.state   = state;
        discordPresence.details = details;

        if (Settings::displayTimeElapsed)
        {
            discordPresence.startTimestamp = StartTime;
        }

        //discordPresence.largeImageKey = AppLogoName.c_str();
        discordPresence.instance      = 0;

        Discord_UpdatePresence(&discordPresence);
    }

    bool InitializeDiscordPresence()
    {
        StartTime = time(0);
        DiscordEventHandlers handlers;
        memset(&handlers, 0, sizeof(handlers));
        handlers.ready        = HandleDiscordReady;
        handlers.errored      = HandleDiscordError;
        handlers.disconnected = HandleDiscordDisconnect;
        Discord_Initialize(otherAppId.c_str(), &handlers, 1, NULL);

        SetDiscordPresence("", Text.LaunchingGame.c_str());

        return true;
    }

    std::string Get_PlayerInfo()
    {
        std::string details{};
        if (Settings::displayCharacterName)
        {
            const char* PName = PlayerRefAsRef->GetDisplayFullName();

            if (PName)
            {
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
        if (PlayerRefAsRef->IsInSpace())
        {
            return "In space";
        }
        std::string state{};

        auto MyShip = PlayerRefAsRef->GetAttachedSpaceship();
        if (MyShip)
        {
            state = "In spaceship";

            if (Settings::displayShipName)
            {
                std::string sTemp = MyShip->GetDisplayFullName();

                if (sTemp.length() > 0)
                {
                    state += " (" + sTemp + ")";
                }
            }

            return state;
        }

        bool isInCombat = PlayerActor->IsInCombat();

        if (isInCombat)
        {
            state = Text.Fighting + " ";
        }
        else
        {
            state = Text.Exploring + " ";
        }

        auto RefCell = GetRefCell(PlayerRefAsRef);

        if (RefCell)
        {
            std::string cellName = RefCell->GetFullName();

            if (cellName.length() > 0)
            {
                state += cellName;
            }
        } else if (PlayerRefAsRef->parentCell)
        {
            std::string parentCellName = PlayerRefAsRef->parentCell->GetFullName(); // for interiors

            if (parentCellName.length() > 0)
            {
                state += parentCellName;
            }
        }

        return state;
    }

    void MainLoop()
    {
        (*discordThread).detach();

        for (;;)
        {
            Sleep(4000);

            logger::debug("DiscordPresence works");
            PlayerRefAsForm = RE::TESForm::LookupByID(20);

            if (!PlayerRefAsForm)
            {
                logger::debug("PlayerRefAsForm is not valid");
                continue;
            }

            PlayerRefAsRef = (RE::TESObjectREFR*)PlayerRefAsForm;

            if (!PlayerRefAsRef)
            {
                logger::debug("PlayerRefAsRef is not valid");
                continue;
            }

            PlayerActor = (RE::Actor*)PlayerRefAsRef;

            if (!PlayerActor)
            {
                logger::debug("PCActor is not valid");
                continue;
            }

            // PlayerFormBASE = LookupFormByID(7);
            PlayerFormBASE = RE::TESForm::LookupByID(7);

            if (!PlayerFormBASE)
            {
                logger::debug("PlayerFormBASE is not valid");
                continue;
            }

            PlayerActorBaseData = dynamic_cast<RE::TESActorBaseData*>(PlayerFormBASE);

            if (!PlayerActorBaseData)
            {
                logger::debug("PlayerActorBaseData is not valid");
                continue;
            }

            logger::debug("All valid - continuing");

            if (!PlayerRefAsRef->parentCell)
            {
                SetDiscordPresence("", Text.InMainMenu.c_str());
                continue;
            }

            std::string sState   = Get_PlayerState();
            std::string sDetails = Get_PlayerInfo();

            logger::debug("Player state>>%s", sState.c_str());
            logger::debug("Player details>>%s", sDetails.c_str());

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
        //SetDiscordPresence("", s_T_LaunchingGame.c_str());
        //auto initResult = discord::Core::Create(468591679599935490, DiscordCreateFlags_Default, &core);
        //if (initResult != discord::Result::Ok)
        //{
        //    logger::info("Failed to Initialize Discord SDK: {}", std::to_underlying(initResult));
        //    return;
        //}

        //logger::info("Hooks installed");

        //discord::Activity activity{};
        //activity.SetState("Testing");
        //activity.SetDetails("Fruit Loops");

        //core->ActivityManager().UpdateActivity(
        //    activity,
        //    [](discord::Result result) {
        //        if (result != discord::Result::Ok)
        //        {
        //            logger::info("activity set correctly");
        //        }
        //        else
        //        {
        //            logger::error("activity not ok: {}", std::to_underlying(result));
        //        }
        //    });
    }
} // namespace Hooks
