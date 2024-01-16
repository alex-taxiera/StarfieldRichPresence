#pragma once

namespace Plugin
{
    using namespace std::string_view_literals;

    static constexpr auto Name{ "StarfieldRichPresence"sv };
    static constexpr auto Author{ "Alex Taxiera"sv };
    static constexpr auto Version{
        REL::Version{0, 0, 1, 0}
    };
}

namespace SFSE
{
	constexpr REL::Version RUNTIME_SF_1_8_88(1, 8, 88, 0);
}

extern "C"
{
	__declspec(dllexport) SFSE::PluginVersionData SFSEPlugin_Version = {
		SFSE::PluginVersionData::kVersion,

		1,							  // version 1
		"Starfield Rich Presence",
		"Alex Taxiera",

		1,                              // yes address independent
		0,                              // not structure independent
		{ SFSE::RUNTIME_SF_1_8_88.pack(), 0 },

		0,  // works with any version of the script extender. you probably do not need to put anything here
		0, 0,  // set these reserved fields to 0
	};
};

//SFSEPluginVersion = []() noexcept {
//    SFSE::PluginVersionData data{};
//
//    data.PluginVersion(Plugin::Version);
//    data.PluginName(Plugin::Name);
//    data.AuthorName(Plugin::Author);
//    data.UsesAddressLibrary(true);
//    data.IsLayoutDependent(true);
//    data.CompatibleVersions({ SFSE::RUNTIME_LATEST });
//
//    return data;
//}();
