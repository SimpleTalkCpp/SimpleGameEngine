#pragma once

#include "../base/Error.h"
#include "../log/Log.h"
#include "../string/StringUtil.h"
#include "../string/Fmt.h"

namespace sge {

enum class SockAddrFamily {
	Unknown = AF_UNSPEC,
	IPv4    = AF_INET,
	IPv6    = AF_INET6,
};

class IPv4 {
public:
	using Addr = decltype(sockaddr_in::sin_addr);
	static constexpr size_t kAddrBufSize = 4;

	IPv4() { _data32[0] = 0; }
	IPv4(u8 v0, u8 v1, u8 v2, u8 v3);

	void	resolve(StrView hostname);
	bool	tryResolve(StrView hostname);

	bool operator==(const IPv4& r) const { return _data32[0] == r._data32[0]; }

	auto onFormat(fmt::format_context& ctx) const {
		return fmt::format_to(ctx.out(), "{}.{}.{}.{}", _data[0], _data[1], _data[2], _data[3]);
	}

	union {
		Addr	_addr;
		u8		_data[kAddrBufSize];
		u32		_data32[1];
	};
};

class SockAddr {
public:
	using Family = SockAddrFamily;

	void reset() { _addr.sa_family = 0; }

#if SGE_OS_WINDOWS
	using SockLen = int;
#else
	using SockLen = socklen_t;
#endif

	IPv4		ipv4	() const;
	u16			port	() const;

	Family		family	() const;
	void		setFamily(Family f);

	void resolve	(StrView hostname, u16 port, Family family = Family::IPv4);
	bool tryResolve	(StrView hostname, u16 port, Family family = Family::IPv4);

	auto onFormat(fmt::format_context& ctx) const {
		return fmt::format_to(ctx.out(), "{}:{}", ipv4(), port());
	}

	static const size_t kAddrBufSize = 32;
friend class Socket;
protected:
	bool _setByAddrInfo(const addrinfo& v, u16 port);

	union {
		sockaddr		_addr;
		sockaddr_in		_addr_in;
		sockaddr_in6	_addr_in6; // sizeof(sockaddr_in6) > sizeof(sockaddr), so we can't use sockaddr to store
		u8				_data[kAddrBufSize];
	};
};

SGE_FORMATTER(IPv4)
SGE_FORMATTER(SockAddr)

} // namespace
