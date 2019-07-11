#pragma once
#include "NetBuffer.h"
#include "KraFight/Detail/Pointer.h"

namespace kra {
	// Wrapper for NetBuffer class used to exclusively load things from the buffer
	class NetLoadBuffer {
	public:
		NetLoadBuffer(NetBuffer& Buff);

		// Load a value out of the buffer
		template<typename T>
		NetLoadBuffer& operator>>(T& Val)
		{
			//TODO: actual loading
			return *this;
		}

		// Load a pointer out of the buffer
		template<typename T>
		NetLoadBuffer& operator>>(Pointer<T>& Val)
		{
			//TODO: actual loading
			return *this;
		}

	private:
		NetBuffer& Buffer;
	};
}