/**
 * @file	createUserForm.cpp
 * @author	Jonathan Bedard
 * @date   	5/3/2016
 * @brief	Implements key management form
 * @bug	None
 *
 * Implements the key management form,
 * which can be accessed from the secure side
 * side of the gateway
 **/

///@cond INTERNAL

#ifndef MANAGE_KEYS_FORM_CPP
#define CREATE_USER_FORM_CPP

#include "cryptoLoginLog.h"
#include "manageKeysForm.h"
#include "osMechanics.h"

using namespace gl;

namespace login
{
	//Create user constructor
	userSettingsForm::userSettingsForm(gl::form* master,os::smart_ptr<crypto::user> ud):gl::navForm(master),
		scrArea(this),
		lblUsername(&scrArea.scrollZone()),
		chgStreamType(&scrArea.scrollZone()),
		chgHash(&scrArea.scrollZone()),
		publicKeyHash(&scrArea.scrollZone()),
		publicKey(&scrArea.scrollZone())
	{
		setTitle("User Security Settings");
		_userData=ud;
		scrArea.lateral().setActive(false);
		scrArea.setWidth(width());
		scrArea.setHeight(height()-barTop.height()-1);
		addMouseListener(&scrArea);

		//Username label
		lblUsername.setText("Username: "+_userData->username());
		lblUsername.setX(30);
		lblUsername.setWidth(150);
		lblUsername.setHeight(20);

		//Stream type
		chgStreamType.setTitle("Stream type");
		chgStreamType.setX(30);
		chgStreamType.setWidth(180);
		chgStreamType.addString(crypto::RCFour::staticAlgorithmName());
		chgStreamType.setChoice(gl::checkbox_chooseOne);
		addMouseListener(&chgStreamType);
		addKeyboardListener(&chgStreamType);

		//Hash type label
		chgHash.setTitle("Hash type");
		chgHash.setX(30);
		chgHash.setWidth(180);
		chgHash.addString(crypto::RCFour::staticAlgorithmName()+" "+std::to_string(8*crypto::size::hash64)+" bit");
		chgHash.addString(crypto::RCFour::staticAlgorithmName()+" "+std::to_string(8*crypto::size::hash128)+" bit");
		chgHash.addString(crypto::RCFour::staticAlgorithmName()+" "+std::to_string(8*crypto::size::hash256)+" bit");
		chgHash.addString(crypto::RCFour::staticAlgorithmName()+" "+std::to_string(8*crypto::size::hash512)+" bit");
		chgHash.setChoice(gl::checkbox_chooseOne);
		int hshMark=0;
		switch(_userData->streamPackage()->hashSize())
		{
		case 8: //Hash size 64
			chgHash.setCheckbox(hshMark,true);
			break;
		case 16: //Hash size 128
			chgHash.setCheckbox(hshMark+1,true);
			break;
		case 64: //Hash size 512
			chgHash.setCheckbox(hshMark+3,true);
			break;
		case 32: //Hash size 256
		default:
			chgHash.setCheckbox(hshMark+2,true);
		}
		addMouseListener(&chgHash);
		addKeyboardListener(&chgHash);


		resize();
	}
	//Triggers every update cycle
	void userSettingsForm::update()
	{
		navForm::update();
	}
	//Received events
	void userSettingsForm::receivedClicked(os::smart_ptr<element> elm)
	{
		navForm::receivedClicked(elm);

		
	}
	//Receive enter
	void userSettingsForm::receivedEnter(os::smart_ptr<element> elm)
	{
		gl::enterListener::receivedEnter(elm);
		userSettingsForm::receivedClicked(elm);
	}

	//Triggered on resize
	void userSettingsForm::resize()
	{
		scrArea.setWidth(width());
		scrArea.setHeight(height()-barTop.height()-1);

		scrArea.scrollZone().setHeight(scrArea.height()+5);
		int trcHei=scrArea.scrollZone().height();

		trcHei-=50;
		lblUsername.setY(trcHei);
		trcHei-=(20+chgStreamType.height());
		chgStreamType.setY(trcHei);
		trcHei-=(20+chgHash.height());
		chgHash.setY(trcHei);
	}

/*-------------------------------------
	Key Bank Form
 --------------------------------------*/

	//Create user constructor
	keyBankForm::keyBankForm(gl::form* master,os::smart_ptr<crypto::keyBank> kb):gl::navForm(master),
		scrArea(this)
	{
		setTitle("Key Bank Management");
		_keyBank=kb;
		scrArea.lateral().setActive(false);
		scrArea.setWidth(width());
		scrArea.setHeight(height()-barTop.height()-1);
		addMouseListener(&scrArea);
		

		resize();
	}
	//Triggers every update cycle
	void keyBankForm::update()
	{
		navForm::update();
	}
	
	//Received events
	void keyBankForm::receivedClicked(os::smart_ptr<element> elm)
	{
		navForm::receivedClicked(elm);

		
	}
	//Receive enter
	void keyBankForm::receivedEnter(os::smart_ptr<element> elm)
	{
		gl::enterListener::receivedEnter(elm);
		keyBankForm::receivedClicked(elm);
	}

	//Triggered on resize
	void keyBankForm::resize()
	{
		scrArea.setWidth(width());
		scrArea.setHeight(height()-barTop.height()-1);
	}
}

#endif

///@endcond