/*
 * TASK1.H
 *
 *  Created on: 10.09.2019
 *      Author: aml
 */

/**
 * \file TASK1.H
 * \brief Contains namespace TASK1. Namespace TASK1 contains the implementation
 * of check sum calculation.
 */

#ifndef TASK1_H_
#define TASK1_H_

#define MESSAGE_SIZE 64
#define PORT 2022

#include <string>
#include "SHA256.H"

using namespace std;

/**
 * \namespace TASK1
 * \brief Contains the implementation of check sum calculation.
 */
namespace TASK1{

/**
 * \var const string SYMBOLS
 * \brief This contains all possible characters a password can contain
 */
const string SYMBOLS = "ABCDEFGHIJKLMNOPQRTSTUVWXYZabcdefghijklmopqrstuvwxyz0123456789";

/**
 * \var const unsigned int MINIMAL_PWD_LENGTH
 * \brief Defines the minimal length of a password
 */
const unsigned int MINIMAL_PWD_LENGTH = 1;

/**
 * \class IBlackBox
 *	\brief Interface class for password check classes.
 */


class IBlackBox{
public:
	virtual string input(string strPwd) = 0;
	virtual string getSymbols() = 0;
};


/**
 * \class BlackBoxUnsafe
 * \brief Implements a unsafe password checker where the
 *        password is stored as plan text.
 */
class BlackBoxUnsafe : public IBlackBox{
public:
    /**
    * \brief Constructor:
             Tests if the symbSetSize is valid (>0) and not bigger than the available number of symbols. If not, the default amount of characters is used.
             Copies the wanted amount of symbols out of the default symbols string into a new symbols string created on the heap and pointed to by charSymbArray.
             Calls the randomPwd methods and saves this password in the pwd_ member.
    * \param pwdLength Length of the password, which will be created with the randomPwd method and saved in the pwd_ member.
    * \param symbSetSize Amount of available characters for the password.
    */
    BlackBoxUnsafe(int pwdLength, int symbSetSize);

    /**
    * \brief Compares the password, which is given as argument, with the one saved in the member pwd_.
    * \param strPwd Password, which should be tested.
    * \return Returns a string containing "ACCESS ACCEPTED" if password is correct or "ACCESS DENIED" if not.
    */
	virtual string input(string strPwd);

    /**
    * \brief returns a string with all possible symbols for the passwords.
    */
    string getSymbols();

    /**
    * \brief string saving the password as plan text
    */
    string pwd_;

protected:
	/**
	* \brief Creates a password of the length pwdLength with random characters out of the charSymbArray.
	* \param pwdLength Length of the password, which will be created.
	* \return Returns a random password
	*/
	string randomPwd(int pwdLength);

	/**
	* \brief Pointer to the Array which contains the available characters for the password. Initialized in the Constructor. Saved on the heap.
	*/
	char * charSymbArray_;

	/**
	* \brief Number of characters available for the password.
	*/
	int    lengthSymbArray_;
};

/**
 * \class BlackBoxSafe
 * \brief Implements a more safe password checker where only the
 *        password's checksum is stored.
 */
class BlackBoxSafe : public BlackBoxUnsafe{
public:
	/**
	*  \brief Constructor of BalckBoxSafe Class. Calls the BlackBoxUnsafe Constructor and directly overwrites the plan text password stored in the pwd_ member
	*         by the BalckBoxUnsafe Constructor with the Checksum calculated by the sha256 algorithm.
	*  \param pwdLength Length of the password, which will be created by the random pwd method and stored as Checksum in the pwd_ member
	*  \param symbSetSize Amount of available characters for the password
	*/
	BlackBoxSafe(int pwdLength, int symbSetSize);

	/**
	*  \brief Compares the password checksum, which is given as argument, with the one saved in the member pwd_.
    *  \param strPwd Password, which should be tested.
    *  \return Returns a string containing "ACCESS ACCEPTED" if password is correct or "ACCESS DENIED" if not.
    */
	virtual string input(string strPwd);
};


/**
 * \brief Simple demonstration of calculating
 * the check sum for a given string.
 */
void demoTASK1_00();

/**
 * \brief Simple demonstration of using the password checker.
 */
void demoTASK1_01();

/**
* \brief Another simple demonstration of using the password checker.
*/
void demoTASK1_02();

} // end namespace TASK1

/**
*   \class pwdMsg
*   \brief pwdMsg class stores information of an message with an string id an zero to four string arguments.
*           It also provides methods to create a message out of an id and some aguments and a method to read the id and the arguments out of the message and store them in the member variables.
*/
class pwdMsg {
public:
    /**
    *   \brief constructor Creates an object of this class and initializes the id and arguments with empty strings.
    */
    pwdMsg();

    /**
    *   \brief Parameterized constructor Creates an object and fills it with the information of an incoming message. The id and the arguments of the incoming message are getting stored in the members.
    *   \param recMsg Received message
    */
    pwdMsg(string recMsg);

    /**
    *   \brief Stores the id and the arguments in the members of an existing object
    *   \param recMsg Received message
    */
    void readMsg(string recMsg);

    /**
    *   \brief Creates a Message out of the id and zero to four arguments.
    *           Static function: can be called without an object.
    *   \return Returns the complete message as string.
    */
    static string newMsg(string id, string arg1 = "", string arg2 = "", string arg3 = "", string arg4 = "");

    string id_ = "";
    string arg1_ = "";
    string arg2_ = "";
    string arg3_ = "";
    string arg4_ = "";
};



#endif /* TASK1_H_ */
