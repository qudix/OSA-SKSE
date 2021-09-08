#pragma once

namespace Serialization
{
	enum : uint32_t
	{
		kSerializationVersion = 0,
		kOSA = 'OSA_'
	};

	void Save(SKSE::SerializationInterface* a_intfc);
	void Load(SKSE::SerializationInterface* a_intfc);
	void Revert(SKSE::SerializationInterface* a_intfc);
}
