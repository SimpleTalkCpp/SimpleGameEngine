#include "FileStream.h"
#include "../base/Error.h"

namespace sge {

void FileStream::openRead(StrView filename) {
	open(filename, FileMode::OpenExists, FileAccess::Read, FileShareMode::Read);
}

void FileStream::openAppend(StrView filename) {
	open(filename, FileMode::OpenOrCreate, FileAccess::ReadWrite, FileShareMode::Read);
	setPosFromEnd(0);
}

void FileStream::openWrite(StrView filename, bool truncate) {
	open(filename, FileMode::OpenOrCreate, FileAccess::ReadWrite, FileShareMode::Read);
	if (truncate) {
		setFileSize(0);
	}
}

#if 0
#pragma mark ========= Windows ============
#endif
#if SGE_OS_WINDOWS

void FileStream::open(StrView filename, FileMode mode, FileAccess access, FileShareMode shareMode) {
	close();
	_filename = filename;


	DWORD	create_flags	= 0;
	DWORD	access_flags	= 0;
	DWORD	share_flags		= 0;

	switch (mode) {
		case FileMode::CreateNew: 		create_flags |= CREATE_NEW;		break;
		case FileMode::OpenExists:		create_flags |= OPEN_EXISTING;	break;
		case FileMode::OpenOrCreate:	create_flags |= OPEN_ALWAYS;	break;
	}

	switch (access) {
		case FileAccess::Read:			access_flags |= GENERIC_READ;					break;
		case FileAccess::ReadWrite:		access_flags |= GENERIC_READ | GENERIC_WRITE;	break;
		case FileAccess::WriteOnly:		access_flags |= GENERIC_WRITE;					break;
	}

	switch (shareMode) {
		case FileShareMode::None:		break;
		case FileShareMode::Read:		share_flags |= FILE_SHARE_READ;						break;
		case FileShareMode::ReadWrite:	share_flags |= FILE_SHARE_READ | FILE_SHARE_WRITE;	break;
		case FileShareMode::Write:		share_flags |= FILE_SHARE_WRITE;					break;
	}

	TempStringW filenameW;
	UtfUtil::convert(filenameW, filename);

	_fd = ::CreateFile(filenameW.c_str(), access_flags, share_flags, nullptr, create_flags, FILE_ATTRIBUTE_NORMAL, nullptr );
	if (_fd == kInvalid()) {
		DWORD err = ::GetLastError();
		switch( err ) {
			case ERROR_FILE_NOT_FOUND:		throw SGE_ERROR("file not found");
			case ERROR_PATH_NOT_FOUND:		throw SGE_ERROR("path not found");
			case ERROR_FILE_EXISTS:			throw SGE_ERROR("file doesn't exists");
			case ERROR_ALREADY_EXISTS:		throw SGE_ERROR("file already exists");
			case ERROR_ACCESS_DENIED:		throw SGE_ERROR("access denied");
			case ERROR_SHARING_VIOLATION:	throw SGE_ERROR("sharing violation");
			default:						throw SGE_ERROR("open file error");
		}
	}
}

void FileStream::close() {
	if (!isOpened()) return;
	BOOL ret = ::CloseHandle(_fd);
	if (!ret) throw SGE_ERROR("close file error");
	_fd = kInvalid();
}

void FileStream::readBytes(Span<u8> data) {
	_ensure_fd();
	if (data.size() <= 0) return;
	if (data.size() >= UINT32_MAX)
		throw SGE_ERROR("file read");

	DWORD dwSize = static_cast<DWORD>(data.size());
	DWORD result;
	BOOL ret = ::ReadFile(_fd, data.data(), dwSize, &result, nullptr);
	if (!ret) {
		DWORD e = ::GetLastError();
		switch (e) {
			case ERROR_LOCK_VIOLATION: throw SGE_ERROR("file lock violation");
		}
		throw SGE_ERROR("file read");
	}
}

void FileStream::writeBytes(ByteSpan data) {
	_ensure_fd();
	if (data.size() <= 0) return;
	if (data.size() >= UINT32_MAX)
		throw SGE_ERROR("file read");

	DWORD dwSize = static_cast<DWORD>(data.size());
	DWORD result;
	BOOL ret = ::WriteFile(_fd, data.data(), dwSize, &result, nullptr);
	if (!ret) throw SGE_ERROR("file write");
}

FileSize FileStream::fileSize() {
	_ensure_fd();

	DWORD high = 0;
	DWORD low  = ::GetFileSize(_fd, &high);
	if (low == INVALID_FILE_SIZE)
		throw SGE_ERROR("file size");

	auto fileSize = static_cast<FileSize>(high) << 32 | low;
	return fileSize;
}

void FileStream::setFileSize(FileSize newSize) {
	_ensure_fd();
	FileSize oldPos = getPos();
	setPos(newSize);
	::SetEndOfFile(_fd);

	if (oldPos < newSize)
		setPos(oldPos);
}

FileSize FileStream::getPos() {
	_ensure_fd();
	LONG high = 0;
	LONG low  = ::SetFilePointer(_fd, 0, &high, FILE_CURRENT);
	if (low < 0 || high < 0) throw SGE_ERROR("get file pos");
	auto pos = static_cast<FileSize>(low) | static_cast<FileSize>(high) << 32;
	return pos;
}

void FileStream::setPos(FileSize pos) {
	_ensure_fd();
	LONG high = static_cast<LONG>(pos >> 32);
	LONG low  = static_cast<LONG>(pos);
	::SetFilePointer( _fd, low, &high, FILE_BEGIN);
}

void FileStream::setPosFromEnd(FileSize pos) {
	_ensure_fd();
	LONG high = static_cast<LONG>(pos >> 32);
	LONG low  = static_cast<LONG>(pos);
	::SetFilePointer( _fd, low, &high, FILE_END);
}

#if 0
#pragma mark ========= Non-Windows ============
#endif
#else

void FileStream::open(StrView filename, FileMode mode, FileAccess access, FileShareMode shareMode) {
	close();

	_filename = filename;
	int access_flag = 0;

#if SGE_OS_LINUX
	access_flags |= O_LARGEFILE;
#endif

	mode_t mode_flags = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

	switch (mode) {
		case FileMode::CreateNew: 		access_flags |= O_CREAT | O_EXCL; break;
		case FileMode::OpenExists:		break;
		case FileMode::OpenOrCreate: 	access_flags |= O_CREAT; break;
		default: throw SGE_ERROR("unknown file mode");
	}

	switch (access) {
		case FileAccess::Read:		access_flags |= O_RDONLY;	break;
		case FileAccess::ReadWrite:	access_flags |= O_RDWR;		break;
		case FileAccess::WriteOnly:	access_flags |= O_WRONLY;	break;
		default: throw SGE_ERROR("unknown file access");
	}

	SGE_ASSERT(_fd == kInvalid);
	_fd = ::open(_filename.c_str(), access_flags, mode_flags);
	if (_fd == kInvalid()) {
		switch (errno) {
			case EACCES:	throw SGE_ERROR("cannot access file");
			case EEXIST:	throw SGE_ERROR("file doesn't exists");
			case ENFILE:	throw SGE_ERROR("too many files open in system");
			case EMFILE:	throw SGE_ERROR("too many files open");
			case ENOENT:	throw SGE_ERROR("no such file or directory");
			default:		throw SGE_ERROR("unknown open file error");
		}
	}
}

void FileStream::close() {
	if (!isOpened()) return;
	::close(_fd);
	_fd = kInvalid();
}

void FileStream::flush() {
	if (!isOpened()) return;
	int b = ::fsync(_fd);
	if (b != 0) throw SGE_ERROR("file flush");
}

FileSize FileStream::fileSize() {
	_ensure_fd();
	off64_t cur, tmp;
	
	cur = ::lseek64(_fd, 0, SEEK_CUR);
	if (cur == -1) throw SGE_ERROR("file size");

	tmp = ::lseek64(_fd, 0, SEEK_END);
	if (tmp == -1) throw SGE_ERROR("file size");

	FileSize outSize = static_cast<FileSize>(tmp);

	tmp = ::lseek64(_fd, 0, SEEK_SET);
	if (tmp == -1) throw SGE_ERROR("file size");

	return outSize;
}

void FileStream::setFileSize(FileSize newSize) {
	_ensure_fd();

	FileSize	oldPos = getPos();

	off64_t o = newSize;
	if (0 != ftruncate64( _fd, o ))
		throw SGE_ERROR("setFileSize");

	if (oldPos < newSize)
		setPos( oldPos );
}

FileSize FileStream::getPos() {
	_ensure_fd();
	off64_t tmp = 0;
	off64_t ret = ::lseek64(_fd, tmp, SEEK_SET);
	if (ret == -1) throw SGE_ERROR("get file pos");
	return static_cast<FileSize>(ret);
}

void FileStream::setPos(FileSize pos) {
	_ensure_fd();
	off64_t tmp = static_cast<off64_t>(pos);
	off64_t ret = ::lseek64(_fd, tmp, SEEK_SET);
	if (ret == -1) throw SGE_ERROR("set file pos");
}

void FileStream::setPosFromEnd(FileSize pos) {
	_ensure_fd();
	off64_t tmp = static_cast<off64_t>(pos);
	off64_t ret = ::lseek64(_fd, tmp, SEEK_END);
	if (ret == -1) throw SGE_ERROR("set file pos from end");
}

void FileStream::readBytes(Span<u8> data) {
	_ensure_fd();
	if (reqSize <= 0) return 0;
	auto ret = ::read(_fd, data.data(), reqSize);
	if (ret <= 0) throw SGE_ERROR("file read");

	size_t result = static_cast<size_t>(ret);
	if (result != data.size())
		throw SGE_ERROR("file read");
}

void FileStream::writeBytes(ByteSpan data) {
	_ensure_fd();

	if (data.size() <= 0) return 0;
	auto ret = ::write(_fd, data.data(), data.size());
	if (ret <= 0) throw SGE_ERROR("error write file");

	size_t result = static_cast<size_t>(ret);
	if (result != data.size())
		throw SGE_ERROR("file read");
}

#endif

}