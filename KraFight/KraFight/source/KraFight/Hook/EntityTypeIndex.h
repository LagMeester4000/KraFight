#pragma once

namespace kra {
	// Struct used in HookMaager to generate an index for each type at runtime
	struct EntityTypeIndex {
		static size_t TypeIndexCounter;

		template<typename T>
		static size_t GetTypeIndex()
		{
			const static size_t Ret = TypeIndexCounter++;
			return Ret;
		}
	};
}