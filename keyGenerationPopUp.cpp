/**
 * @file	keyGenerationPopUp.cpp
 * @author	Jonathan Bedard
 * @date   	4/12/2016
 * @brief	Key generation pop-up
 * @bug	None
 *
 * Implements the key-generation pop-up
 * which provides a UI while public keys
 * are being generated.
 **/

///@cond INTERNAL

#ifndef KEY_GENERATION_POP_UP_CPP
#define KEY_GENERATION_POP_UP_CPP

#include "cryptoLoginLog.h"
#include "keyGenerationPopUp.h"
#include "osMechanics.h"

using namespace gl;

namespace login
{
	pulblicKeyPopUp::pulblicKeyPopUp(os::smart_ptr<form> prev,os::smart_ptr<crypto::user> usr):
		popUp(prev),lblKeyGeneration(&popUpFrame())
	{
		lblKeyGeneration.setWidth(popUpFrame().width());
		lblKeyGeneration.setHeight(popUpFrame().height());
		lblKeyGeneration.setLateralLayout(gl::layout_center);
		lblKeyGeneration.setVerticalLayout(gl::layout_middle);
		lblKeyGeneration.setX(0);
		lblKeyGeneration.setY(0);
		lblKeyGeneration.setText("Generating Keys");
		setTitle("Generating Keys");
		_user=usr;
		dotCount=0;
	}
	void pulblicKeyPopUp::update()
	{
		if(dotCount%40 == 39)
			lblKeyGeneration.setText(lblKeyGeneration.text()+".");
		dotCount++;
		dotCount=dotCount%200;
		if(dotCount==0)
			lblKeyGeneration.setText("Generating Keys");

		//Check if it is time to close
		if(!_user)
		{
			close();
			return;
		}
		if(!_user->getDefaultPublicKey())
		{
			close();
			return;
		}
		if(!_user->getDefaultPublicKey()->generating())
		{
			_user->getDefaultPublicKey()->save();
			close();
			return;
		}
	}

	userLoadingPopUp::userLoadingPopUp(os::smart_ptr<form> prev,os::smart_ptr<loginMetaData> met):
		popUp(prev),lblLoadingUser(&popUpFrame())
	{
		lblLoadingUser.setWidth(popUpFrame().width());
		lblLoadingUser.setHeight(popUpFrame().height());
		lblLoadingUser.setLateralLayout(gl::layout_center);
		lblLoadingUser.setVerticalLayout(gl::layout_middle);
		lblLoadingUser.setX(0);
		lblLoadingUser.setY(0);
		lblLoadingUser.setText("Loading User");
		setTitle("Loading User");
		_meta=met;
		dotCount=0;
	}
	void userLoadingPopUp::update()
	{
		if(dotCount%40 == 39)
			lblLoadingUser.setText(lblLoadingUser.text()+".");
		dotCount++;
		dotCount=dotCount%200;
		if(dotCount==0)
			lblLoadingUser.setText("Loading User");

		//Check if it is time to close
		if(!_meta)
		{
			close();
			return;
		}
		if(!_meta->needsSaving())
		{
			close();
			return;
		}
	}
}

#endif

///@endcond