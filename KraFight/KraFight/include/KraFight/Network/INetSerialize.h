#pragma once
#include "KraFight/Detail/Function.h"
#include "KraFight/Detail/Pointer.h"

namespace kra {
	class NetSaveBuffer;
	class NetLoadBuffer;

	// Pure virtual class for virtual objects used for gameplay
	class INetSerialize {
	public:
		// Virtual destructor to prevent memory leaks
		virtual ~INetSerialize() {}

		// Function that returns a function pointer that is able to create an object of the derived type
		// Typically this could be done with a simple (static) lambda function
		virtual Function<Pointer<INetSerialize>(void)> GetCreateFunc() = 0;

		// Function to save the object into the buffer
		virtual void NetSave(NetSaveBuffer& Buff) = 0;

		// Function to load the object from the buffer
		virtual void NetLoad(NetLoadBuffer& Buff) = 0;
	};
}

#define INET_FUNCTION(Type) return kra::Function<kra::Pointer<kra::INetSerialize>(void)>{ []() { return kra::PointerCast<kra::INetSerialize>(kra::MakePointer<Type>()); } };