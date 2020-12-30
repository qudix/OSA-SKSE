#pragma once

// Derived from: https://github.com/powerof3/PapyrusExtenderSSE

void IterateOverAttachedCells(RE::TES* TES, const RE::NiPoint3& a_origin, float a_radius, std::function<bool(RE::TESObjectREFR&)> a_fn)
{
	auto cell = TES->interiorCell;
	if (cell) {
		cell->ForEachReferenceInRange(a_origin, a_radius, [&](RE::TESObjectREFR& ref) {
			if (!a_fn(ref))
				return false;

			return true;
		});
	} else {
		auto gridCellArray = TES->gridCells;
		if (gridCellArray) {
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
						if (cellCoords) {
							if (cellCoords->worldX < xPlus && (cellCoords->worldX + 4096.0) > xMinus && cellCoords->worldY < yPlus && (cellCoords->worldY + 4096.0) > yMinus) {
								cell->ForEachReferenceInRange(a_origin, a_radius, [&](RE::TESObjectREFR& a_cellRef) {
									if (!a_fn(a_cellRef)) {
										return false;
									}
									return true;
								});
							}
						}
					}
				}
			}
		}
	}
}
