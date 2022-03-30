#include "Error.h"

namespace sge {

Error::Error(const SrcLoc& loc, StrView msg) 
: _loc(loc), _msg(msg)
{} 

} // namespace
