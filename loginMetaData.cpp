/**
 * @file	loginMetaData.h
 * @author	Jonathan Bedard
 * @date   	9/10/2016
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
#include "osMechanics/osMechanics.h"
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
		_currentUser=cpy._currentUser;

		_savePath=cpy._savePath;
		users.insertStructure(cpy.users);
		os::testCreateFolder(_savePath);
		markChanged();

		needsBinding=true;
		cpy.lock.decrement();
	}
	
	//Generate save tree
	os::smart_ptr<os::XMLNode> loginMetaData::generateSaveTree()
	{
		lock.increment();
		os::smart_ptr<os::XMLNode> head=os::smart_ptr<os::XMLNode>(new os::XMLNode("loginData"),os::shared_type);

		//Saved meta-data
		os::smart_ptr<os::XMLNode> lv1=os::smart_ptr<os::XMLNode>(new os::XMLNode("saved"),os::shared_type);
		os::smart_ptr<os::XMLNode> lv2=os::smart_ptr<os::XMLNode>(new os::XMLNode("username"),os::shared_type);
		lv2->setData(defaultUsername);
		lv1->addChild(*lv2);

		lv2=os::smart_ptr<os::XMLNode>(new os::XMLNode("password"),os::shared_type);
		lv2->setData(defaultPassword);
		lv1->addChild(*lv2);
		head->addChild(*lv1);

		//User list
		lv1=os::smart_ptr<os::XMLNode>(new os::XMLNode("users"),os::shared_type);
		for(auto it=users.first();it;++it)
		{
			lv2=os::smart_ptr<os::XMLNode>(new os::XMLNode("user"),os::shared_type);
			os::smart_ptr<os::XMLNode> lv3=os::smart_ptr<os::XMLNode>(new os::XMLNode("name"),os::shared_type);
			lv3->setData(it->username);
			lv2->addChild(*lv3);

			lv3=os::smart_ptr<os::XMLNode>(new os::XMLNode("password"),os::shared_type);
			lv3->setData(it->password);
			lv2->addChild(*lv3);

			lv3=os::smart_ptr<os::XMLNode>(new os::XMLNode("timestamp"),os::shared_type);
			lv3->setData(std::to_string((long long unsigned int)it->timestamp));
			lv2->addChild(*lv3);

			lv1->addChild(*lv2);
		}
		head->addChild(*lv1);
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
		for(unsigned int i=0;i<arrayLen;++i)
			arr[i]=os::extract_name(arr[i]);

		//Next, add all read directories to list
		for(unsigned int i=0;i<arrayLen;++i)
		{
			os::smart_ptr<userNode> usNode(new userNode(arr[i]),os::shared_type);
			usNode->userExists=true;
			users.insert(usNode);
		}

		//Check all declared users, mark if the directory exists
		for(auto trc=users.first();trc;++trc)
		{
			trc->userExists=false;
			for(unsigned int i=0;i<arrayLen && !trc->userExists;++i)
			{
				if(arr[i]==trc->username)
					trc->userExists=true;
			}
		}
		lock.unlock();
	}
	//Load meta-data
	void loginMetaData::load()
	{
		lock.lock();
        os::smart_ptr<os::XMLNode> xmNode(new os::XMLNode(os::XMLNode::read(_savePath+"/"+META_FILE)), os::shared_type);
		
		//Parse read tree
		try
		{
			//Jump out
			if(!xmNode) throw -1;

			//Find default username and password
			auto searchList=xmNode->searchList("saved");
			if(searchList.size()!=1) throw -1;
			os::smart_ptr<os::XMLNode> saveNode=&searchList.first();

			searchList=saveNode->searchList("username");
			if(searchList.size()!=1) throw -1;
			defaultUsername=searchList.first()->data();

			searchList=saveNode->searchList("password");
			if(searchList.size()!=1) throw -1;
			defaultPassword=searchList.first()->data();

			//Find all users
			searchList=xmNode->searchList("users");
			if(!searchList) throw -1;
			if(searchList.size()!=1) throw -1;
			os::smart_ptr<os::XMLNode> usrList=&searchList.first();
			searchList=usrList->searchList("user");
			if(!searchList) throw -1;

			//Iterate through user list
			for(auto it=searchList.first();it;++it)
			{
				try
				{
					//Find name
					auto usrList=it->searchList("name");
					if(!usrList) throw -1;
					if(usrList.size()!=1) throw -1;
					os::smart_ptr<userNode> insNode(new userNode(usrList.first()->data()));
                    auto temp=users.search(insNode);
					if(temp)
						insNode=&temp;

					//Search password
					usrList=it->searchList("password");
					if(!usrList) throw -1;
					if(usrList.size()!=1) throw -1;
					insNode->password=usrList.first()->data();

					//Search timestamp
					usrList=it->searchList("timestamp");
					if(!usrList) throw -1;
					if(usrList.size()!=1) throw -1;
					std::stringstream(usrList.first()->data())>>insNode->timestamp;

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
        os::XMLNode::write(_savePath+"/"+META_FILE,*generateSaveTree());
		savingGroup::save();
		finishedSaving();
		lock.decrement();
	}

//User Access----------------------------------------------------------------

	//Find user
	os::smart_ptr<userNode> loginMetaData::findUser(std::string usr)
	{

		auto fnd= users.search(os::smart_ptr<userNode>(new userNode(usr),os::shared_type));
		if(!fnd) return NULL;
		return &fnd;
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