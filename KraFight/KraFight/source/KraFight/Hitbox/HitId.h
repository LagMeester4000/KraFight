#pragma once
#include <stdint.h>

namespace kra {
	using HitIdT = uint32_t;

	class HitId {
		// Each HitId can have 9 other HitId offsets
		static const size_t HitIdSize = 10;

		friend class HitIdGenerator;
	public:
		HitId();

		// Offset this ID to make a new one
		HitId Offset(HitIdT Off) const;

		// Get the raw value of the HitId
		HitIdT Value() const;

		// Operators
		bool operator==(const HitId& Other) const;
		bool operator!=(const HitId& Other) const;

	private:
		HitIdT Id = 0;
	};
}