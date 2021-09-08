#include "Serial/Manager.h"

#include "Game/Locker.h"

namespace Serialization
{
	constexpr std::size_t SIZE = sizeof(uint32_t);

	std::string DecodeType(uint32_t a_typeCode)
	{
		std::string sig;
		sig.resize(SIZE);
		char* iter = reinterpret_cast<char*>(&a_typeCode);
		for (std::size_t i = 0, j = SIZE - 2; i < SIZE - 1; ++i, --j) {
			sig[j] = iter[i];
		}

		return sig;
	}

	void Save(SKSE::SerializationInterface* a_intfc)
	{
		//logger::info("Finished saving data"sv);
	}

	void Load(SKSE::SerializationInterface* a_intfc)
	{
		//logger::info("Finished loading data"sv);
	}

	void Revert(SKSE::SerializationInterface* a_intfc)
	{
		auto locker = Locker::GetSingleton();
		locker->Revert(a_intfc);
	}
}
