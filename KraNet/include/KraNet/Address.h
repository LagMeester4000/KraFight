#pragma once
#include "Types.h"
#include <SFML/Network/IpAddress.hpp>

namespace kra {
	namespace net {
		struct Address {
			sf::IpAddress Ip;
			u16 port;
		};
	}
}