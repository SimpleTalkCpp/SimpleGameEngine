#pragma once

#include "../string/Fmt.h"

#define SGE_ERROR(...) Error(SGE_LOC, Fmt(__VA_ARGS__))

namespace sge {

class IError : public std::exception {
public:

};

class Error : public IError {
public:
//	Error() = default;
	Error(const SrcLoc& loc, StrView msg);

private:
	SrcLoc _loc;
	TempString _msg;
};


} // namespace