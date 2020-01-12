#pragma once

namespace kra {
	// Struct used in AnyPtr to generate an index for each type at runtime
	struct AnyTypeIndex {
		static size_t TypeIndexCounter;

		template<typename T>
		static size_t GetTypeIndex()
		{
			const static size_t Ret = TypeIndexCounter++;
			return Ret;
		}
	};

	// Wrapper around a void pointer to make it more type safe
	// AnyPtr has no ownership over the pointer
	// Usage:
	// if (auto* World = Ptr.As<GameWorld>()) {}
	// the `As` function will return nullptr if the type doesn't line up
	class AnyPtr {
	public:
		AnyPtr();

		// Assignment operator will set the pointer and type
		template<typename T>
		AnyPtr& operator=(T* Ptr)
		{
			TypeVal = AnyTypeIndex::GetTypeIndex<T>();
			Ptr = (void*)Ptr;
			return *this;
		}

		// Return the pointer as given type T
		// If the AnyPtr is not of type T, it will return nullptr
		template<typename T>
		T* As() const
		{
			if (AnyTypeIndex::GetTypeIndex<T>() != TypeVal)
				return nullptr;
			return (T*)Ptr;
		}

		// Get the raw pointer value
		// Not to be casted
		const void* Raw() const;

		// Get the index of a type, good for use in if else chains
		template<typename T>
		static size_t Type()
		{
			return AnyTypeIndex::GetTypeIndex<T>();
		}

	private:
		void* Ptr;
		size_t TypeVal;
	};
}