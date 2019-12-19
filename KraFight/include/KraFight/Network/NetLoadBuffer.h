#pragma once
#include "NetBuffer.h"
#include "KraFight/Detail/Pointer.h"
#include "INetSerialize.h"
#include "HasFunc.h"
#include <type_traits>

namespace kra {
	// Wrapper for NetBuffer class used to exclusively load things from the buffer
	class NetLoadBuffer {
	public:
		NetLoadBuffer(NetBuffer& Buff);

		template<typename T>
		void CopyRaw(T& Val)
		{
			Buffer.CopyFrom(LoadIndex, Val);
			LoadIndex += sizeof(T);
		}

		// Load a value out of the buffer
		template<typename T>
		NetLoadBuffer& operator>>(T& Val);

		// Load a pointer out of the buffer
		template<typename T>
		NetLoadBuffer& operator>>(Pointer<T>& Val)
		{
			static_assert(std::is_base_of_v<INetSerialize, T>, "T is not INetSerialize type, and thus cannot be serialized");

			// Get and save the creation function pointer
			Function<Pointer<INetSerialize>(void)> CPtr;
			Buffer.CopyFrom(LoadIndex, CPtr);
			LoadIndex += sizeof(CPtr);

			// Create a new object and load data into it
			auto NewVal = CPtr.func();
			NewVal->NetLoad(*this);
			Val = PointerCast<T>(NewVal);

			return *this;
		}

	private:
		NetBuffer& Buffer;
		size_t LoadIndex;
	};

	// Compile time if statement
	template<typename T, bool B, bool B2>
	struct RunNetLoad;

	template<typename T>
	struct RunNetLoad<T, false, false> {
		static void Func(NetLoadBuffer& Buff, T& Val)
		{
			Buff.CopyRaw(Val);
		}
	};

	template<typename T>
	struct RunNetLoad<T, true, false> {
		static void Func(NetLoadBuffer& Buff, T& Val)
		{
			Val.NetLoad(Buff);
		}
	};

	template<typename T>
	struct RunNetLoad<T, false, true> {
		static void Func(NetLoadBuffer& Buff, T& Val)
		{
			NetLoad(Buff, Val);
		}
	};

	template<typename T>
	inline NetLoadBuffer & NetLoadBuffer::operator>>(T & Val)
	{
		RunNetLoad<T, ::HasNetLoad<T>, ::HasRawNetLoad<T>>::Func(*this, Val);
		return *this;	
	}
}