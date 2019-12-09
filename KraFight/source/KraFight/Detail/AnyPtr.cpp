#include "KraFight/Detail/AnyPtr.h"

using namespace kra;

size_t AnyTypeIndex::TypeIndexCounter = 1;

kra::AnyPtr::AnyPtr()
	: Ptr(nullptr), Type(0)
{
}

const void * kra::AnyPtr::Raw() const
{
	return Ptr;
}
