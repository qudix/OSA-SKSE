#pragma once

namespace Papyrus
{
	using VM = RE::BSScript::IVirtualMachine;

	void BuildDB(RE::StaticFunctionTag*)
	{
		DB::Build();
	}

	bool IsBed(RE::TESObjectREFR* a_ref)
	{
		using TYPES = RE::BSFurnitureMarker::AnimationType;

		if (!a_ref)
			return false;

		std::string objectName = a_ref->GetDisplayFullName();
		if (objectName == "")
			return false;

		auto root = a_ref->Get3D();
		if (!root)
			return false;
			
		auto extra = root->GetExtraData("FRN");
		if (!extra)
			return false;

		auto node = netimmerse_cast<RE::BSFurnitureMarkerNode*>(extra);
		if (!node)
			return false;

		if (node->markers.empty())
			return false;

		auto types = node->markers[0].animationType;
		if (!types.all(TYPES::kSleep))
			return false;

		return true;
	}

	std::vector<RE::TESObjectREFR*> FindBed(RE::StaticFunctionTag*, RE::TESObjectREFR* a_centerRef, float a_radius, float a_sameFloor)
	{
		std::vector<RE::TESObjectREFR*> vec;

		if (!a_centerRef)
			return vec;

		auto handler = RE::TESDataHandler::GetSingleton();
		auto keyword = handler->LookupForm(0xFD0E1, "Skyrim.esm"sv)->As<RE::BGSKeyword>();
		if (!handler || !keyword)
			return vec;

		auto TES = RE::TES::GetSingleton();
		if (TES) {
			auto originPos = a_centerRef->GetPosition();
			auto squaredRadius = a_radius * a_radius;

			IterateOverAttachedCells(TES, originPos, squaredRadius, [&](RE::TESObjectREFR& a_ref) {
				if (a_ref.GetBaseObject()->Is(RE::FormType::Furniture)) {
					auto refPos = a_ref.GetPosition();
					bool sameFloor = (a_sameFloor > 0.0) ? (std::fabs(originPos.z - refPos.z) <= a_sameFloor) : true;
					if (sameFloor) {
						if (a_ref.HasKeyword(keyword) && IsBed(&a_ref)) {
							vec.push_back(&a_ref);
						}
					}
				}
				return true;
			});

			if (!vec.empty()) {
				std::sort(vec.begin(), vec.end(), [&](RE::TESObjectREFR* a_refA, RE::TESObjectREFR* a_refB) {
					return originPos.GetDistance(a_refA->GetPosition()) < originPos.GetDistance(a_refB->GetPosition());
				});
			}
		}

		return vec;
	}

	static constexpr char CLASS_NAME[] = "OSANative";

	bool Register(VM* a_vm)
	{
		a_vm->RegisterFunction("BuildDB", CLASS_NAME, BuildDB);
		a_vm->RegisterFunction("FindBed", CLASS_NAME, FindBed);
		return true;
	}
}
