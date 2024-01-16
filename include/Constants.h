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
		{ 0x00005788, { .smallImageKey = "barrett_small" } }, // Barrett
		{ 0x00005986, { .smallImageKey = "sarah_morgan_small" } }, // Sarah Morgan
		{ 0x000059A9, { .smallImageKey = "andreja_small" } }, // Andreja
		{ 0x001631C5, { .smallImageKey = "sam_coe_small" } }, // Adoring Fan
		{ 0x0029D488, { .smallImageKey = "sam_coe_small" } }, // Sam Coe
		// elite crew
		{ 0x00299F66, { .smallImageKey = "andreja_small" } },  // Amelia Earhart
		{ 0x0016B3D0, { .smallImageKey = "andreja_small" } },  // Andromeda Kepler
		{ 0x001F0262, { .smallImageKey = "andreja_small" } },  // Autumn MacMillan
		{ 0x0020DC69, { .smallImageKey = "andreja_small" } },  // Betty Howser
		{ 0x001CDAFC, { .smallImageKey = "andreja_small" } },  // Dani Garcia
		{ 0x0022198C, { .smallImageKey = "andreja_small" } },  // Erick Von Price
		{ 0x0017A859, { .smallImageKey = "andreja_small" } },  // Ezekiel
		{ 0x00015064, { .smallImageKey = "andreja_small" } },  // Gideon Aker
		{ 0x002B17C4, { .smallImageKey = "andreja_small" } },  // Hadrian
		{ 0x0000563C, { .smallImageKey = "andreja_small" } },  // Heller
		{ 0x001D898E,
			{ .smallImageKey = "andreja_small" } },  // Jessamine Griffin
		{ 0x00005639, { .smallImageKey = "andreja_small" } },  // Supervisor Lin
		{ 0x0017A858, { .smallImageKey = "andreja_small" } },  // Lyle Brewer
		{ 0x00015062, { .smallImageKey = "andreja_small" } },  // Marika Boros
		{ 0x0016D16A, { .smallImageKey = "andreja_small" } },  // Mickey Caviar
		{ 0x0029C982, { .smallImageKey = "andreja_small" } },  // Moara Otero
		{ 0x001593F8, { .smallImageKey = "andreja_small" } },  // Omari Hassan
		{ 0x001A9054, { .smallImageKey = "andreja_small" } },  // You?
		{ 0x000C4632, { .smallImageKey = "andreja_small" } },  // Rafael Aguerro
		{ 0x001A0CB1,
			{ .smallImageKey = "andreja_small" } },  // Dr. Rosie Tannehill
		{ 0x00015063,
			{ .smallImageKey = "andreja_small" } },  // Simeon Bankowski
		{ 0x00147954, { .smallImageKey = "andreja_small" } },  // Sophia Grace
		{ 0x000057BE, { .smallImageKey = "andreja_small" } },  // Vasco
	};
};

#endif
