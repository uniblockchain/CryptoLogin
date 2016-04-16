/**
 * @file	loginMain.h
 * @author	Jonathan Bedard
 * @date   	4/13/2016
 * @brief	Entry login form
 * @bug	None
 *
 * Provides a template class for
 * applications which would like to
 * include a password to log in.  Will
 * allow the user to create new users
 * and define public keys for those
 * users as well.
 **/

#ifndef LOGIN_MAIN_H
#define LOGIN_MAIN_H

#include "glLibrary.h"
#include "loginMetaData.h"
#include "createUserForm.h"
#include "listUsersForm.h"
#include "keyGenerationPopUp.h"

namespace login{

	/** @brief Login form
	 *
	 * A form which places the next form
	 * behind a login security wall.  Defined
	 * as a template class so it can open
	 * any form after a successful login.
	 */
	template<class nextForm>
	class mainLogin: public gl::form, public gl::clickedListener, public gl::enterListener
	{
		/** @brief Login meta-data
		 */
		loginMetaData _metaData;
		/** @brief Logged-in state
		 */
		bool loggedIn;

		/** @brief Title label
		 */
		gl::label lblTitle;

		/** @brief User textbox label
		 */
		gl::label lblUser;
		/** @brief User textbox
		 */
		gl::textbox tbxUser;
		/** @brief Password textbox label
		 */
		gl::label lblPassword;
		/** @brief Password textbox
		 */
		gl::textbox tbxPassword;

		/** @brief Login button
		 */
		gl::button btnLogin;

		/** @brief New user button
		 */
		gl::button btnNewUser;
		/** @brief List users button
		 */
		gl::button btnListUsers;
		/** @brief Exit button
		 */
		gl::button btnExit;
	protected:
		/** @brief Triggered every cycle
		 *
		 * Saves the meta-data if needed, binds
		 * the meta-data to the form if the flag
		 * has been tripped and opens the next
		 * form is valid.
		 *
		 * @return void
		 */
		void update()
		{
			//Save meta-data
			if(_metaData.needsSaving())
				_metaData.pushDistributor(os::savable::getThread());

			//Bind meta-data
			if(_metaData.needsBinding)
			{
				tbxUser.setText(_metaData.defaultUsername);
				tbxPassword.setText(_metaData.defaultPassword);
				if(_metaData.defaultUsername=="")
					tbxUser.focus();
				else if(_metaData.defaultPassword=="")
					tbxPassword.focus();
				else
					btnLogin.focus();
				_metaData.needsBinding=false;
			}

			//Open
			if(_metaData.currentUser())
			{
				if(_metaData.currentUser()->getDefaultPublicKey() &&
				_metaData.currentUser()->getDefaultPublicKey()->generating())
				{
					open(os::smart_ptr<gl::form>(new pulblicKeyPopUp(this,_metaData.currentUser()),os::shared_type));
					return;
				}
				else if(loggedIn)
				{
					_metaData.unbindUser();
					loggedIn=false;
				}
				else openNextForm();
			}
		}
	public:
		mainLogin(int* argc, char** argv, std::string title,loginMetaData metaData):
			form(argc,argv,500,400),_metaData(metaData),
			lblTitle(this),lblUser(this),lblPassword(this),
			tbxUser(this),tbxPassword(this),
			btnLogin(this),btnNewUser(this),btnListUsers(this),btnExit(this)
		{
			//Check the savable thread distributors
			if(!os::savable::getThread())
				os::savable::setThread(os::smart_ptr<os::threadDistributor>(new os::threadDistributor(1),os::shared_type));

			loggedIn=false;
			setTitle(title+": Login");
			setResizePolicy(gl::resize_lock);

			//Title
			lblTitle.setFont(os::smart_ptr<gl::font>(new gl::TimesRoman(),os::shared_type));
			lblTitle.setFontSize(24);
			lblTitle.setLateralLayout(gl::layout_center);
			lblTitle.setText(title);
			lblTitle.setWidth(width());
			lblTitle.setX(0);
			lblTitle.setY(height()-40);

			//User
			lblUser.setText("Username:");
			lblUser.setX(170);
			lblUser.setY(310);
			tbxUser.focus();
			tbxUser.setX(150);
			tbxUser.setY(250);
			tbxUser.setWidth(200);
			addMouseListener(&tbxUser);
			addKeyboardListener(&tbxUser);
			tbxUser.pushEnterListener(this);

			//Password
			lblPassword.setText("Password:");
			lblPassword.setX(170);
			lblPassword.setY(230);
			tbxPassword.setTextHidden(true);
			tbxPassword.setX(150);
			tbxPassword.setY(170);
			tbxPassword.setWidth(200);
			addMouseListener(&tbxPassword);
			addKeyboardListener(&tbxPassword);
			tbxPassword.pushEnterListener(this);

			//Login
			btnLogin.setText("Login");
			btnLogin.setX(175);
			btnLogin.setY(100);
			addMouseListener(&btnLogin);
			addKeyboardListener(&btnLogin);
			btnLogin.pushClickedListener(this);
			btnLogin.pushEnterListener(this);

			//New User
			btnNewUser.setText("New User");
			btnNewUser.setX(10);
			btnNewUser.setY(10);
			addMouseListener(&btnNewUser);
			addKeyboardListener(&btnNewUser);
			btnNewUser.pushClickedListener(this);
			btnNewUser.pushEnterListener(this);

			//List users
			btnListUsers.setText("List Users");
			btnListUsers.setX((width()-btnListUsers.width())/2);
			btnListUsers.setY(10);
			addMouseListener(&btnListUsers);
			addKeyboardListener(&btnListUsers);
			btnListUsers.pushClickedListener(this);
			btnListUsers.pushEnterListener(this);

			//Exit
			btnExit.setText("Exit");
			btnExit.setX(340);
			btnExit.setY(10);
			addMouseListener(&btnExit);
			addKeyboardListener(&btnExit);
			btnExit.pushClickedListener(this);
			btnExit.pushEnterListener(this);

			//Load meta data (should be fast!)
			_metaData.load();
		}
		virtual ~mainLogin(){}

		//Open next form
		void openNextForm()
		{
			if(_metaData.currentUser())
			{
				loggedIn=true;
				open(os::smart_ptr<gl::form>(new nextForm(this,&_metaData),os::shared_type));
			}
			else open(os::smart_ptr<gl::form>(new gl::singleButtonPopUp(this,"Cannot login, no user!"),os::shared_type));

		}

		//Received events
		void receivedClicked(os::smart_ptr<element> elm)
		{
			//Login
			if(elm==&btnLogin)
			{
				try
				{
					os::smart_ptr<crypto::user> usr=_metaData.openUser(tbxUser.trueText(),tbxPassword.trueText());
					if(!usr) throw std::string("User could not be opened");
					if(usr->numberErrors()>0) 
					{
						auto terr= usr->popError();
						if(terr->errorTitle()=="Hash Compare")
							throw std::string("Password incorrect");
						throw terr->errorTitle();
					}
				}
				catch(std::string str)
				{
					open(os::smart_ptr<gl::form>(new gl::singleButtonPopUp(this,str),os::shared_type));
					_metaData.unbindUser();
					return;
				}
				catch(...)
				{
					open(os::smart_ptr<gl::form>(new gl::singleButtonPopUp(this,"Unknown error when logging on"),os::shared_type));
					_metaData.unbindUser();
					return;
				}

				if(_metaData.needsSaving())
				{
					_metaData.pushDistributor(os::savable::getThread());
					open(os::smart_ptr<gl::form>(new userLoadingPopUp(this,&_metaData),os::shared_type));
				}
				return;
			}

			//New user
			if(elm==&btnNewUser)
			{
				//Open new user creation
				open(os::smart_ptr<gl::form>(new createUser(this,lblTitle.text(),&_metaData),os::shared_type));
				return;
			}

			//List users
			if(elm==&btnListUsers)
			{
				open(os::smart_ptr<gl::form>(new listUsers(this,lblTitle.text(),&_metaData),os::shared_type));
				return;
			}

			//Exit button
			if(elm==&btnExit)
			{
				glutLeaveMainLoop();
				return;
			}
		}
		void receivedEnter(os::smart_ptr<element> elm)
		{
			//Allow a triple-enter login
			if(elm==&tbxUser)
			{
				tbxPassword.focus();
				return;
			}
			if(elm==&tbxPassword)
			{
				btnLogin.focus();
				return;
			}
			receivedClicked(elm);
		}
	};

};

#endif