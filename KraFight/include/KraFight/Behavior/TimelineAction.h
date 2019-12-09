#pragma once
#include "KraFight/Detail/Handle.h"

namespace kra {
	class Entity;
	struct Context;

	// Timeline Action class
	// This class (or children of it) are stored in a timeline
	// and executed when their point in the timeline has been
	// reached. 
	// An instance of this class (or children) should usable on 
	// multiple instances of an entity. So it should have no 
	// variables bound specifically to one entity.
	class TimelineAction {
	public:
		virtual ~TimelineAction() {}

		// Function called when the timeline action is activated
		virtual void Activate(const Context& Con, Handle<Entity> Ent) = 0;
	};
}