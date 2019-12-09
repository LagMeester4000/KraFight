#pragma once
#include <vector>

namespace kra {
	class NetSaveBuffer;
	class NetLoadBuffer;

	template<typename T>
	void NetSave(NetSaveBuffer& Buff, std::vector<T>& Vec)
	{
		size_t VecSize = Vec.size();
		Buff << VecSize;

		for (size_t I = 0; I < VecSize; ++I)
		{
			Buff << Vec[I];
		}
	}

	template<typename T>
	void NetLoad(NetLoadBuffer& Buff, std::vector<T>& Vec)
	{
		size_t VecSize;
		Buff >> VecSize;

		Vec.clear();
		for (size_t I = 0; I < VecSize; ++I)
		{
			T Push;
			Buff >> Push;
			Vec.push_back(Push);
		}
	}
}