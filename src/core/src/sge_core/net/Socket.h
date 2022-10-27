#pragma once

#include "SockAddr.h"

namespace sge {

// OOP - BSD socket
class Socket : public NonCopyable {
public:
	using SockLen = SockAddr::SockLen;

#ifdef SGE_OS_WINDOWS
	using socklen_t = int;
	using NativeHandle = SOCKET;

	static const NativeHandle kInvalidHandle = INVALID_SOCKET;
#else
	using NativeHandle = int;
	static const NativeHandle kInvalidHandle = -1;
#endif
	Socket() = default;
	Socket(Socket && r) noexcept { operator=(std::move(r)); }
	void operator=(Socket && r) noexcept {
		close();
		_sock = r._sock;
		r._sock = kInvalidHandle;
	}

	~Socket() { close(); }

	bool isValid() const { return _sock != kInvalidHandle; }

	void createUDP();
	void createTCP();
	void close();

	void setReuseAddr(bool b);
	void bind(const SockAddr& addr);
	void bind(StrView hostname, uint16_t port);

	void listen(int backlog = 64);

	bool connect(const SockAddr& addr);
	bool connect(StrView hostname, uint16_t port);

	bool accept(Socket & acceptedSocket);

	int sendto(const SockAddr& addr, ByteSpan data);
	int sendto(const SockAddr& addr, StrView  data)		{ return sendto(addr, ByteSpan(reinterpret_cast<const u8*>(data.data()), data.size())); }

	int send(const ByteSpan data);
	int send(const StrView  data)						{ return send(ByteSpan(reinterpret_cast<const u8*>(data.data()), data.size())); }

	//template<size_t N>
	//int send(const char (&sz)[N])		{ return N ? send(sz, N-1) : 0; }

	size_t availableBytesToRead();

	int recv(u8* buf, size_t bytesToRecv, int flags);
	int recvfrom(const SockAddr& addr, u8* buf, size_t bytesToRecv);

	template<class CONTAINER>
	int recv(CONTAINER& container, size_t bytesToRecv, int flags);

	template<class CONTAINER>
	int appendRecv(CONTAINER& container, size_t bytesToRecv, int flags);

	void setNonBlocking(bool b);

	SOCKET sock() { return _sock; }

	static	void platformInit();

	int getSockError();

	NativeHandle nativeHandle() { return _sock; }

private:
	NativeHandle _sock = kInvalidHandle;
	static const size_t kIntMax = static_cast<size_t>(std::numeric_limits<int>::max());

	void _setsockopt(int level, int optname, const void* optval, SockLen optlen);

	class PlatformInit {
	public:
		PlatformInit();
		~PlatformInit();
	};

};

template<class CONTAINER> SGE_INLINE
int Socket::recv(CONTAINER& container, size_t bytesToRecv, int flags) {
	container.clear();
	return appendRecv<CONTAINER>(container, bytesToRecv, flags);
}

template<class CONTAINER> inline
int Socket::appendRecv(CONTAINER & buf, size_t bytesToRecv, int flags) {
	auto oldSize = buf.size();
	auto newSize = oldSize + bytesToRecv;
	if (newSize < oldSize)
		throw SGE_ERROR("appendRecv newSize < oldSize");

	buf.resize(newSize);
	int ret = 0;
	
	try {
		ret = recv(reinterpret_cast<u8*>(&buf[oldSize]), bytesToRecv, flags);
		if (ret < 0) {
			buf.resize(oldSize);
			return ret;
		}
	} catch(...) {
		buf.resize(oldSize);
		throw;
	}

	buf.resize(oldSize + ret);
	return ret;
}


} // namespace