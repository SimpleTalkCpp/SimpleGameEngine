#include "SockAddr.h"
#include "Socket.h"

namespace sge {

IPv4::IPv4(u8 v0, u8 v1, u8 v2, u8 v3) {
	_data[0] = v0; _data[1] = v1; _data[2] = v2; _data[3] = v3;
}

void IPv4::resolve(StrView hostname) {
	if (!tryResolve(hostname))
		throw SGE_ERROR("resolve");
}

bool IPv4::tryResolve(StrView hostname) {
	SockAddr addr;
	if (!addr.tryResolve(hostname, 0, SockAddrFamily::IPv4))
		return false;
	*this = addr.ipv4();
	return true;
}

IPv4 SockAddr::ipv4() const {
	if (family() != Family::IPv4) {
		return IPv4();
	}	
	static_assert(sizeof(IPv4) == sizeof(_addr_in.sin_addr));
	return *reinterpret_cast<const IPv4*>(&_addr_in.sin_addr);
}

u16 SockAddr::port() const {
	switch (family()) {
		case Family::IPv4: return ntohs(_addr_in.sin_port);
		case Family::IPv6: return ntohs(_addr_in6.sin6_port);
		default: throw SGE_ERROR("");
	}
}

SockAddr::Family SockAddr::family() const {
	switch (_addr.sa_family) {
		case AF_INET:	return Family::IPv4;
		case AF_INET6:	return Family::IPv6;
		default:		return Family::Unknown;
	}
}

void SockAddr::setFamily(Family f) {
	_addr.sa_family = static_cast<u16>(f);
}

void SockAddr::resolve(StrView hostname, u16 port, Family family) {
	if (!tryResolve(hostname, port, family))
		throw SGE_ERROR("resolve hostname");
}

bool SockAddr::tryResolve(StrView hostname, u16 port, Family family) {
	Socket::platformInit();
	reset();

	TempString hostnameStr(hostname);

	struct addrinfo hints = {};
	hints.ai_family = enumInt(family);

	struct addrinfo* result = nullptr;

	if (getaddrinfo(hostnameStr.c_str(), nullptr, &hints, &result) != 0)
		return false;

	return _setByAddrInfo(*result, port);
}

bool SockAddr::_setByAddrInfo(const addrinfo& v, u16 port) {
	setFamily(Family::Unknown);	
	switch (v.ai_family) {
		case AF_INET: {
			_addr_in = *reinterpret_cast<const sockaddr_in*>(v.ai_addr);
			_addr_in.sin_port = htons(port);
			setFamily(Family::IPv4);
		}break;

		case AF_INET6: {
			_addr_in6 = *reinterpret_cast<const sockaddr_in6*>(v.ai_addr);
			_addr_in6.sin6_port = htons(port);
			setFamily(Family::IPv6);
		}break;

		default:
			return false;
	}
	return true;
}

} // namespace