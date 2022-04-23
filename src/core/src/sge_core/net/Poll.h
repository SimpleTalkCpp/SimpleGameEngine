#pragma once

#include "../base/sge_base.h"

namespace sge {

enum class PollFlags : i16 {
	None	= 0,
	In		= POLLIN,
	Out		= POLLOUT,
	InOut	= POLLIN | POLLOUT,
	Error	= POLLERR,
	PRI		= POLLPRI,
	Hup		= POLLHUP,
	Invalid = POLLNVAL,
};

SGE_ENUM_BITWISE_OPERATOR(PollFlags)

#if SGE_OS_WINDOWS
	using PollFd = ::WSAPOLLFD;
	inline int Poll(Span<PollFd> fds, int timeoutMilliseconds) {
		int ret = ::WSAPoll(fds.data(), static_cast<ULONG>(fds.size()), timeoutMilliseconds);
		return ret;
	}
#else
	using PollFd = ::pollfd;
	inline int Poll(Span<PollFd> fds, int timeoutMilliseconds) {
		int ret = ::poll(fds.data(), fds.size(), timeoutMilliseconds);
		return ret;
	}
#endif


} // namespace
