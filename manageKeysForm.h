//Primary author: Jonathan Bedard
//Confirmed working: 8/29/2016

#ifndef MANAGE_KEYS_FORM_H
#define MANAGE_KEYS_FORM_H

#include "glGraphics/glLibrary.h"
#include "loginMetaData.h"

namespace login{

	class userSettingsForm;

	//Public key management node
	class publicKeyTypeFrame: public gl::frame, public gl::enterListener, public gl::clickedListener, public crypto::keyChangeReceiver
	{
		os::smart_ptr<userSettingsForm> masterForm;
		os::smart_ptr<crypto::user> _user;
		os::smart_ptr<crypto::publicKeyPackageFrame> _pbkFrame;
		os::smart_ptr<crypto::publicKey> _pbk;

		os::pointerUnsortedList<gl::label> timestampList;
		os::pointerUnsortedList<gl::label> keyList;

		bool saveTrigger;
		bool expanded;

		void updateKeyList();
	protected:
		void publicKeyChanged(os::smart_ptr<crypto::publicKey> pbk);
		void update();
	public:
		gl::element elmDivider;
		gl::label lblTitle;
		gl::button btnExpand;
		gl::activeDisplayButton btnGenerate;
		gl::button btnSetDefault;

		publicKeyTypeFrame(os::smart_ptr<userSettingsForm> master,os::smart_ptr<crypto::user> usr, os::smart_ptr<crypto::publicKeyPackageFrame> pbkfrm);
		virtual ~publicKeyTypeFrame()throw(){if(_pbk)_pbk->save();}

		//Received events
		void receivedClicked(os::smart_ptr<element> elm);
		void receivedEnter(os::smart_ptr<element> elm);

		void resize();
        
        #undef CURRENT_CLASS
        #define CURRENT_CLASS publicKeyTypeFrame
        POINTER_COMPARE
        POINTER_HASH_CAST
        COMPARE_OPERATORS
	};
	//User settings
	class userSettingsForm: public gl::navForm, public gl::enterListener
	{
		friend class publicKeyTypeFrame;

		os::smart_ptr<crypto::user> _userData;
		gl::scrollArea scrArea;

		void refreshDefaultPublicKeys();
		os::pointerUnsortedList<publicKeyTypeFrame> frameList;
	protected:
		void update();
	public:
		gl::label lblUsername;
		
		gl::checkboxGroup chgStreamType;
		gl::checkboxGroup chgHash;
		gl::label lblPublicKeyTitle;
		gl::label lblPublicKeyHashDescriptor;
		gl::label lblPublicKeyHash;
		gl::label lblPublicKeyDescriptor;
		gl::label lblPublicKey;

		userSettingsForm(gl::form* master,os::smart_ptr<crypto::user> ud);
		virtual ~userSettingsForm() throw(){}

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