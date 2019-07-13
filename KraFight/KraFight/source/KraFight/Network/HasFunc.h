#pragma once
#include "KraFight/Detail/HasFunctionMacro.h"

namespace kra {
	class NetSaveBuffer;
	class NetLoadBuffer;
	
	void NetSave(const NetSaveBuffer&, const char*);
	void NetLoad(const NetLoadBuffer&, const char*);
}

HAS_FUNC_NAMED(HasNetSave, NetSave, const kra::NetSaveBuffer&);
HAS_FUNC_NAMED(HasNetLoad, NetLoad, const kra::NetLoadBuffer&);
IS_RAW_FUNC_NAMED(HasRawNetSave, kra::NetSave, const kra::NetSaveBuffer&, T&);
IS_RAW_FUNC_NAMED(HasRawNetLoad, kra::NetLoad, const kra::NetLoadBuffer&, T&);