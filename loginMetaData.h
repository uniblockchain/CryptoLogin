/**
 * @file	loginMetaData.h
 * @author	Jonathan Bedard
 * @date   	7/4/2016
 * @brief	Contains meta-data for login form
 * @bug	None
 *
 * Provides meta-data to the login form.
 * This class has default states, so if a
 * custom meta-data class is not passed to
 * the login form, a default one will be
 * created.
 **/

#ifndef LOGIN_META_DATA_H
#define LOGIN_META_DATA_H

#include "CryptoGateway/CryptoGateway.h"
#include <string>

namespace login{
	/** @brief Meta-data file name
	 */
	extern const char* META_FILE;
	/** @brief Folder holding user data
	 */
	extern const char* USERS_FOLDER;

	/** @brief User node
	 *
	 * Used in the meta-data file.
	 * Stores all user-names and, if
	 * applicable, passwords associated
	 * with the user-names.
	 */
	struct userNode
	{
		/** @brief Construct user node
		 *
		 * Note that the existance of a user-node
		 * does not gurantee the existance of a user.
		 * By default, it is assumed that such a user
		 * does not exists.
		 *
		 * @param [in] usr User-name
		 */
		userNode(std::string usr)
		{
			username=usr;
			password="";
			userExists=false;
		}
		/** @brief Username string
		 */
		std::string username;
		/** @brief Password string
		 *
		 * If this is defined, then the password
		 * for the user in question is being stored
		 * in plain-text.  This is insecure, but
		 * many use cases don't demand the security.
		 */
		std::string password;
		/** @brief User exists flag
		 *
		 * Set based on weather the user was
		 * found on disk.
		 */
		bool userExists;
		/** @brief Last time user accessed
		 */
		uint64_t timestamp;

        #undef CURRENT_CLASS
        #define CURRENT_CLASS userNode
        /** @brief Compare two user nodes
         * @param [in] usr User node to compare
         * @return Integer comparison of users
         */
        int compare(const userNode& usr) const{return username.compare(usr.username);}
        /** @brief Cast user to size_t
         * @return Hashed size_t from the user
         */
        operator size_t() const {return os::hashData(username.c_str(),username.length());}
        COMPARE_OPERATORS
	};

	/** @brief Login meta-data class
	 *
	 * Contains supporting data for
	 * login interfaces.
	 */
	class loginMetaData: public os::savingGroup
	{
		/** @brief Path to save directory
		 */
		std::string _savePath;
		/** @brief Pointer to the current user
		 */
		os::smart_ptr<crypto::user> _currentUser;

		/** @brief Generate a new save tree
		 * @return XML tree to be saved
		 */
		os::smartXMLNode generateSaveTree();
	public:
		/** @brief List of current users
		 * Stores all users in a list to
		 * be logged into.
		 */
		os::pointerAVLTreeThreadSafe<userNode> users;
		/** @brief Defaut username
		 */
		std::string defaultUsername;
		/** @brief Default password
		 */
		std::string defaultPassword;
		/** @brief Data changed flag
		 * This flag is used by the UI to refresh
		 * when the loginMetaData changes.
		 */
		bool needsBinding;
		/** @brief Read/write lock
		 *
		 * Allows for both reading and writing
		 * to occur asynchronously
		 */
		os::readWriteLock lock;


		/** @brief Meta-data constructor
		 *
		 * @param [in] svPath Save directory path
		 */
		loginMetaData(std::string svPath);
		/** @brief Meta-data copy constructor
		 *
		 * @param [in] cpy Meta-data to copy
		 */
		loginMetaData(loginMetaData& cpy);
		/** @brief Virtual destructor
         *
         * Destructor must be virtual, if an object
         * of this type is deleted, the destructor
         * of the type which inherits this class should
         * be called.
         */
		virtual ~loginMetaData() throw() {}

		/** @brief Loads meta-data from XML file
		 *
		 * Parses the XML tree and scans folders
		 * to populate the list of available users.
		 * @return void
		 */
		void load();
		/** @brief Saves meta-data file
		 * @return void
		 */
		void save();

		/** @brief Check user directories
		 *
		 * Iterates through all folders in the
		 * user directory to see which users are
		 * actually availalbe.
		 *
		 * @return void
		 */
		void checkUserData();
		
		/** @brief Find user by name
		 * @param [in] usr Name of user to be searched
		 * @return User with the given name
		 */
		os::smart_ptr<userNode> findUser(std::string usr);
		/** @brief Create new user
		 * @param [in] usr Target username
		 * @param [in] pass Target password
		 * @return Newly created user
		 */
		os::smart_ptr<crypto::user> createUser(std::string usr, std::string pass) throw (std::string);
		/** @brief Logs a new user in
		 * @param [in] usr Target username
		 * @param [in] pass Target password
		 * @return Opened user
		 */
		os::smart_ptr<crypto::user> openUser(std::string usr, std::string pass) throw (std::string);
		/** @brief Sets the current user to NULL
		 * @return void
		 */
		void unbindUser() {_currentUser=NULL;}

		/** @brief Returns the save path
		 * @return login::loginMetaData::_savePath
		 */
		const std::string& savePath() const {return _savePath;}
		/** @brief Returns the current user
		 * @return login::loginMetaData::_currentUser
		 */
		os::smart_ptr<crypto::user> currentUser() {return _currentUser;}
	};
}

#endif