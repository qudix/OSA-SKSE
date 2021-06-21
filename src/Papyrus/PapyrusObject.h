#pragma once

namespace Papyrus::Object
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

	std::vector<RE::TESObjectREFR*> FindBed(RE::StaticFunctionTag*, RE::TESObjectREFR* a_centerRef, float a_radius, float a_sameFloor)
	{
		std::vector<RE::TESObjectREFR*> vec;

		if (!a_centerRef)
			return vec;

		const auto handler = RE::TESDataHandler::GetSingleton();
		const auto keyword = handler->LookupForm(0xFD0E1, "Skyrim.esm"sv)->As<RE::BGSKeyword>();
		if (!handler || !keyword)
			return vec;

		auto TES = RE::TES::GetSingleton();
		if (TES) {
			const auto originPos = a_centerRef->GetPosition();

			util::iterate_attached_cells(TES, originPos, a_radius * a_radius, [&](RE::TESObjectREFR& a_ref) {
				bool isType = a_ref.GetBaseObject()->Is(RE::FormType::Furniture);
				if (isType) {
					const auto refPos = a_ref.GetPosition();
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

	std::vector<RE::TESNPC*> LookupRelationshipPartners(RE::StaticFunctionTag*, RE::Actor* act, RE::BGSAssociationType* at){
		std::vector<RE::TESNPC*> ret;

		if (!act || !at){
			return ret;
		}

		RE::TESNPC* base = act->GetActorBase();



		for (auto relationship : *base->relationships) {

			if (relationship->assocType == at){
				if (relationship->npc1 == base){
					ret.push_back(relationship->npc2);
				} else if(relationship->npc2 == base){
					ret.push_back(relationship->npc1);
				}
			}


		}
		return ret;

	}

	RE::Actor* GetActorFromBase(RE::StaticFunctionTag*, RE::TESNPC* input){
		RE::Actor* ret = nullptr;
		if (!input){
			return ret;
		}

		const auto& [map, lock] = RE::TESForm::GetAllForms();

		const RE::BSReadWriteLock l{ lock };

		//auto forms = map;
		int b = 0;
		for (auto thing : *map) {
			auto form = thing.second;

			if (form->GetSavedFormType() == RE::FormType::ActorCharacter){
				b += 1;
				RE::Actor* act = form->As<RE::Actor>();
				if (act){
					if (act->GetActorBase() == input){
						return act;
					}
				}
			}
		}

		logger::info("{}", b);
		return ret;
	}

	RE::TESNPC* GetLeveledActorBase(RE::StaticFunctionTag*, RE::Actor* input){
		if (!input){
			return nullptr;
		}
		return input->GetActorBase();
	}

	int GetSex(RE::StaticFunctionTag*, RE::TESNPC* input){
		if (!input){
			return -1;
		}

		auto sex = input->GetSex();

		switch (sex)
		{
			case (RE::SEX::kMale):
				return 0;
			case (RE::SEX::kFemale):
				return 1;
		}

		return -1;
	}


	void Bind(VM& a_vm)
	{
		const auto obj = "OSANative"sv;

		BIND(FindBed);
		BIND(LookupRelationshipPartners);
		BIND(GetActorFromBase);
		BIND(GetLeveledActorBase);
		BIND(GetSex);
	}
}