#include "KraFight/Network/NetLoadBuffer.h"

kra::NetLoadBuffer::NetLoadBuffer(NetBuffer & Buff) 
	: Buffer(Buff), LoadIndex(0)
{
}
