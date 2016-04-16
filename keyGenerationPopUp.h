//Primary author: Jonathan Bedard
//Confirmed working: 4/13/2016

#ifndef KEY_GENERATION_POP_UP_H
#define KEY_GENERATION_POP_UP_H

#include "glLibrary.h"
#include "loginMetaData.h"

namespace login{

	/** @brief Used when generating keys
	 *
	 * When generating keys, the UI displays
	 * this form until the user can be accessed
	 * with a valid public key.
	 */
	class pulblicKeyPopUp: public gl::popUp
	{
		gl::label lblKeyGeneration;
		os::smart_ptr<crypto::user> _user;

		unsigned int dotCount;
	public:
		pulblicKeyPopUp(os::smart_ptr<gl::form> prev,os::smart_ptr<crypto::user> usr);
		virtual ~pulblicKeyPopUp(){}
		void update();
	};
	/** @brief Used when loading the user
	 *
	 * While the user is opening it's files, this
	 * is the UI which will display.
	 */
	class userLoadingPopUp: public gl::popUp
	{
		gl::label lblLoadingUser;
		os::smart_ptr<loginMetaData> _meta;

		unsigned int dotCount;
	public:
		userLoadingPopUp(os::smart_ptr<gl::form> prev,os::smart_ptr<loginMetaData> met);
		virtual ~userLoadingPopUp(){}
		void update();
	};

};

#endif