#pragma once

namespace Patch
{
	constexpr std::uint8_t JMP{ 0xEB };

	void SetVehicleFix()
	{
		// Disable player scaling being set to 1.0
		REL::Relocation<std::uintptr_t> target1{ REL::ID(36879), 0x60 };
		REL::safe_write(target1.address(), JMP);
	}

	void Install()
	{
		SetVehicleFix();
	}
}