//Primary author: Jonathan Bedard
//Confirmed working: 3/9/2016

#ifndef LIST_USERS_FORM_H
#define LIST_USERS_FORM_H

#include "glLibrary.h"
#include "loginMetaData.h"

namespace login{

	class listUsers;

	//User data frame
	class userFrame: public gl::frame, public gl::clickedListener, public gl::enterListener
	{
		os::smart_ptr<userNode> _user;
		listUsers* mst;

		gl::label lblUsername;
		gl::label lblTimestamp;
		gl::label lblPassword;
		
		gl::button btnRemove;
		gl::button btnLoad;
	public:
		userFrame(listUsers& master,os::smart_ptr<userNode> usr);
		virtual ~userFrame(){}

		void receivedClicked(os::smart_ptr<element> elm);
		void receivedEnter(os::smart_ptr<element> elm){receivedClicked(elm);}

		bool operator==(const userFrame& usr){return *_user==*(usr._user);}
		bool operator!=(const userFrame& usr){return *_user!=*(usr._user);}
		bool operator<(const userFrame& usr){return *_user<*(usr._user);}
		bool operator>(const userFrame& usr){return *_user>*(usr._user);}
		bool operator<=(const userFrame& usr){return *_user<=*(usr._user);}
		bool operator>=(const userFrame& usr){return *_user>=*(usr._user);}
	};

	//Login form
	class listUsers: public gl::navForm
	{
		friend class userFrame;

		bool needRefresh;
		gl::scrollArea scrArea;
		loginMetaData& metaData;
		os::smartSet<userFrame> frameDisplay;

		void refreshFromMeta();
	protected:
		void update();
	public:
		listUsers(gl::form* master,std::string baseTitle,loginMetaData* metaDataPointer);
		virtual ~listUsers(){}
	};

};

#endif