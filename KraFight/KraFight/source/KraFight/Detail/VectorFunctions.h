#pragma once
#include <vector>

namespace kra {
	template<typename T>
	void ExtendToFit(std::vector<T>& Vec, size_t DesiredIndex)
	{
		if (Vec.size() <= DesiredIndex)
		{
			Vec.resize(DesiredIndex + 1);
		}
	}

	template<typename T>
	bool Fits(const std::vector<T>& Vec, size_t DesiredIndex)
	{
		return Vec.size() > DesiredIndex;
	}
}