#pragma once
#include <array>
#include <utility>
#include <assert.h>

namespace kra {
	namespace net {
		template<typename T, size_t S>
		class RingBufferStatic {
		public:
			RingBufferStatic()
			{
			}

			// Push an element on top of the buffer
			// Pushes buffer one index forward
			void PushBack(T&& Val)
			{
				size_t TopI = Top();

				if (SizeIndex < Buffer.size())
				{
					SizeIndex++;
				}
				else
				{
					BottomIndex++;
				}

				operator[](TopI) = std::move(Val);
			}

			// Push an element on top of the buffer
			// Pushes buffer one index forward
			void PushBack(const T& Val)
			{
				size_t TopI = Top();

				if (SizeIndex < Buffer.size())
				{
					SizeIndex++;
				}
				else
				{
					BottomIndex++;
				}

				operator[](TopI) = Val;
			}

			// Insert element into ring buffer
			// Returns element that needs to be invalidated (if needed)
			T* Insert(T& Val, size_t Index)
			{
				T* Ret = nullptr;
				size_t TopI = Top();

				// Push bottom boundry if needed
				if (Index > (TopI - 1))
				{
					BottomIndex = Index - (TopI - 1);

					// Invalidate new element
					Ret = &operator[](Top());
				}

				// Insert element
				operator[](Index) = Val;

				return Ret;
			}

			// Insert element into ring buffer
			// Returns element that needs to be invalidated (if needed)
			T* Insert(T&& Val, size_t Index)
			{
				T* Ret = nullptr;
				size_t TopI = Top();

				// Push bottom boundry if needed
				if (Index > (TopI - 1))
				{
					BottomIndex = Index - (TopI - 1);

					// Invalidate new element
					Ret = &operator[](Top());
				}

				// Insert element
				operator[](Index) = std::move(Val);

				return Ret;
			}

			// Returns the lowes usable index
			size_t Bottom() const
			{
				return BottomIndex;
			}

			// Returns Bottom() + BottomIndex
			size_t Top() const
			{
				return BottomIndex + Size();
			}

			// Get the size of the buffer
			size_t Size() const
			{
				return SizeIndex;
			}

			T& operator[](size_t I)
			{
				assert(I >= BottomIndex);
				assert(I < Top());
				return Buffer[I % Size()];
			}

			const T& operator[](size_t I) const
			{
				assert(I >= BottomIndex);
				assert(I < Top());
				return Buffer[I % Size()];
			}

			// Get an element relative to the top usable index
			// Index 0 retrieves the highest usable element
			const T& GetFromTop(size_t I) const
			{
				operator[](Top() - 1 - I);
			}

		private:
			std::array<T, S> Buffer;
			size_t BottomIndex = 0;
			size_t SizeIndex = 0;
		};
	}
}