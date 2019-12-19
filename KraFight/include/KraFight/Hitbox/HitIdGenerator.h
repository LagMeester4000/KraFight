#pragma once
#include "HitId.h"

namespace kra {
	class NetSaveBuffer;
	class NetLoadBuffer;

	class HitIdGenerator {
	public:
		HitIdGenerator();

		// Generate a new HitId
		// Increases Counter value
		HitId Generate();

	public: // Networking
		void NetSave(NetSaveBuffer& Buff);
		void NetLoad(NetLoadBuffer& Buff);

	private:
		HitIdT Counter = 1;
	};
}