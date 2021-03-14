#include "Util.h"

#define WIN32_LEAN_AND_MEAN

#define NOGDICAPMASKS
#define NOVIRTUALKEYCODES
//#define NOWINMESSAGES
#define NOWINSTYLES
#define NOSYSMETRICS
#define NOMENUS
#define NOICONS
#define NOKEYSTATES
#define NOSYSCOMMANDS
#define NORASTEROPS
#define NOSHOWWINDOW
#define OEMRESOURCE
#define NOATOM
#define NOCLIPBOARD
#define NOCOLOR
//#define NOCTLMGR
#define NODRAWTEXT
#define NOGDI
#define NOKERNEL
//#define NOUSER
#define NONLS
//#define NOMB
#define NOMEMMGR
#define NOMETAFILE
#define NOMINMAX
//#define NOMSG
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOTEXTMETRIC
#define NOWH
#define NOWINOFFSETS
#define NOCOMM
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX

#include <shlobj_core.h>

namespace Util
{
	std::optional<fs::path> GetDocumentsPath()
	{
		wchar_t* buffer{ nullptr };
		const auto result = ::SHGetKnownFolderPath(::FOLDERID_Documents, 0, nullptr, std::addressof(buffer));
		std::unique_ptr<wchar_t[], decltype(&::CoTaskMemFree)> knownPath(buffer, ::CoTaskMemFree);

		if (!knownPath || result != S_OK) {
			logger::error("failed to get known folder path"sv);
			return std::nullopt;
		}

		std::filesystem::path path = knownPath.get();
		path /= "My Games/Skyrim Special Edition"sv;

		return path;
	}

	std::optional<fs::path> GetDatabasePath()
	{
		auto path = GetDocumentsPath();
		if (!path)
			return std::nullopt;

		*path /= "JCUser/ODatabase.json"sv;

		return path;
	}

	std::vector<std::string> StringSplit(const std::string& text, char sep)
	{
		std::vector<std::string> tokens;
		size_t start = 0, end = 0;

		while ((end = text.find(sep, start)) != std::string::npos) {
			if (text.substr(start, end - start) != "")
				tokens.push_back(text.substr(start, end - start));

			start = end + 1;
		}

		if (text.substr(start) != "")
			tokens.push_back(text.substr(start));

		return tokens;
	}
}
