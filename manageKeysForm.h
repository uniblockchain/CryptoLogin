//Primary author: Jonathan Bedard
//Confirmed working: 5/3/2016

#ifndef MANAGE_KEYS_FORM_H
#define MANAGE_KEYS_FORM_H

#include "glLibrary.h"
#include "loginMetaData.h"

namespace login{

	//User settings
	class userSettingsForm: public gl::navForm, public gl::enterListener
	{
		os::smart_ptr<crypto::user> _userData;
		gl::scrollArea scrArea;
	protected:
		void update();
	public:
		gl::label lblUsername;
		
		gl::checkboxGroup chgStreamType;
		gl::checkboxGroup chgHash;
		gl::label publicKeyHash;
		gl::label publicKey;

		userSettingsForm(gl::form* master,os::smart_ptr<crypto::user> ud);
		virtual ~userSettingsForm(){}

		//Received events
		void receivedClicked(os::smart_ptr<element> elm);
		void receivedEnter(os::smart_ptr<element> elm);

		void resize();
	};

	//Key bank form
	class keyBankForm: public gl::navForm, public gl::enterListener
	{
		os::smart_ptr<crypto::keyBank> _keyBank;
		gl::scrollArea scrArea;
	protected:
		void update();
	public:
		keyBankForm(gl::form* master,os::smart_ptr<crypto::keyBank> kb);
		virtual ~keyBankForm(){}

		//Received events
		void receivedClicked(os::smart_ptr<element> elm);
		void receivedEnter(os::smart_ptr<element> elm);

		void resize();
	};
};

#endif