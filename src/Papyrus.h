#pragma once

namespace Papyrus
{
	using VM = RE::BSScript::IVirtualMachine;

	void BuildDB(RE::StaticFunctionTag*)
	{
		DB::Build();
	}

	static constexpr char CLASS_NAME[] = "OSANative";

	bool Register(VM* a_vm)
	{
		a_vm->RegisterFunction("BuildDB", CLASS_NAME, BuildDB);
		return true;
	}
}