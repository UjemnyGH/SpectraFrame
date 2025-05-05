#pragma once
#ifndef _SPECTRAFRAME_LOGGER_
#define _SPECTRAFRAME_LOGGER_

#include <iostream>

namespace sf {
	/**
	 * @brief Console log
	 */
#define SF_CLOG(msg) std::cout << "[SF_LOG]: " << __FILE__ << ":" << __LINE__ << " > " << msg << std::endl

#ifdef NDEBUG
#define SF_CDEBUG(msg)
#else
#define SF_CDEBUG(msg) std::cout << "[SF_DEBUG]: " << __FILE__ << ":" << __LINE__ << " > " << msg << std::endl
#endif
}

#endif