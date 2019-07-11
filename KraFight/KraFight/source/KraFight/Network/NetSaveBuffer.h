#pragma once
#include "NetBuffer.h"
#include "KraFight/Detail/Pointer.h"

namespace kra {
	// Wrapper for NetBuffer class used to exclusively load things from the buffer
	class NetSaveBuffer {
	public:
		NetSaveBuffer(NetBuffer& Buff);

		// Save a value into the buffer
		template<typename T>
		NetSaveBuffer& operator<<(T& Val)
		{
			//TODO: actual saving
			return *this;
		}

		// Save a pointer into the buffer
		template<typename T>
		NetSaveBuffer& operator<<(Pointer<T>& Val)
		{
			//TODO: actual saving
			return *this;
		}

	private:
		NetBuffer& Buffer;
	};
}