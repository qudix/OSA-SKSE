#pragma once

namespace PapyrusObject
{
	using VM = RE::BSScript::IVirtualMachine;

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

	std::vector<RE::TESObjectREFR*> FindBed(
		RE::BSScript::IVirtualMachine* a_vm,
		RE::VMStackID a_stackID,
		RE::StaticFunctionTag*,
		RE::TESObjectREFR* a_centerRef,
		float a_radius,
		float a_sameFloor)
	{
		if (!a_centerRef) {
			a_vm->TraceStack("CenterRef is None", a_stackID);
			return {};
		}

		const auto handler = RE::TESDataHandler::GetSingleton();
		const auto keyword = handler->LookupForm(0xFD0E1, "Skyrim.esm"sv)->As<RE::BGSKeyword>();
		if (!handler || !keyword) {
			a_vm->TraceStack("Could not find Furniture Keyword", a_stackID);
			return {};
		}

		const auto originPos = a_centerRef->GetPosition();
		std::vector<RE::TESObjectREFR*> vec;
		util::iterate_attached_cells(originPos, a_radius * a_radius, [&](RE::TESObjectREFR& a_ref) {
			bool isType = a_ref.GetBaseObject()->Is(RE::FormType::Furniture);
			if (isType) {
				const auto refPos = a_ref.GetPosition();
				bool sameFloor = (a_sameFloor > 0.0) ? (std::fabs(originPos.z - refPos.z) <= a_sameFloor) : true;
				if (sameFloor && a_ref.HasKeyword(keyword) && IsBed(&a_ref))
					vec.push_back(&a_ref);
			}
			return true;
		});

		if (!vec.empty()) {
			std::sort(vec.begin(), vec.end(), [&](RE::TESObjectREFR* a_refA, RE::TESObjectREFR* a_refB) {
				return originPos.GetDistance(a_refA->GetPosition()) < originPos.GetDistance(a_refB->GetPosition());
			});
		}

		return vec;
	}

	std::vector<float> GetCoords(RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref)
	{
		if (!a_ref)
			return { 0.0f, 0.0f, 0.0f };

		return { a_ref->GetPositionX(), a_ref->GetPositionY(), a_ref->GetPositionZ() };
	}

	int GetFormID(RE::StaticFunctionTag*, RE::TESForm* a_form)
	{	
		return a_form ? a_form->GetFormID() : 0;
	}

	float GetWeight(RE::StaticFunctionTag*, RE::TESForm* a_form)
	{	
		return a_form ? a_form->GetWeight() : 0.0f;
	}

	std::string GetName(RE::StaticFunctionTag*, RE::TESForm* a_form)
	{
		return a_form ? a_form->GetName() : ""s;
	}

	std::string GetDisplayName(RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref)
	{
		return a_ref ? a_ref->GetDisplayFullName() : ""s;
	}

	float GetScaleFactor(RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref)
	{	
		return a_ref ? static_cast<float>(a_ref->refScale) / 100.0f : 0.0f;
	}

	RE::TESObjectREFR* GetLocationMarker(RE::StaticFunctionTag*, RE::BGSLocation* a_loc){
		if (!a_loc)
			return nullptr;

		auto marker = a_loc->worldLocMarker;

		return &*marker.get();
	}

	bool Bind(VM* a_vm)
	{
		const auto obj = "OSANative"sv;

		BIND(FindBed);
		BIND(GetLocationMarker, true);
		BIND(GetCoords, true);
		BIND(GetFormID, true);
		BIND(GetWeight, true);
		BIND(GetName, true);
		BIND(GetDisplayName, true);
		BIND(GetScaleFactor, true);

		return true;
	}
}
