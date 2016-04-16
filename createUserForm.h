//Primary author: Jonathan Bedard
//Confirmed working: 1/25/2016

#ifndef CREATE_USER_FORM_H
#define CREATE_USER_FORM_H

#include "glLibrary.h"
#include "loginMetaData.h"

namespace login{

	//Login form
	class createUser: public gl::navForm, public gl::enterListener
	{
		gl::label lblUser;
		gl::textbox tbxUser;
		gl::label indUser;

		gl::label lblPassword1;
		gl::textbox tbxPassword1;
		gl::label indPassword;

		gl::label lblPassword2;
		gl::textbox tbxPassword2;

		gl::activeDisplayButton btnRSA128;
		gl::activeDisplayButton btnRSA256;
		gl::activeDisplayButton btnRSA512;

		gl::activeDisplayButton btnCreate;
		gl::button btnQuit;

		loginMetaData& metaData;
		unsigned int keyCreation;
	protected:
		void update();
	public:
		createUser(gl::form* master,std::string baseTitle,loginMetaData* metaDataPointer);
		virtual ~createUser(){}

		//Update indicators
		void updateIndicators();

		//Received events
		void receivedClicked(os::smart_ptr<element> elm);
		void receivedEnter(os::smart_ptr<element> elm);
		bool keyUnpress(const gl::key& _key);
	};

};

#endif