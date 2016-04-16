/**
 * @file	loginMetaData.h
 * @author	Jonathan Bedard
 * @date   	4/13/2016
 * @brief	Impliments login-form meta-data
 * @bug	None
 *
 * Impliments the login meta-data class.
 * Consult loginMetaData.h for details.
 **/

///@cond INTERNAL

#ifndef LOGIN_META_DATA_CPP
#define LOGIN_META_DATA_CPP

#include "cryptoLoginLog.h"
#include "loginMetaData.h"
#include "osMechanics.h"
#include <sstream>

namespace login
{
	const char* META_FILE="loginMetaData.xml";
	const char* USERS_FOLDER="Users";

	//Meta-data constructor
	loginMetaData::loginMetaData(std::string svPath)
	{
		_savePath=svPath;

		os::testCreateFolder(_savePath);
		os::testCreateFolder(_savePath+"/"+USERS_FOLDER);
		markChanged();

		needsBinding=true;
	}
	//Copy constructor
	loginMetaData::loginMetaData(loginMetaData& cpy)
	{
		cpy.lock.increment();
		defaultUsername=cpy.defaultUsername;
		defaultPassword=cpy.defaultPassword;

		_savePath=cpy._savePath;
		users.insert(&cpy.users);
		os::testCreateFolder(_savePath);
		markChanged();

		needsBinding=true;
		cpy.lock.decrement();
	}
	
	//Generate save tree
	os::smartXMLNode loginMetaData::generateSaveTree()
	{
		lock.increment();
		os::smartXMLNode head=os::smartXMLNode(new os::XML_Node("loginData"),os::shared_type);

		//Saved meta-data
		os::smartXMLNode lv1=os::smartXMLNode(new os::XML_Node("saved"),os::shared_type);
		os::smartXMLNode lv2=os::smartXMLNode(new os::XML_Node("username"),os::shared_type);
		lv2->setData(defaultUsername);
		lv1->addElement(lv2);

		lv2=os::smartXMLNode(new os::XML_Node("password"),os::shared_type);
		lv2->setData(defaultPassword);
		lv1->addElement(lv2);
		head->addElement(lv1);

		//User list
		lv1=os::smartXMLNode(new os::XML_Node("users"),os::shared_type);
		for(auto it=users.getFirst();it;it=it->getNext())
		{
			lv2=os::smartXMLNode(new os::XML_Node("user"),os::shared_type);
			os::smartXMLNode lv3=os::smartXMLNode(new os::XML_Node("name"),os::shared_type);
			lv3->setData(it->getData()->username);
			lv2->addElement(lv3);

			lv3=os::smartXMLNode(new os::XML_Node("password"),os::shared_type);
			lv3->setData(it->getData()->password);
			lv2->addElement(lv3);

			lv3=os::smartXMLNode(new os::XML_Node("timestamp"),os::shared_type);
			lv3->setData(std::to_string(it->getData()->timestamp));
			lv2->addElement(lv3);

			lv1->addElement(lv2);
		}
		head->addElement(lv1);
		lock.decrement();

		return head;
	}

	//Checks all user data
	void loginMetaData::checkUserData()
	{
		lock.lock();
		uint32_t arrayLen;
		os::smart_ptr<std::string> arr=os::list_files(_savePath+"/"+USERS_FOLDER,arrayLen);

		//First, chop off excess parts
		for(unsigned int i=0;i<arrayLen;i++)
			arr[i]=os::extract_name(arr[i]);

		//Next, add all read directories to list
		for(unsigned int i=0;i<arrayLen;i++)
		{
			os::smart_ptr<userNode> usNode(new userNode(arr[i]),os::shared_type);
			usNode->userExists=true;
			users.insert(usNode);
		}

		//Check all declared users, mark if the directory exists
		for(auto trc=users.getFirst();trc;trc=trc->getNext())
		{
			trc->getData()->userExists=false;
			for(unsigned int i=0;i<arrayLen && !trc->getData()->userExists;i++)
			{
				if(arr[i]==trc->getData()->username)
					trc->getData()->userExists=true;
			}
		}
		lock.unlock();
	}
	//Load meta-data
	void loginMetaData::load()
	{
		lock.lock();
		os::smartXMLNode xmNode=os::XML_Input(_savePath+"/"+META_FILE);
		
		//Parse read tree
		try
		{
			//Jump out
			if(!xmNode) throw -1;

			//Find default username and password
			os::smartXMLNodeList searchList=xmNode->findElement("saved");
			if(!searchList) throw -1;
			if(searchList->size()!=1) throw -1;
			os::smartXMLNode saveNode=searchList->getFirst()->getData();

			searchList=saveNode->findElement("username");
			if(!searchList) throw -1;
			if(searchList->size()!=1) throw -1;
			defaultUsername=searchList->getFirst()->getData()->getData();

			searchList=saveNode->findElement("password");
			if(!searchList) throw -1;
			if(searchList->size()!=1) throw -1;
			defaultPassword=searchList->getFirst()->getData()->getData();

			//Find all users
			searchList=xmNode->findElement("users");
			if(!searchList) throw -1;
			if(searchList->size()!=1) throw -1;
			os::smartXMLNode usrList=searchList->getFirst()->getData();
			searchList=usrList->findElement("user");
			if(!searchList) throw -1;

			//Iterate through user list
			for(auto it=searchList->getFirst();it;it=it->getNext())
			{
				try
				{
					//Find name
					os::smartXMLNodeList usrList=it->getData()->findElement("name");
					if(!usrList) throw -1;
					if(usrList->size()!=1) throw -1;
					os::smart_ptr<userNode> insNode(new userNode(usrList->getFirst()->getData()->getData()));
                    os::smart_ptr<os::adnode<userNode> > temp=users.find(insNode);
					if(temp)
						insNode=temp->getData();

					//Search password
					usrList=it->getData()->findElement("password");
					if(!usrList) throw -1;
					if(usrList->size()!=1) throw -1;
					insNode->password=usrList->getFirst()->getData()->getData();

					//Search timestamp
					usrList=it->getData()->findElement("timestamp");
					if(!usrList) throw -1;
					if(usrList->size()!=1) throw -1;
					std::stringstream(usrList->getFirst()->getData()->getData())>>insNode->timestamp;

					users.insert(insNode);
				}
				catch(...){}
			}
		}
		catch(...)
		{}
		lock.unlock();

		//Find any existant users
		checkUserData();

		markChanged();
	}
	//Save meta-data
	void loginMetaData::save()
	{
		lock.increment();
		os::XML_Output(_savePath+"/"+META_FILE,generateSaveTree());
		savingGroup::save();
		finishedSaving();
		lock.decrement();
	}

//User Access----------------------------------------------------------------

	//Find user
	os::smart_ptr<userNode> loginMetaData::findUser(std::string usr)
	{

		auto fnd= users.find(os::smart_ptr<userNode>(new userNode(usr),os::shared_type));
		if(!fnd) return NULL;
		return fnd->getData();
	}

	//Create a user which does not exists
	os::smart_ptr<crypto::user> loginMetaData::createUser(std::string usr, std::string pass) throw (std::string)
	{
		os::smart_ptr<userNode> usNode=findUser(usr);
		if(usNode) throw std::string("Cannot re-create user");
		if(pass=="") throw std::string("Illegal password");

		//Create user node
		lock.lock();
		usNode=os::smart_ptr<userNode>(new userNode(usr),os::shared_type);
		usNode->userExists=true;
		usNode->timestamp=os::getTimestamp();
		users.insert(usNode);

		defaultUsername=usNode->username;
		defaultPassword=usNode->password;
		needsBinding=true;
		lock.unlock();

		markChanged();

		_currentUser=os::smart_ptr<crypto::user>(new crypto::user(usr,_savePath+"/"+USERS_FOLDER,(unsigned char*) pass.c_str(),pass.size()),os::shared_type);
		if(_currentUser->needsSaving())
			_currentUser->save();
		bindSavable(os::cast<os::savable,crypto::user>(_currentUser));
		return _currentUser;
	}
	//Open an existant user
	os::smart_ptr<crypto::user> loginMetaData::openUser(std::string usr, std::string pass) throw (std::string)
	{
		checkUserData();
		os::smart_ptr<userNode> usNode=findUser(usr);
		if(!usNode) throw std::string("Cannot find user");
		
		if(!usNode->userExists) throw std::string("User folder not found");

		lock.lock();
		usNode->timestamp=os::getTimestamp();

		defaultUsername=usNode->username;
		defaultPassword=usNode->password;
		needsBinding=true;
		lock.unlock();

		markChanged();
		
		_currentUser=os::smart_ptr<crypto::user>(new crypto::user(usr,_savePath+"/"+USERS_FOLDER,(unsigned char*) pass.c_str(),pass.size()),os::shared_type);
		if(_currentUser->numberErrors()>0)
			_currentUser=os::smart_ptr<crypto::user>(new crypto::user(usr,_savePath+"/"+USERS_FOLDER,(unsigned char*) pass.c_str(),pass.size()),os::shared_type);
		bindSavable(os::cast<os::savable,crypto::user>(_currentUser));
		return _currentUser;
	}
}

#endif

///@endcond