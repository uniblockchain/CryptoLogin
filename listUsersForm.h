/**
 * @file	listUsers.h
 * @author	Jonathan Bedard
 * @date   	8/28/2016
 * @brief	User-list header
 * @bug	None
 *
 * Defines a form used to list users associated with
 * this particular application.
 **/

#ifndef LIST_USERS_FORM_H
#define LIST_USERS_FORM_H

#include "glGraphics/glLibrary.h"
#include "loginMetaData.h"

namespace login{

	///@cond INTERNAL
	class listUsers;
	///@endcond

	/** @brief Defines a user display frame
	 * This frame displays basic user data
	 * before a user is logged in.  This data
	 * is defined from the login meta-data.
	 */
	class userFrame: public gl::frame, public gl::clickedListener, public gl::enterListener
	{
		/** @brief User-node to be displayed
		 */
		os::smart_ptr<userNode> _user;
		/** @brief Master-form
		 */
		listUsers* mst;

		/** @brief Label displaying user-name
		 */
		gl::label lblUsername;
		/** @brief Label displaying time-stamp
		 */
		gl::label lblTimestamp;
		/** @brief Label displaying password
		 */
		gl::label lblPassword;
		
		/** @brief Remove user button
		 */
		gl::button btnRemove;
		/** @brief Load user button
		 */
		gl::button btnLoad;
	public:
		/** @brief Constructor
		 *
		 * @param [in] master Master form
		 * @param [in] usr User node to be displayed
		 */
		userFrame(listUsers& master,os::smart_ptr<userNode> usr);
		/** @brief Virtual destructor
         *
         * Destructor must be virtual, if an object
         * of this type is deleted, the destructor
         * of the type which inherits this class should
         * be called.
         */
		virtual ~userFrame(){}

		/** @brief Trigger on button click
		 * @param [in] elm Element clicked
		 * @return void
		 */
		void receivedClicked(os::smart_ptr<element> elm);
		/** @brief Trigger on enter event
		 *
		 * Note that in this element, there is no difference
		 * between an enter event and a clicked event.
		 *
		 * @param [in] elm Element receiving enter
		 * @return void
		 */
		void receivedEnter(os::smart_ptr<element> elm){receivedClicked(elm);}

        #undef CURRENT_CLASS
        #define CURRENT_CLASS userFrame
        /** @brief Compare two user frames
          * @param [in] usr User frame to compare
          * @return Integer comparison of users
          */
        int compare(const userFrame& usr) const{return _user->compare(*usr._user);}
        /** @brief Cast user to size_t
          * @return Hashed size_t from the user
          */
        operator size_t() const {return (size_t) *_user;}
        COMPARE_OPERATORS
	};

	/** @brief List-user form
	 * A navigation form listing all
	 * users associated with a particular
	 * loginMetaData class.
	 */
	class listUsers: public gl::navForm
	{
		/** @brief Friendship with user-frame
		 * Since the user-frame defines button-press
		 * events, the user-frame must be able to
		 * modify the intrinsics of this form+.
		 */
		friend class userFrame;

		/** @brief Indicates if meta-data needs to be refreshed
		 */
		bool needRefresh;
		/** @brief Area holding user frames
		 */
		gl::scrollArea scrArea;
		/** @brief Meta-data defining users
		 */
		loginMetaData& metaData;
		/** @brief User frames to be displayed
		 */
		os::pointerSet<userFrame> frameDisplay;

		/** @brief Rebuild form from meta-data
		 * @return void
		 */
		void refreshFromMeta();
	protected:
		/** @brief Triggered each update cycle
		 * @return void
		 */
		void update();
	public:
		/** @brief Form constructor
		 * @param [in] master Previos form
		 * @param [in] baseTitle Title of base application
		 * @param [in] metaDataPointer Meta-data for user definition
		 */
		listUsers(gl::form* master,std::string baseTitle,loginMetaData& metaDataPointer);
		/** @brief Virtual destructor
         *
         * Destructor must be virtual, if an object
         * of this type is deleted, the destructor
         * of the type which inherits this class should
         * be called.
         */
		virtual ~listUsers() throw(){}
	};

};

#endif