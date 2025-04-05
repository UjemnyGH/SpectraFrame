#pragma once
#ifndef _SPECTRAFRAME_LOGGER_
#define _SPECTRAFRAME_LOGGER_

namespace sf {
	/**
	 * @brief Console log
	 */
#define SF_CLOG(msg) std::cout << "[LOG]: " << __FILE__ << ":" << __LINE__ << " > " << msg << std::endl;
}

#endif