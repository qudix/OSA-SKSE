#pragma once

namespace PapyrusActor
{
	using VM = RE::BSScript::IVirtualMachine;

	std::vector<RE::TESNPC*> LookupRelationshipPartners(RE::StaticFunctionTag*, RE::Actor* a_actor, RE::BGSAssociationType* a_type)
	{
		if (!a_actor || !a_type)
			return {};

		std::vector<RE::TESNPC*> ret;
		auto base = a_actor->GetActorBase();
		for (auto& relationship : *base->relationships) {
			if (relationship->assocType == a_type) {
				if (relationship->npc1 == base) {
					ret.push_back(relationship->npc2);
				} else if (relationship->npc2 == base) {
					ret.push_back(relationship->npc1);
				}
			}
		}

		return ret;
	}

	RE::Actor* GetActorFromBase(RE::StaticFunctionTag*, RE::TESNPC* a_base)
	{
		if (!a_base)
			return nullptr;

		const auto& [map, lock] = RE::TESForm::GetAllForms();
		const RE::BSReadWriteLock l{ lock };
		for (auto& [id, form] : *map) {
			if (form->GetSavedFormType() == RE::FormType::ActorCharacter) {
				auto actor = form->As<RE::Actor>();
				if (actor && actor->GetActorBase() == a_base) {
					return actor;
				}
			}
		}

		return nullptr;
	}

	// Below not offering speed improvements??
	// Qudix: You didn't set callable tasklets to true for them

	RE::TESNPC* GetLeveledActorBase(RE::StaticFunctionTag*, RE::Actor* a_actor)
	{
		return a_actor ? a_actor->GetActorBase() : nullptr;
	}

	int32_t GetSex(RE::StaticFunctionTag*, RE::TESNPC* a_base)
	{
		if (!a_base)
			return -1;

		auto sex = a_base->GetSex();
		switch (sex) {
		case (RE::SEX::kMale):
			return 0;
		case (RE::SEX::kFemale):
			return 1;
		}

		return -1;
	}

	bool Bind(VM* a_vm)
	{
		const auto obj = "OSANative"sv;

		BIND(LookupRelationshipPartners);
		BIND(GetActorFromBase);
		BIND(GetLeveledActorBase, true);
		BIND(GetSex, true);

		return true;
	}
}
