#pragma once
#include <random>


namespace PapyrusUtil
{
    using VM = RE::BSScript::IVirtualMachine;

	
	RE::TESForm* NewObject(RE::StaticFunctionTag*, std::string classtype)
	{
		logger::info("{} a", classtype);

		auto const formType = RE::FormType::Quest;
		auto factory = RE::IFormFactory::GetFormFactoryByType(formType);
		auto form = factory->Create();


		auto virtualmachine = RE::BSScript::Internal::VirtualMachine::GetSingleton();

		auto handle = virtualmachine->GetObjectHandlePolicy1()->GetHandleForObject(formType, form);
		RE::BSTSmartPointer<RE::BSScript::Object> script;

		virtualmachine->CreateObject2(classtype, script);


		virtualmachine->BindObject(script, handle, false);
		
		return form;
	}

	void DeleteObject(RE::StaticFunctionTag*, RE::TESForm* form)
	{
		//form->SetDelete(true);
		auto const formType = RE::FormType::Quest;
		auto virtualmachine = RE::BSScript::Internal::VirtualMachine::GetSingleton();

		auto handle = virtualmachine->GetObjectHandlePolicy1()->GetHandleForObject(formType, form);

		virtualmachine->ResetAllBoundObjects(handle);
		virtualmachine->GetObjectBindPolicy()->bindInterface->RemoveAllBoundObjects(handle);
		// dangerous??
		delete(form);
	}

	int RandomInt(RE::StaticFunctionTag*, int min, int max)
	{
		std::random_device rd;
    	std::uniform_int_distribution<int> dist(min, max);
   		std::mt19937 mt(rd());

   		return dist(mt);
	}

	float RandomFloat(RE::StaticFunctionTag*, float min, float max)
	{
		std::random_device rd;
    	std::uniform_real_distribution<float> dist(min, max);
   		std::mt19937 mt(rd());

   		return dist(mt);
	}

	bool Bind(VM* a_vm)
	{
		const auto obj = "OSANative"sv;

		BIND(NewObject);
		BIND(DeleteObject);
		BIND(RandomInt, true);
		BIND(RandomFloat, true);



		return true;
	}
}
