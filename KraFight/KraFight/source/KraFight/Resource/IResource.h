#pragma once
#include <string>

namespace kra {
	class IResource {
	public:
		virtual ~IResource() {}

		virtual void CreateFromFile(std::string Path) = 0;


	};
}