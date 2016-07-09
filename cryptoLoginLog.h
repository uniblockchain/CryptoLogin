/**
 * @file	cryptoLoginLog.h
 * @author	Jonathan Bedard
 * @date   	7/9/2016
 * @brief	Logging for login namespace
 * @bug	None
 *
 * This file contains declarations which are used
 * for logging within the login namespace.
 **/

#ifndef CRYPTO_LOGIN_LOG_H
#define CRYPTO_LOGIN_LOG_H

#include "Datastructures/Datastructures.h"
#include <iostream>

namespace login
{
	/** @brief Standard out pointer for login namespace
	 *
	 * This std::ostream is used as standard out
	 * for the login namespace.  This pointer can be
	 * swapped out to programmatically redirect standard out for
	 * the login namespace.
	 */
	extern os::smart_ptr<std::ostream> loginout_ptr;
	/** @brief Standard error pointer for login namespace
	 *
	 * This std::ostream is used as standard error
	 * for the login namespace.  This pointer can be
	 * swapped out to programmatically redirect standard error for
	 * the login namespace.
	 */
    extern os::smart_ptr<std::ostream> loginerr_ptr;
	/** @brief Standard out object for login namespace
	 *
	 * #define statements allow the user to call this
	 * function with "login::loginout."  Logging is achieved
	 * by using "login::loginout" as one would use "std::cout."
	 */
	std::ostream& loginout_func();
	/** @brief Standard error object for login namespace
	 *
	 * #define statements allow the user to call this
	 * function with "login::loginerr."  Logging is achieved
	 * by using "login::loginerr" as one would use "std::cerr."
	 */
	std::ostream& loginerr_func();
}

#ifndef loginout
#define loginout loginout_func()
#endif
#ifndef loginerr
#define loginerr loginerr_func()
#endif

#endif