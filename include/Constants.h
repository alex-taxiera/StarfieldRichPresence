#if !defined(MYLIB_CONSTANTS_H)
#define MYLIB_CONSTANTS_H 1

const std::string discordAppId = "1151867904082587709";
const DWORD sleepTime = 4000;

struct Text
{
    std::string LaunchingGame = "Launching Game";
    std::string Exploring     = "Exploring";
    std::string InMainMenu    = "In Main Menu";
    std::string Fighting      = "Fighting";
    std::string InSpace       = "In Space";
    std::string InSpaceship   = "In Spaceship";
    std::string Level         = "Lv";
};

struct Resources
{
    std::string StarfieldLogo = "sf_logo";
};

const Text text;
const Resources resources;

#endif
