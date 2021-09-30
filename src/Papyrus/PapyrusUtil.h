#pragma once

namespace PapyrusUtil
{
    using VM = RE::BSScript::IVirtualMachine;
	
	RE::TESForm* NewObject(
		RE::BSScript::IVirtualMachine* a_vm,
		RE::VMStackID a_stackID,
		RE::StaticFunctionTag*,
		std::string a_class)
	{
		if (a_class.empty()) {
			a_vm->TraceStack("Class is empty", a_stackID);
			return nullptr;
		}

		logger::info("{} a", a_class);

		auto vm = RE::BSScript::Internal::VirtualMachine::GetSingleton();
		auto formType = RE::FormType::Quest;
		auto factory = RE::IFormFactory::GetFormFactoryByType(formType);
		auto form = factory->Create();

		RE::BSTSmartPointer<RE::BSScript::Object> script;
		auto handle = vm->GetObjectHandlePolicy1()->GetHandleForObject(formType, form);
		vm->CreateObject2(a_class, script);
		vm->BindObject(script, handle, false);
		
		return form;
	}

	void DeleteObject(
		RE::BSScript::IVirtualMachine* a_vm,
		RE::VMStackID a_stackID,
		RE::StaticFunctionTag*,
		RE::TESForm* a_form)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		//form->SetDelete(true);
		auto vm = RE::BSScript::Internal::VirtualMachine::GetSingleton();
		auto formType = RE::FormType::Quest;

		auto handle = vm->GetObjectHandlePolicy1()->GetHandleForObject(formType, a_form);
		vm->ResetAllBoundObjects(handle);
		vm->GetObjectBindPolicy()->bindInterface->RemoveAllBoundObjects(handle);

		// dangerous??
		// nuclear!!
		delete (a_form);
	}

	int RandomInt(RE::StaticFunctionTag*, int a_min, int a_max)
	{
		std::random_device rd;
		std::uniform_int_distribution<int> dist(a_min, a_max);
   		std::mt19937 mt(rd());

   		return dist(mt);
	}

	float RandomFloat(RE::StaticFunctionTag*, float a_min, float a_max)
	{
		std::random_device rd;
		std::uniform_real_distribution<float> dist(a_min, a_max);
   		std::mt19937 mt(rd());

   		return dist(mt);
	}

	void SendEvent(
		RE::BSScript::IVirtualMachine* a_vm,
		RE::VMStackID a_stackID,
		RE::StaticFunctionTag*,
		RE::TESForm* a_form,
		std::string a_event)
	{
		if (!a_form) {
			a_vm->TraceStack("Form is None", a_stackID);
			return;
		}

		auto args = RE::MakeFunctionArguments();
		auto handle = Script::GetHandle(a_form);
		a_vm->SendEvent(handle, a_event, args);
		delete args;
	}

	bool TryLock(RE::StaticFunctionTag*, std::string a_lock)
	{
		auto locker = Locker::GetSingleton();
		return locker->TryLock(a_lock);
	}

	void Unlock(RE::StaticFunctionTag*, std::string a_lock)
	{
		auto locker = Locker::GetSingleton();
		locker->Unlock(a_lock);
	}

	std::string Translate(RE::StaticFunctionTag*, std::string a_key)
	{
		auto locale = LocaleManager::GetSingleton();
		return locale->Translate(a_key);
	}

	void SetLocale(RE::StaticFunctionTag*, std::string a_locale)
	{
		auto locale = LocaleManager::GetSingleton();
		locale->SetOverride(a_locale);
	}

	bool Bind(VM* a_vm)
	{
		const auto obj = "OSANative"sv;

		BIND(RandomInt, true);
		BIND(RandomFloat, true);
		BIND(SendEvent);
		BIND(NewObject);
		BIND(DeleteObject);

		BIND(TryLock);
		BIND(Unlock);

		BIND(Translate);
		BIND(SetLocale);

		return true;
	}
}
