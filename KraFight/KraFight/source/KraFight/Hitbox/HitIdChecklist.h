#pragma once
#include "HitId.h"
#include <array>

namespace kra {
	class HitIdChecklist {
		static const size_t ChecklistSize = 8;
	public:
		HitIdChecklist();

		// Push a new HitId onto the Checklist
		void Push(HitId NewHit);

		// Check if the Id is in the Checklist
		bool Check(HitId Hit);

	private:
		std::array<HitId, ChecklistSize> Checklist;
	};
}