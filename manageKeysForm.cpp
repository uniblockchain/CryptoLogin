/**
 * @file	createUserForm.cpp
 * @author	Jonathan Bedard
 * @date   	5/26/2016
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
	//Constructor
	publicKeyTypeFrame::publicKeyTypeFrame(os::smart_ptr<userSettingsForm> master,
		os::smart_ptr<crypto::user> usr, os::smart_ptr<crypto::publicKeyPackageFrame> pbkfrm):
		gl::frame(&master->scrArea.scrollZone()),
		elmDivider(this),lblTitle(this),
		btnExpand(this),btnGenerate(this),btnSetDefault(this)
	{
		masterForm=master;
		_user=usr;
		_pbkFrame=pbkfrm;
		_pbk=_user->findPublicKey(_pbkFrame);

		elmDivider.setColor(gl::col::black);
		elmDivider.setX(0);
		elmDivider.setHeight(5);
		
		lblTitle.setHeight(60);
		lblTitle.setX(10);
		lblTitle.setText(pbkfrm->algorithmName()+" "+std::to_string((long long unsigned int)32*pbkfrm->keySize())+" bit");

		btnExpand.setText("Expand");
		btnExpand.setX(200);
		btnExpand.pushClickedListener(this);
		btnExpand.pushEnterListener(this);
		addMouseListener(&btnExpand);
		addKeyboardListener(&btnExpand);

		btnGenerate.setText("Generate");
		btnGenerate.setX(360);
		btnGenerate.pushClickedListener(this);
		btnGenerate.pushEnterListener(this);
		addMouseListener(&btnGenerate);
		addKeyboardListener(&btnGenerate);
		if(_pbk) _pbk->keyChangeSender::pushReceivers(this);
		if(_pbk && _pbk->generating())
		{
			btnGenerate.setActive(false);
			btnGenerate.setText("Generating...");
		}

		btnSetDefault.setText("Set as default");
		btnSetDefault.setX(520);
		btnSetDefault.pushClickedListener(this);
		btnSetDefault.pushEnterListener(this);
		addMouseListener(&btnSetDefault);
		addKeyboardListener(&btnSetDefault);
		if(!_pbk || !_pbk->getN()) btnSetDefault.disable();

		saveTrigger=false;
		setHeight(60);

		expanded=false;

		updateKeyList();
		resize();
	}
	//Triggered when a key is finished generating
	void publicKeyTypeFrame::publicKeyChanged(os::smart_ptr<crypto::publicKey> pbk)
	{
		btnSetDefault.enable();
		btnGenerate.setActive(true);
		btnGenerate.setText("Generate");
		saveTrigger=true;
		masterForm->refreshDefaultPublicKeys();
	}
	//Update the key list
	void publicKeyTypeFrame::updateKeyList()
	{
		auto temp=timestampList.getFirst();
		while(temp)
		{
			timestampList.findDelete(temp->getData());
			temp=timestampList.getFirst();
		}
		temp=keyList.getFirst();
		while(temp)
		{
			keyList.findDelete(temp->getData());
			temp=keyList.getFirst();
		}
		timestampList=os::unsortedList<gl::label>();
		keyList=os::unsortedList<gl::label>();

		//Expanded case
		if(expanded)
		{
			btnExpand.setText("Collapse");
			os::smart_ptr<gl::label> lblCurrent;

			//No public key
			if(!_pbk)
			{
				lblCurrent=os::smart_ptr<gl::label>(new gl::label(this),os::shared_type);
				lblCurrent->setHeight(30);
				lblCurrent->setText("No keys of this type");
				lblCurrent->setX(30);
				keyList.insert(lblCurrent);
				setHeight(110);
			}
			else if(!_pbk->getN())
			{
				lblCurrent=os::smart_ptr<gl::label>(new gl::label(this),os::shared_type);
				lblCurrent->setHeight(30);
				lblCurrent->setText("Generating keys, hang tight!");
				lblCurrent->setX(30);
				keyList.insert(lblCurrent);
				setHeight(110);
			}
			else
			{
				size_t hist=crypto::publicKey::CURRENT_INDEX;
				int newSize=65;
				os::smart_ptr<crypto::number> num=_pbk->getOldD(hist);
				while(num)
				{
					num=_pbk->copyConvert(num);
					num->reduce();

					//Key list
					std::string temp=num->toString();
					lblCurrent=os::smart_ptr<gl::label>(new gl::label(this),os::shared_type);
					lblCurrent->setHeight(30);
					int breaks=_pbk->size()/4-1;
					int breakPos=4*9-1;
					for(int i=0;i<breaks;++i)
					{
						temp[breakPos]='\n';
						breakPos+=4*9;
						lblCurrent->setHeight(lblCurrent->height()+25);
					}
					lblCurrent->setText(temp);
					lblCurrent->setX(250);
					keyList.insert(lblCurrent);
					newSize+=lblCurrent->height()+10;

					//Timestamp
					lblCurrent=os::smart_ptr<gl::label>(new gl::label(this),os::shared_type);
					lblCurrent->setHeight(30);
					lblCurrent->setText(os::convertTimestamp(_pbk->getOldTimestamp(hist)));
					lblCurrent->setX(20);
					timestampList.insert(lblCurrent);

					hist++;
					num=_pbk->getOldN(hist);
				}
				setHeight(newSize);
			}
		}
		//Colapse case
		else
		{
			btnExpand.setText("Expand");
			setHeight(60);
		}
		masterForm->resize();
	}
	//Triggers every update cycle
	void publicKeyTypeFrame::update()
	{
		if(saveTrigger && _pbk)
		{
			saveTrigger=false;
			_pbk->save();
			updateKeyList();
		}
	}
	//Received click event
	void publicKeyTypeFrame::receivedClicked(os::smart_ptr<element> elm)
	{
		//Expand node
		if(elm==&btnExpand)
		{
			expanded=!expanded;
			updateKeyList();
			return;
		}
		//Generate keys
		if(elm==&btnGenerate)
		{
			if(_pbk)
				_pbk->generateNewKeys();
			else
			{
				_pbk=_pbkFrame->generate();
				_pbk->keyChangeSender::pushReceivers(this);
				_user->addPublicKey(_pbk);
			}
			btnGenerate.setActive(false);
			btnGenerate.setText("Generating...");
			return;
		}

		//Set default
		if(elm==&btnSetDefault)
		{
			if(_pbk) _user->setDefaultPublicKey(_pbk);
			masterForm->refreshDefaultPublicKeys();
			return;
		}
	}
	//Receive enter event
	void publicKeyTypeFrame::receivedEnter(os::smart_ptr<element> elm)
	{publicKeyTypeFrame::receivedClicked(elm);}
	//Resize event
	void publicKeyTypeFrame::resize()
	{
		gl::frame::resize();

		int trc=height();

		trc-=5;
		elmDivider.setWidth(width());
		elmDivider.setY(trc);

		trc-=60;
		lblTitle.setY(trc);

		btnExpand.setY(trc);
		btnGenerate.setY(trc);
		btnSetDefault.setY(trc);

		auto trc1=keyList.getFirst();
		auto trc2=timestampList.getFirst();
		while(trc1)
		{
			if(trc2)
				trc2->getData()->setY(trc-trc2->getData()->height());
			trc-=trc1->getData()->height();
			trc1->getData()->setY(trc);
			
			trc1=trc1->getNext();
			if(trc2)
				trc2=trc2->getNext();
			trc-=10;
		}
	}

	//Create user constructor
	userSettingsForm::userSettingsForm(gl::form* master,os::smart_ptr<crypto::user> ud):gl::navForm(master),
		scrArea(this),
		lblUsername(&scrArea.scrollZone()),
		chgStreamType(&scrArea.scrollZone()),
		chgHash(&scrArea.scrollZone()),
		lblPublicKeyTitle(&scrArea.scrollZone()),
		lblPublicKeyHashDescriptor(&scrArea.scrollZone()),lblPublicKeyHash(&scrArea.scrollZone()),
		lblPublicKeyDescriptor(&scrArea.scrollZone()),lblPublicKey(&scrArea.scrollZone())
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
		chgStreamType.pushClickedListener(this);
		chgStreamType.pushEnterListener(this);

		//Hash type label
		chgHash.setTitle("Hash type");
		chgHash.setX(30);
		chgHash.setWidth(180);
		chgHash.addString(crypto::RCFour::staticAlgorithmName()+" "+std::to_string((long long unsigned int)8*crypto::size::hash64)+" bit");
		chgHash.addString(crypto::RCFour::staticAlgorithmName()+" "+std::to_string((long long unsigned int)8*crypto::size::hash128)+" bit");
		chgHash.addString(crypto::RCFour::staticAlgorithmName()+" "+std::to_string((long long unsigned int)8*crypto::size::hash256)+" bit");
		chgHash.addString(crypto::RCFour::staticAlgorithmName()+" "+std::to_string((long long unsigned int)8*crypto::size::hash512)+" bit");
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
		chgHash.pushClickedListener(this);
		chgHash.pushEnterListener(this);

		refreshDefaultPublicKeys();

		//Add key management bits
		uint16_t algoArray[]={crypto::algo::publicRSA};
		uint16_t keyArrSize[]={crypto::size::public128,crypto::size::public256,crypto::size::public512,crypto::size::public1024,crypto::size::public2048};
		
		for(int i1=0;i1<1;i1++)
		{
			auto pbktype=crypto::publicKeyTypeBank::singleton()->findPublicKey(algoArray[i1]);
			for(int i2=0;i2<5;i2++)
			{
				pbktype=pbktype->getCopy();
				pbktype->setKeySize(keyArrSize[i2]);
				auto frm=os::smart_ptr<publicKeyTypeFrame>(new publicKeyTypeFrame(this,_userData,pbktype),os::shared_type);
				addMouseListener(frm.get());
				addKeyboardListener(frm.get());
				frameList.insert(frm);
			}
		}
		resize();
	}
	//Refresh the default public keys
	void userSettingsForm::refreshDefaultPublicKeys()
	{
		lblPublicKeyTitle.setHeight(20);
		lblPublicKeyTitle.setX(280);

		lblPublicKeyHashDescriptor.setHeight(20);
		lblPublicKeyHashDescriptor.setX(280);

		
		lblPublicKeyHash.setWidth(200);
		lblPublicKeyHash.setX(300);

		lblPublicKeyDescriptor.setHeight(20);
		lblPublicKeyDescriptor.setX(280);

		lblPublicKey.setWidth(200);
		lblPublicKey.setX(300);

		os::smart_ptr<crypto::publicKey> pbk=_userData->getDefaultPublicKey();
		if(!pbk)
		{
			lblPublicKeyTitle.setText("Default public key undefined");
			lblPublicKeyHashDescriptor.setText("No hash defined");
			lblPublicKeyHash.setText("");
			lblPublicKeyHash.setHeight(0);
			lblPublicKeyDescriptor.setText("No public key defined");
			lblPublicKey.setText("");
			lblPublicKey.setHeight(0);
			return;
		}
		lblPublicKeyHash.setHeight(20);
		lblPublicKey.setHeight(20);

		lblPublicKeyTitle.setText("Default public key is "+pbk->algorithmName()+" "+std::to_string((long long unsigned int)pbk->size()*32)+" bit");
		
		//Pull the hash
		lblPublicKeyHashDescriptor.setText(_userData->streamPackage()->hashAlgorithmName()+" "+std::to_string((long long unsigned int)8*_userData->streamPackage()->hashSize())+" bit hash of public-key:");
		size_t len;
		auto pbkData=pbk->getN()->getCompCharData(len);
		std::string hshStr;
		crypto::hash hsh=_userData->streamPackage()->hashData(pbkData.get(),len);
		hshStr=hsh.toString();
		int breaks=0;
		int breakPos;
		if(_userData->streamPackage()->hashSize()==crypto::size::hash256)
		{
			breaks=1;
			breakPos=32;
		}
		else if(_userData->streamPackage()->hashSize()==crypto::size::hash512)
		{
			breaks=3;
			breakPos=96;
		}
		for(int i=0;i<breaks;++i)
		{
			hshStr=hshStr.substr(0,breakPos)+"\n"+hshStr.substr(breakPos,hshStr.length()-breakPos);
			breakPos-=32;
			lblPublicKeyHash.setHeight(lblPublicKeyHash.height()+25);
		}
		lblPublicKeyHash.setText(hshStr);

		//Public key
		lblPublicKeyDescriptor.setText("Hex representation of public key:");
		os::smart_ptr<crypto::number> num=pbk->copyConvert(pbk->getN());
		num->reduce();
		std::string pbkstr=num->toString();
		breaks=pbk->size()/4-1;
		breakPos=4*9-1;
		for(int i=0;i<breaks;++i)
		{
			pbkstr[breakPos]='\n';
			breakPos+=4*9;
			lblPublicKey.setHeight(lblPublicKey.height()+25);
		}
		lblPublicKey.setText(pbkstr);
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

		//Change stream type
		if(elm==&chgStreamType)
		{
			//Only have one choice
			std::string targName=crypto::RCFour::staticAlgorithmName();
			os::smart_ptr<crypto::streamPackageFrame> targFrm=
				crypto::streamPackageTypeBank::singleton()->findStream(targName,_userData->streamPackage()->hashAlgorithmName());
			targFrm=targFrm->getCopy();
			targFrm->setHashSize(_userData->streamPackage()->hashSize());
			_userData->setStreamPackage(targFrm);
			_userData->markChanged();
			return;
		}

		//Change hash type
		if(elm==&chgHash)
		{
			//Only have RC-4 as a valid hash
			uint16_t targ=crypto::algo::hashRC4;
			uint16_t sz=2;

			//Search for clicked element
			for(int i=0;i<chgHash.numString();++i)
			{
				if(chgHash[i]) sz=i;
			}

			//Set the size
			switch(sz)
			{
			case 0: //Hash size 64
				sz=crypto::size::hash64;
				break;
			case 1: //Hash size 128
				sz=crypto::size::hash128;
				break;
			case 3: //Hash size 512
				sz=crypto::size::hash512;
				break;
			case 2: //Hash size 256
			default:
				sz=crypto::size::hash256;
			}

			os::smart_ptr<crypto::streamPackageFrame> targFrm=
				crypto::streamPackageTypeBank::singleton()->findStream(_userData->streamPackage()->streamAlgorithm(),targ);
			targFrm=targFrm->getCopy();
			targFrm->setHashSize(sz);
			_userData->setStreamPackage(targFrm);
			_userData->markChanged();
			refreshDefaultPublicKeys();
			return;
		}
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

		//Find size of the scroll zone
		int trcHei;
		int lgap;
		trcHei=150;
		trcHei+=5+lblPublicKeyHash.height();
		trcHei+=5+lblPublicKey.height();

		lgap=50;
		lgap+=(20+chgStreamType.height());
		lgap+=(20+chgHash.height());
		if(lgap>trcHei) trcHei=lgap;
		for(auto it=frameList.getFirst();it;it=it->getNext())
			trcHei+=it->getData()->height()+10;

		//Set y position
		if(trcHei>scrArea.height()+5)
			scrArea.scrollZone().setHeight(trcHei);
		else scrArea.scrollZone().setHeight(scrArea.height()+5);
		trcHei=scrArea.scrollZone().height();
		

		trcHei-=50;
		lblPublicKeyTitle.setY(trcHei);
		trcHei-=50;
		lblPublicKeyHashDescriptor.setY(trcHei);
		trcHei-=5+lblPublicKeyHash.height();
		lblPublicKeyHash.setY(trcHei);
		trcHei-=50;
		lblPublicKeyDescriptor.setY(trcHei);
		trcHei-=5+lblPublicKey.height();
		lblPublicKey.setY(trcHei);
		lgap=trcHei;

		trcHei=scrArea.scrollZone().height();
		trcHei-=50;
		lblUsername.setY(trcHei);
		trcHei-=(20+chgStreamType.height());
		chgStreamType.setY(trcHei);
		trcHei-=(20+chgHash.height());
		chgHash.setY(trcHei);
		if(trcHei<lgap)
			lgap=trcHei;

		//Public key frames
		trcHei=lgap;
		for(auto it=frameList.getFirst();it;it=it->getNext())
		{
			trcHei-=it->getData()->height()+10;
			it->getData()->setY(trcHei);
			it->getData()->setWidth(width()-20);
			it->getData()->setX(20);
		}
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