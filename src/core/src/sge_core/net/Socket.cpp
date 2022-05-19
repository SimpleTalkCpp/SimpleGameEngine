#include "Socket.h"

namespace sge {

Socket::PlatformInit::PlatformInit() {
#if SGE_OS_WINDOWS
	WSADATA	wsad;
	if (WSAStartup(WINSOCK_VERSION, &wsad) != 0) {
		throw SGE_ERROR("WSAStartup");
	}
#endif
}

Socket::PlatformInit::~PlatformInit() {
#if SGE_OS_WINDOWS
	if (WSACleanup() != 0) {
		SGE_ASSERT(false);
	}
#endif
}

void Socket::platformInit() {
	static PlatformInit s;
}

int Socket::getSockError() {
#if SGE_OS_WINDOWS
	int ret = WSAGetLastError();
#else
	int ret = errno;
#endif
	return ret;
}

void Socket::createUDP() {
	close();
	platformInit();

	_sock = ::socket(AF_INET, SOCK_DGRAM, 0);
	if (_sock == kInvalidHandle) {
		throw SGE_ERROR("createUDP");
	}
}

void Socket::createTCP() {
	close();
	platformInit();

	_sock = ::socket(AF_INET, SOCK_STREAM, 0);
	if (_sock == kInvalidHandle) {
		throw SGE_ERROR("createTCP");
	}
}

void Socket::setReuseAddr(bool b) {
	int a = b ? 1 : 0;
#if !SGE_OS_WINDOWS
	_setsockopt(SOL_SOCKET, SO_REUSEPORT, &a, sizeof(a));
#endif
	_setsockopt(SOL_SOCKET, SO_REUSEADDR, &a, sizeof(a));
}

void Socket::_setsockopt(int level, int optname, const void* optval, SockLen optlen) {
	if (!isValid()) return;

	int ret = ::setsockopt(_sock, level, optname, (const char*)optval, optlen );
	if (ret != 0)
		throw SGE_ERROR("setsockopt");
}

void Socket::bind(StrView hostname, uint16_t port) {
	SockAddr addr;
	addr.resolve(hostname, port);
	return bind(addr);
}

void Socket::bind(const SockAddr& addr) {
	if (!isValid()) return;

	int ret = ::bind(_sock, &addr._addr, sizeof(addr._addr));
	if (ret != 0) {
		throw SGE_ERROR("bind");
	}
}

void Socket::listen(int backlog) {
	if (!isValid()) return;

	int ret = ::listen(_sock, backlog);
	if (ret != 0) {
		throw SGE_ERROR("listen");
	}
}

bool Socket::connect(const SockAddr& addr) {
	if (!isValid()) return false;

	int ret = ::connect(_sock, &addr._addr, sizeof(addr._addr));
	if (ret < 0) {
#ifdef SGE_OS_WINDOWS
		int e = WSAGetLastError();
		if (e == WSAEWOULDBLOCK) // connect in non-blocking mode
			return false;
#else
		int e = errno;
		if (e == EINPROGRESS) // connect in non-blocking mode
			return false;
#endif
		throw SGE_ERROR("connect");
	}
	return true;
}

bool Socket::connect(StrView hostname, uint16_t port) {
	SockAddr addr;
	addr.resolve(hostname, port);
	return connect(addr);
}

bool Socket::accept(Socket & acceptedSocket) {
	acceptedSocket.close();

	auto c = ::accept(_sock, nullptr, nullptr);
	if (c == kInvalidHandle)
		return false;

	acceptedSocket._sock = c;
	return true;
}

int Socket::sendto(const SockAddr& addr, ByteSpan data) {
	if (data.size() > kIntMax)
		throw SGE_ERROR("send dataSize is too big");

	int ret =::sendto(_sock, reinterpret_cast<const char*>(data.data()), static_cast<int>(data.size()), 0, &addr._addr, sizeof(addr._addr));
	return ret;
}

int Socket::send(ByteSpan data) {
	if (!isValid()) return 0;
	if (data.size() > kIntMax)
		throw SGE_ERROR("send dataSize is too big");

	int ret =::send(_sock, reinterpret_cast<const char*>(data.data()), static_cast<int>(data.size()), 0);
	return ret;
}

size_t Socket::availableBytesToRead() {
	if (!isValid()) return 0;

#ifdef _WIN32
	u_long n = 0;
	if (0 != ::ioctlsocket(_sock, FIONREAD, &n))
		throw SGE_ERROR("availableBytesToRead");
	return static_cast<size_t>(n);
#else
	int n = 0;
	if (0 != ::ioctl(_sock, FIONREAD, &n))
		throw SGE_ERROR("availableBytesToRead");
	return static_cast<size_t>(n);
#endif
}

int Socket::recv(u8* buf, size_t bytesToRecv, int flags) {
	if (!isValid()) return 0;

	if (bytesToRecv > kIntMax)
		bytesToRecv = kIntMax;

	int ret = ::recv(_sock, reinterpret_cast<char*>(buf), static_cast<int>(bytesToRecv), flags);
	return ret;
}

int Socket::recvfrom(const SockAddr& addr, u8* buf, size_t bytesToRecv) {
	if (!isValid()) return 0;

	if (bytesToRecv > kIntMax)
		bytesToRecv = kIntMax;

	socklen_t addrLen = sizeof(addr._addr);
	int ret = ::recvfrom(_sock, reinterpret_cast<char*>(buf), static_cast<int>(bytesToRecv), 0, constCast(&addr._addr), &addrLen);
	return ret;
}

void Socket::setNonBlocking(bool b)
{
#ifdef _WIN32
	u_long v = b ? 1 : 0;
	if (0 != ::ioctlsocket(_sock, FIONBIO, &v))
		throw SGE_ERROR("setNonBlocking");
#else
	long v = b ? 1 : 0;
	if (0 != ::ioctl(_sock, FIONBIO, &v))
		throw SGE_ERROR("setNonBlocking");
#endif
}

void Socket::close() {
	if (_sock != kInvalidHandle) {
		#ifdef _WIN32
			::closesocket(_sock);
		#else
			::close(_sock);
		#endif
		_sock = kInvalidHandle;
	}
}

} // namespace