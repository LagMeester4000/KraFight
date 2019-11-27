#pragma once
#include <map>
#include <vector>
#include <string>
#include "KraFight/Detail/Handle.h"

namespace kra {
	template<typename T>
	class ResourceStorage {
	public:
		ResourceStorage()
		{

		}

		// Get a pointer to an existing resource
		// If the resource does not exist, returns nullpt
		T* GetResource(Handle<T> Handl)
		{
			if (!Handl.IsValid() || Resources.size() <= Handl.GetHandle())
				return nullptr;

			return Resources[Handl.GetHandle()];
		}

		// Create a resource
		// If a resource with Name already exists, return the current handle to it
		Handle<T> MakeResource(std::string Name)
		{
			auto Found = ResourceHandles.find(Name);
			if (Found == ResourceHandles.end())
			{
				Resources.emplace_back();
				return Handle<T>((HandleT)(Resources.size() - 1));
			}
			return Found.second;
		}

	private:
		std::vector<T> Resources;
		std::map<std::string, Handle<T>> ResourceHandles;
	};
}