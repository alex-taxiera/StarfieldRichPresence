#if !defined(MYLIB_CONSTANTS_H)
#	define MYLIB_CONSTANTS_H 1

struct CompanionData
{
	std::string smallImageKey;
};

namespace Constants
{
	const std::string discordAppId = "1158402634152161450";
	const float       updateDebounceTime = 3000;
}

namespace Resources
{
	constexpr const char* StarfieldLogo = "constellation";
	const std::map<std::uint32_t, CompanionData> companionDataMap = {
		{ 0x000059A9, { .smallImageKey = "andreja_small" } },
		{ 0x00005986, { .smallImageKey = "sarah_morgan_small" } },
		{ 0x00005788, { .smallImageKey = "barrett_small" } },
		{ 0x0029D488, { .smallImageKey = "sam_coe_small" } },
	};
};

#endif
