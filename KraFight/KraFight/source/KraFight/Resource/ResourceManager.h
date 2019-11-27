#pragma once
#include <memory>
#include "ResourceStorage.h"
#include "KraFight/Behavior/Attack.h"

namespace kra {
	class IResource;

	// Class that stores resources for each used type
	// Adding a new resource should be as easy as adding a ResourceStorage<T> object to this class and serializing it
	class ResourceManager {
	public:
		ResourceStorage<Attack> Attacks;
	};
}