#pragma once

namespace PapyrusUtil
{
    using VM = RE::BSScript::IVirtualMachine;

	// Sairion, what the heck does this do?
	int32_t GetFormID(RE::StaticFunctionTag*, RE::TESForm* a_form)
	{
		if (!a_form)
			return -1;

		auto d = a_form->formID;
		auto c = a_form->formID;
		auto act = a_form->As<RE::Actor>();
		auto h = act->GetActorBase()->formID;

		return d + c + h;
	}

	bool Bind(VM* a_vm)
	{
		const auto obj = "OSANative"sv;

		BIND(GetFormID);

		return true;
	}
}
