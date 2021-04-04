#pragma once

#define BIND(a_method, ...) a_vm.RegisterFunction(#a_method##sv, obj, a_method __VA_OPT__(, ) __VA_ARGS__)

#include "Util.h"
#include "Papyrus/PapyrusCamera.h"
#include "Papyrus/PapyrusDatabase.h"
#include "Papyrus/PapyrusFace.h"
#include "Papyrus/PapyrusObject.h"
#include "Papyrus/PapyrusUtil.h"

#undef BIND

namespace Papyrus
{
	using VM = RE::BSScript::IVirtualMachine;

	bool Bind(VM* a_vm)
	{
		if (!a_vm) {
			return false;
		}

		Camera::Bind(*a_vm);
		Database::Bind(*a_vm);
		Face::Bind(*a_vm);
		Object::Bind(*a_vm);

		return true;
	}
}
