#include "KraNet/InputBuffer.h"

kra::net::InputBuffer::InputBuffer()
{
	// Each input buffer starts with one valid empty input to make frame count 0
	Element Push;
	Push.Valid = true;
	//Push.Input should have values as 0 by default
	Inputs.PushBack(Push);
}