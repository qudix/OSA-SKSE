#pragma once

namespace PapyrusActor
{
	using VM = RE::BSScript::IVirtualMachine;

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

	RE::TESRace* GetRace(RE::StaticFunctionTag*, RE::TESNPC* a_base)
	{
		if (!a_base)
			return nullptr;

		return a_base->GetRace();
	}

	RE::BGSVoiceType* GetVoiceType(RE::StaticFunctionTag*, RE::TESNPC* a_base)
	{
		if (!a_base)
			return nullptr;

		return a_base->GetVoiceType();
	}

	std::vector<RE::Actor*> GetActors(RE::StaticFunctionTag*, RE::TESObjectREFR* a_centerRef, float a_radius)
	{
		std::vector<RE::Actor*> actors;

		auto pl = RE::ProcessLists::GetSingleton();
		if (a_centerRef && a_radius > 0) {
			const auto originPos = a_centerRef->GetPosition();
			for (auto& handle : pl->highActorHandles) {
				auto actor = handle.get().get();
				if (actor) {
					const auto actorPos = actor->GetPosition();
					if (util::within_radius(originPos, actorPos, a_radius))
						actors.push_back(actor);
				}
			}
		} else {
			for (auto& handle : pl->highActorHandles) {
				auto actor = handle.get().get();
				if (actor) {
					actors.push_back(actor);
				}
			}
		}

		return actors;
	}

	void SetPositionEx(RE::StaticFunctionTag*, RE::Actor* a_actor, float a_x, float a_y, float a_z)
	{
		if (!a_actor)
			return;

		RE::NiPoint3 pos{ a_x, a_y, a_z };
		a_actor->SetPosition(pos, false);
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

	void ToggleCombat(RE::StaticFunctionTag*, bool enable){
		auto processor = RE::ProcessLists::GetSingleton();

		processor->runDetection = enable;
		processor->ClearCachedFactionFightReactions();
	}

	bool DetectionActive(RE::StaticFunctionTag*){
		return RE::ProcessLists::GetSingleton()->runDetection;
	}

	std::vector<RE::Actor*> SortActorsByDistance(RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, std::vector<RE::Actor*> a_list){


		int i,n=a_list.size(),j;
		RE::Actor* temp;


		for(i=0;i<n;i++)
		{
    		for(j=0;j<n-i-1;j++)
    		{
    	    	//if( a[j]>a[j+1] )
    	    	if ((a_ref->data.location.GetDistance(a_list[j]->data.location)) > (a_ref->data.location.GetDistance(a_list[j+1]->data.location)) )
    	   		{
    	    	    temp=a_list[j+1];//swaping element 
    	    	    a_list[j+1]=a_list[j];
    	    	    a_list[j]=temp;
    		    }
		    }
		}
		


		return a_list;
	}

	std::vector<RE::Actor*> RemoveActorsWithGender(RE::StaticFunctionTag*, std::vector<RE::Actor*> a_list, int gender){
		std::vector<RE::Actor*> ret;

		for (auto act : a_list)
			if (act->GetActorBase()->GetSex() != gender )
				ret.push_back(act);

		return ret;
	}

	bool Bind(VM* a_vm)
	{
		const auto obj = "OSANative"sv;

		BIND(RemoveActorsWithGender, true);
		BIND(SortActorsByDistance, true);
		BIND(GetActorFromBase, true);
		BIND(GetLeveledActorBase, true);
		BIND(GetSex, true);
		BIND(GetRace, true);
		BIND(GetVoiceType, true);
		BIND(GetActors);
		BIND(SetPositionEx);
		BIND(LookupRelationshipPartners, true);
		BIND(ToggleCombat);
		BIND(DetectionActive);


		return true;
	}
}
