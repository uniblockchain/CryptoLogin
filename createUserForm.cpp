/**
 * @file	createUserForm.cpp
 * @author	Jonathan Bedard
 * @date   	4/21/2016
 * @brief	Implements the user creation form
 * @bug	None
 *
 * Implements the user creation form.  Consult
 * createUserForm.h for details.
 **/

///@cond INTERNAL

#ifndef CREATE_USER_FORM_CPP
#define CREATE_USER_FORM_CPP

#include "cryptoLoginLog.h"
#include "loginMain.h"
#include "osMechanics.h"

using namespace gl;

namespace login
{
	//Create user constructor
	createUser::createUser(gl::form* master,std::string baseTitle,loginMetaData& md):gl::navForm(master),
		lblUser(this),tbxUser(this),indUser(this),
		lblPassword1(this),tbxPassword1(this),indPassword(this),
		lblPassword2(this),tbxPassword2(this),
		btnRSA128(this),btnRSA256(this),btnRSA512(this),
		btnQuit(this),btnCreate(this),metaData(md)
	{
		setTitle(baseTitle+": New User");
		
		//User
		lblUser.setX(20);
		lblUser.setY(height()-70);
		lblUser.setWidth(120);
		lblUser.setLateralLayout(gl::layout_right);
		lblUser.setText("User-name:");

		tbxUser.setX(150);
		tbxUser.setY(height()-95);
		tbxUser.setWidth(200);
		tbxUser.pushClickedListener(this);
		tbxUser.pushEnterListener(this);
		tbxUser.focus();
		addMouseListener(&tbxUser);
		addKeyboardListener(&tbxUser);

		indUser.setX(360);
		indUser.setY(height()-90);
		indUser.setWidth(30);
		indUser.setHeight(30);

		//Password 1
		lblPassword1.setX(20);
		lblPassword1.setY(height()-120);
		lblPassword1.setWidth(120);
		lblPassword1.setLateralLayout(gl::layout_right);
		lblPassword1.setText("Password:");
		
		tbxPassword1.setTextHidden(true);
		tbxPassword1.setX(150);
		tbxPassword1.setY(height()-145);
		tbxPassword1.setWidth(200);
		tbxPassword1.pushClickedListener(this);
		tbxPassword1.pushEnterListener(this);
		addMouseListener(&tbxPassword1);
		addKeyboardListener(&tbxPassword1);

		indPassword.setX(360);
		indPassword.setY(height()-140);
		indPassword.setWidth(30);
		indPassword.setHeight(30);

		//Password 2
		lblPassword2.setX(20);
		lblPassword2.setY(height()-170);
		lblPassword2.setWidth(120);
		lblPassword2.setLateralLayout(gl::layout_right);
		lblPassword2.setText("Confirm:");

		tbxPassword2.setTextHidden(true);
		tbxPassword2.setX(150);
		tbxPassword2.setY(height()-195);
		tbxPassword2.setWidth(200);
		tbxPassword2.pushClickedListener(this);
		tbxPassword2.pushEnterListener(this);
		addMouseListener(&tbxPassword2);
		addKeyboardListener(&tbxPassword2);

		//RSA 128
		btnRSA128.setText("RSA: 128 bit");
		btnRSA128.setX(10);
		btnRSA128.setY(height()-250);
		btnRSA128.setHeight(40);
		addMouseListener(&btnRSA128);
		addKeyboardListener(&btnRSA128);
		btnRSA128.pushClickedListener(this);
		btnRSA128.pushEnterListener(this);

		btnRSA128.setActiveColor(gl::col::red);
		btnRSA128.setActiveTextColor(gl::col::white);
		btnRSA128.setInactiveColor(gl::col::green);
		btnRSA128.setInactiveTextColor(gl::col::black);

		//RSA 256
		btnRSA256.setText("RSA: 256 bit");
		btnRSA256.setX((width()-150)/2);
		btnRSA256.setY(height()-250);
		btnRSA256.setHeight(40);
		addMouseListener(&btnRSA256);
		addKeyboardListener(&btnRSA256);
		btnRSA256.pushClickedListener(this);
		btnRSA256.pushEnterListener(this);

		btnRSA256.setActiveColor(gl::col::red);
		btnRSA256.setActiveTextColor(gl::col::white);
		btnRSA256.setInactiveColor(gl::col::green);
		btnRSA256.setInactiveTextColor(gl::col::black);

		//RSA 512
		btnRSA512.setText("RSA: 512 bit");
		btnRSA512.setX(width()-160);
		btnRSA512.setY(height()-250);
		btnRSA512.setHeight(40);
		addMouseListener(&btnRSA512);
		addKeyboardListener(&btnRSA512);
		btnRSA512.pushClickedListener(this);
		btnRSA512.pushEnterListener(this);

		btnRSA512.setActiveColor(gl::col::red);
		btnRSA512.setActiveTextColor(gl::col::white);
		btnRSA512.setInactiveColor(gl::col::green);
		btnRSA512.setInactiveTextColor(gl::col::black);
		keyCreation=0;

		//Create button
		btnCreate.setText("Create User");
		btnCreate.setWidth(200);
		btnCreate.setX((width()-btnCreate.width())/2);
		btnCreate.setY(height()-320);
		addMouseListener(&btnCreate);
		addKeyboardListener(&btnCreate);
		btnCreate.pushClickedListener(this);
		btnCreate.pushEnterListener(this);

		btnCreate.setActiveColor(gl::col::gray);
		btnCreate.setActiveTextColor(gl::col::black);
		btnCreate.setInactiveColor(gl::col::inactiveGray);
		btnCreate.setInactiveTextColor(gl::col::gray);

		//Quit
		btnQuit.setText("Quit");
		btnQuit.setX(340);
		btnQuit.setY(10);
		addMouseListener(&btnQuit);
		addKeyboardListener(&btnQuit);
		btnQuit.pushClickedListener(this);
		btnQuit.pushEnterListener(this);

		updateIndicators();
	}
	//Triggers every update cycle
	void createUser::update()
	{
		if(metaData.needsSaving())
			metaData.save();
	}
	//Update indicators
	void createUser::updateIndicators()
	{
		btnCreate.setActive(true);
		if(tbxUser.trueText().length()>0 && !metaData.findUser(tbxUser.trueText()))
			indUser.setColor(gl::col::green);
		else
		{
			indUser.setColor(gl::col::red);
			btnCreate.setActive(false);
		}
		if(tbxPassword1.trueText().length()>0 && tbxPassword1.trueText()==tbxPassword2.trueText())
			indPassword.setColor(gl::col::green);
		else
		{
			indPassword.setColor(gl::col::red);
			btnCreate.setActive(false);
		}

		//Key creation
		btnRSA128.setActive(true);
		btnRSA256.setActive(true);
		btnRSA512.setActive(true);
		if(keyCreation==0)
			btnRSA128.setActive(false);
		if(keyCreation==1)
			btnRSA256.setActive(false);
		if(keyCreation==2)
			btnRSA512.setActive(false);
	}
	//Received events
	void createUser::receivedClicked(os::smart_ptr<element> elm)
	{
		navForm::receivedClicked(elm);

		//Check key type
		if(elm==&btnRSA128)
		{
			keyCreation=0;
			updateIndicators();
			return;
		}
		if(elm==&btnRSA256)
		{
			keyCreation=1;
			updateIndicators();
			return;
		}
		if(elm==&btnRSA512)
		{
			keyCreation=2;
			updateIndicators();
			return;
		}

		//Quit
		if(elm==&btnQuit)
		{
			glutLeaveMainLoop();
			return;
		}

		//Text box cases
		if(elm==&tbxUser || elm==&tbxPassword1 || elm==&tbxPassword2)
		{
			updateIndicators();
			return;
		}

		//Generate user
		if(elm==&btnCreate)
		{
			os::smart_ptr<crypto::user> usr;
			try
			{
				usr=metaData.createUser(tbxUser.trueText(),tbxPassword1.trueText());
				if(!usr) throw std::string("User could not be created");
				if(usr->numberErrors()>0)
					throw usr->popError()->errorTitle();
				
				//Add 128 bit key
				if(keyCreation==0)
					usr->addPublicKey(os::smart_ptr<crypto::publicKey>(new crypto::publicRSA(crypto::size::public128),os::shared_type));
				//Add 256 bit key
				else if(keyCreation==1)
					usr->addPublicKey(os::smart_ptr<crypto::publicKey>(new crypto::publicRSA(crypto::size::public256),os::shared_type));
				//Add 512 bit key
				else if(keyCreation==2)
					usr->addPublicKey(os::smart_ptr<crypto::publicKey>(new crypto::publicRSA(crypto::size::public512),os::shared_type));
				//Default key
				else
					usr->addPublicKey(os::smart_ptr<crypto::publicKey>(new crypto::publicRSA(),os::shared_type));
			}
			catch(std::string str)
			{
				open(os::smart_ptr<gl::form>(new gl::singleButtonPopUp(this,str),os::shared_type));
				return;
			}
			catch(...)
			{
				open(os::smart_ptr<gl::form>(new gl::singleButtonPopUp(this,"Unknown error when creating user"),os::shared_type));
				return;
			}
			close();
			return;
		}
	}
	//Receive enter
	void createUser::receivedEnter(os::smart_ptr<element> elm)
	{
		gl::enterListener::receivedEnter(elm);
		createUser::receivedClicked(elm);

		//Jump from textbox to textbox
		if(elm==&tbxUser) tbxPassword1.focus();
		else if(elm==&tbxPassword1) tbxPassword2.focus();
		else if(elm==&tbxPassword2) btnCreate.focus();
	}
	//Unpress a key
	bool createUser::keyUnpress(const gl::key& _key)
	{
		if(gl::navForm::keyUnpress(_key))
		{
			updateIndicators();
			return true;
		}
		return false;
	}
}

#endif

///@endcond