#include "KraFight/Network/NetSaveBuffer.h"

kra::NetSaveBuffer::NetSaveBuffer(NetBuffer & Buff)
	: Buffer(Buff), LoadIndex(0)
{
}
