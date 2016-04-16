/**
 * @file	loginMain.cpp
 * @author	Jonathan Bedard
 * @date   	3/7/2016
 * @brief	Miscellaneous implementation for login namespace
 * @bug	None
 *
 * Implements a number of functions and objects
 * used in the login namespaces.  In particular, this
 * file implements the logging structures for the login
 * namespace.
 **/

///@cond INTERNAL

#ifndef LOGIN_MAIN_CPP
#define LOGIN_MAIN_CPP

#include "cryptoLoginLog.h"
#include "loginMain.h"
#include "osMechanics.h"

static std::string _loginPath="";

namespace login
{
	os::smart_ptr<std::ostream> loginout_ptr=&std::cout;
	os::smart_ptr<std::ostream> loginerr_ptr=&std::cerr;
	
	//Login out function
	std::ostream& loginout_func()
	{
		return *loginout_ptr;
	}
	//Login error function
	std::ostream& loginerr_func()
	{
		return *loginerr_ptr;
	}

}

#endif

///@endcond