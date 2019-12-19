#pragma once
#include "KraFight/Detail/HasFunctionMacro.h"

namespace kra {
	class NetSaveBuffer;
	class NetLoadBuffer;
	
	void NetSave(NetSaveBuffer&, const char*);
	void NetLoad(NetLoadBuffer&, const char*);
}

HAS_FUNC_NAMED(HasNetSave, NetSave, kra::NetSaveBuffer&);
HAS_FUNC_NAMED(HasNetLoad, NetLoad, kra::NetLoadBuffer&);
IS_RAW_FUNC_NAMED(HasRawNetSave, NetSave, kra::NetSaveBuffer&, T&);
IS_RAW_FUNC_NAMED(HasRawNetLoad, NetLoad, kra::NetLoadBuffer&, T&);