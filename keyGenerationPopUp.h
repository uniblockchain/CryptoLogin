/**
 * @file	keyGenerationPopUp.h
 * @author	Jonathan Bedard
 * @date   	8/29/2016
 * @brief	Various crypto-graphic pop-ups
 * @bug	None
 *
 * Provides two pop-up forms used by the
 * CryptoLogin library.  One is used when
 * generating public keys and another is
 * used when loading user data.
 **/

#ifndef KEY_GENERATION_POP_UP_H
#define KEY_GENERATION_POP_UP_H

#include "glGraphics/glLibrary.h"
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
		/** @brief Key generation label
		 */
		gl::label lblKeyGeneration;
		/** @brief User generating key
		 */
		os::smart_ptr<crypto::user> _user;

		/** @brief Dot counter
		 */
		unsigned int dotCount;
	public:
		/** @brief Basic constructor
		 * @param [in] prev Previous form
		 * @param [in] usr User generating key
		 */
		pulblicKeyPopUp(os::smart_ptr<gl::form> prev,os::smart_ptr<crypto::user> usr);
		/** @brief Virtual destructor
         *
         * Destructor must be virtual, if an object
         * of this type is deleted, the destructor
         * of the type which inherits this class should
         * be called.
         */
		virtual ~pulblicKeyPopUp(){}
		/** @brief Triggered each update cycle
		 * @return void
		 */
		void update();
	};
	/** @brief Used when loading the user
	 *
	 * While the user is opening it's files, this
	 * is the UI which will display.
	 */
	class userLoadingPopUp: public gl::popUp
	{
		/** @brief User loading label
		 */
		gl::label lblLoadingUser;

		/** @brief Dot counter
		 */
		unsigned int dotCount;
	public:
		/** @brief Basic constructor
		 * @param [in] prev Previous form
		 * @param [in] met Meta-data loading user
		 */
		userLoadingPopUp(os::smart_ptr<gl::form> prev,loginMetaData& met,std::string _username,std::string _password);
		/** @brief Virtual destructor
         *
         * Destructor must be virtual, if an object
         * of this type is deleted, the destructor
         * of the type which inherits this class should
         * be called.
         */
		virtual ~userLoadingPopUp(){}
		/** @brief Triggered each update cycle
		 * @return void
		 */
		void update();

		/** @brief Meta-data opening a user
		 */
		os::smart_ptr<loginMetaData> meta;
		/** @brief Username string
		 */
		std::string username;
		/** @brief Password string
		 */
		std::string password;
	};

};

#endif