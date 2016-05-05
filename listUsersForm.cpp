/**
 * @file	listUsers.cpp
 * @author	Jonathan Bedard
 * @date   	4/21/2016
 * @brief	Implementation of user-list form
 * @bug	None
 *
 * Implements a form to list and load users which this
 * application is aware of.  Consult listUserForm.h for
 * details.
 **/

///@cond INTERNAL

#ifndef LIST_USERS_FORM_CPP
#define LIST_USERS_FORM_CPP

#include "cryptoLoginLog.h"
#include "listUsersForm.h"
#include "osMechanics.h"

using namespace gl;

namespace login
{

//List Users Form------------------------------------------------------------

	userFrame::userFrame(listUsers& master,os::smart_ptr<userNode> usr):gl::frame(&master.scrArea.scrollZone()),
		lblUsername(this),lblTimestamp(this),lblPassword(this),
		btnRemove(this),btnLoad(this)
	{
		_user=usr;
		mst=&master;

		if(usr->password=="") setHeight(120);
		else setHeight(160);

		lblUsername.setText(usr->username);
		lblUsername.setX(0);
		lblUsername.setY(height()-30);

		lblTimestamp.setText("Last accessed: "+os::convertTimestamp(usr->timestamp));
		lblTimestamp.setX(40);
		lblTimestamp.setY(height()-60);

		if(usr->password!="") lblPassword.setText("Password: \""+usr->password+"\"");
		lblPassword.setX(40);
		lblPassword.setY(height()-100);

		btnRemove.setText("Remove");
		addMouseListener(&btnRemove);
		addKeyboardListener(&btnRemove);
		btnRemove.setTextColor(gl::col::white);
		btnRemove.setColor(gl::col::red);
		btnRemove.setHeight(40);
		btnRemove.setX(70);
		btnRemove.pushClickedListener(this);
		btnRemove.pushEnterListener(this);

		if(usr->password=="") btnRemove.setY(height()-130);
		else btnRemove.setY(height()-170);

		if(usr->userExists)
		{
			btnLoad.setText("Load");
			addMouseListener(&btnLoad);
			addKeyboardListener(&btnLoad);
			btnLoad.setTextColor(gl::col::black);
			btnLoad.setColor(gl::col::green);
			btnLoad.setHeight(40);
			btnLoad.setX(250);
			btnLoad.pushClickedListener(this);
			btnLoad.pushEnterListener(this);
			if(usr->password=="") btnLoad.setY(height()-130);
			else btnLoad.setY(height()-170);
		}
		else btnLoad.disable();
	}
	//Received clicked element
	void userFrame::receivedClicked(os::smart_ptr<element> elm)
	{
		//Remove node
		if(elm==&btnRemove)
		{
			if(_user->userExists)
				os::delete_file(mst->metaData.savePath()+"/"+USERS_FOLDER+"/"+_user->username);
			if(mst->metaData.defaultUsername==_user->username)
			{
				mst->metaData.defaultUsername="";
				mst->metaData.defaultPassword="";
				mst->metaData.needsBinding=true;
			}
			mst->metaData.users.findDelete(_user);
			mst->metaData.markChanged();
			mst->needRefresh=true;
			return;
		}
		//Open node
		if(elm==&btnLoad)
		{
			mst->metaData.defaultUsername=_user->username;
			mst->metaData.defaultPassword=_user->password;
			mst->metaData.needsBinding=true;
			mst->metaData.markChanged();
			mst->close();
			return;
		}
	}

//List Users Form------------------------------------------------------------

	//Create user constructor
	listUsers::listUsers(gl::form* master,std::string baseTitle,loginMetaData& metaDataPointer):gl::navForm(master),
		metaData(metaDataPointer),scrArea(this)
	{
		//Set up basics
		setTitle(baseTitle+": List User");
		scrArea.setWidth(width());
		scrArea.setHeight(height()-barTop.height()-1);
		scrArea.lateral().setActive(false);
		addMouseListener(&scrArea);

		needRefresh=false;
		scrArea.resize();
		refreshFromMeta();
	}

	//Triggers refresh
	void listUsers::update()
	{
		if(needRefresh) refreshFromMeta();
		needRefresh=false;

		if(metaData.needsSaving())
			metaData.save();
	}
	//Iterates through meta-data
	void listUsers::refreshFromMeta()
	{
		metaData.checkUserData();

		frameDisplay=os::smartSet<userFrame>();
		auto trc=metaData.users.getLast();
		int ty=10;
		while(trc)
		{
			os::smart_ptr<userFrame> usr(new userFrame(*this,trc->getData()),os::shared_type);
			usr->setX(30);
			ty+=10+usr->height();
			
			addMouseListener(usr.get());
			addKeyboardListener(usr.get());
			frameDisplay.insert(usr);
			trc=trc->getPrev();
		}
		if(ty>scrArea.height())
			scrArea.scrollZone().setHeight(ty);
		else
			scrArea.scrollZone().setHeight(scrArea.height());
		ty=0;
		for(auto i=frameDisplay.getLast();i;i=i->getPrev())
		{
			ty+=10+i->getData()->height();
			i->getData()->setY(scrArea.scrollZone().height()-ty);
		}
		scrArea.resize();
	}
}

#endif

///@endcond