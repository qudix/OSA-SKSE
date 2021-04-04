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

namespace stl
{
	std::vector<std::string> string_split(const std::string& text, char sep)
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

namespace util
{
	std::optional<fs::path> documents_path()
	{
		wchar_t* buffer{ nullptr };
		const auto result = ::SHGetKnownFolderPath(::FOLDERID_Documents, 0, nullptr, std::addressof(buffer));
		std::unique_ptr<wchar_t[], decltype(&::CoTaskMemFree)> knownPath(buffer, ::CoTaskMemFree);

		if (!knownPath || result != S_OK) {
			logger::error("failed to get known folder path"sv);
			return std::nullopt;
		}

		fs::path path = knownPath.get();
		path /= "My Games/Skyrim Special Edition"sv;

		return path;
	}

	std::optional<fs::path> database_path()
	{
		auto path = documents_path();
		if (!path)
			return std::nullopt;

		*path /= "JCUser/ODatabase.json"sv;

		return path;
	}

	// Derived from: https://github.com/powerof3/PapyrusExtenderSSE
	void iterate_attached_cells(RE::TES* TES, const RE::NiPoint3& a_origin, float a_radius, std::function<bool(RE::TESObjectREFR&)> a_fn)
	{
		auto cell = TES->interiorCell;
		if (cell) {
			cell->ForEachReferenceInRange(a_origin, a_radius, [&](RE::TESObjectREFR& ref) { return a_fn(ref); });
			return;
		}

		auto gridCellArray = TES->gridCells;
		if (!gridCellArray)
			return;

		auto gridLength = gridCellArray->length;
		if (gridLength > 0) {
			std::uint32_t x = 0;
			std::uint32_t y = 0;
			float yPlus = a_origin.y + a_radius;
			float yMinus = a_origin.y - a_radius;
			float xPlus = a_origin.x + a_radius;
			float xMinus = a_origin.x - a_radius;

			for (x = 0, y = 0; (x < gridLength && y < gridLength); x++, y++) {
				cell = gridCellArray->GetCell(x, y);
				if (cell && cell->IsAttached()) {
					auto cellCoords = cell->GetCoordinates();
					if (!cellCoords)
						continue;

					float worldX = cellCoords->worldX;
					float worldY = cellCoords->worldY;

					if (worldX < xPlus && (worldX + 4096.0) > xMinus && worldY < yPlus && (worldY + 4096.0) > yMinus) {
						cell->ForEachReferenceInRange(a_origin, a_radius, [&](RE::TESObjectREFR& a_cellRef) { return a_fn(a_cellRef); });
					}
				}
			}
		}
	}
}
