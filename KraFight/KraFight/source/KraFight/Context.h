#pragma once
#include "KraFight/Detail/Handle.h"

namespace kra {
	class PhysicsManager;
	class HitboxManager;
	class EntityManager;
	class InputManager;
	class StateMachineManager;

	// Forward declaration to use ContextT
	template<typename T>
	struct ContextT;

	// Struct holding references to every system used for gameplay
	struct Context {
		PhysicsManager* PhysicsObjects;
		HitboxManager* Hitboxes;
		EntityManager* Entities;
		InputManager* Inputs;
		StateMachineManager* StateMachines;

		// Make Func
		template<typename T>
		ContextT<T> Make(Handle<T> Self) const;
	};

	// Context type passed to objects in subsystems
	template<typename T>
	struct ContextT : Context {
		Handle<T> Self;
	};

	template<typename T>
	inline ContextT<T> Context::Make(Handle<T> Self) const
	{
		ContextT<T> Ret;
		Context& Ref = Ret;
		Ref = Context(*this);
		Ret.Self = Self;
		return Ret;
	}

	// Decl
	class Entity;
	using EntContext = ContextT<Entity>;
}