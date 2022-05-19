#include "Error.h"
#include <sge_core/log/Log.h>

namespace sge {

Error::Error(const SrcLoc& loc, StrView msg) 
: _loc(loc), _msg(msg)
{
	SGE_LOG("Error: {}", msg);
	SGE_ASSERT(false);
} 

} // namespace
