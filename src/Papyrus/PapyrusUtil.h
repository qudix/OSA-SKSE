#pragma once

namespace Papyrus::Util
{
    using VM = RE::BSScript::IVirtualMachine;

	void Bind([[maybe_unused]] VM& a_vm)
	{
		const auto obj = "OSANative"sv;
	}
}