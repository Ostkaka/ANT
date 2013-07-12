#ifndef DATAUTILS_HPP_
	#define DATAUTILS_HPP_

#include <ant/ant_std.hpp>
#include <ant/core_types.hpp>

namespace dataUtils
{
	/// Converts Bytes to MB and vice versa
	ant::Real convertByte2MB(ant::UInt bytes);

	ant::UInt convertMB2Byte(ant::UInt MB);
}

ANT_INLINE ant::Real dataUtils::convertByte2MB(ant::UInt bytes)
{
	return (ant::Real(bytes) / (1024.0 * 1014.0));	
}

ANT_INLINE ant::UInt dataUtils::convertMB2Byte(ant::UInt MB)
{
	return (MB * 1024 * 1014);
}

#endif