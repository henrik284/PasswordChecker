#ifndef CLIENT_H_INCLUDED
#define CLIENT_H_INCLUDED

#include "SIMPLESOCKET.H"
#include "SHA256.H"



using namespace std;


/**
*   \class pwdMsg
*   \brief pwdMsg class stores information of an message with an string id an zero to four string arguments
*/
class pwdMsg {
public:
    /**
    *   \brief constructor without an incoming message. Message can be read later with the readMsg method.
    */
    pwdMsg();

    /**
    *   \brief constructor with an incoming message. Message is stored in the members.
    *   \param recMsg Received message
    */
    pwdMsg(string recMsg);

    /**
    *   \brief Stores the id and the arguments in the members
    *   \param recMsg Received message
    */
    void readMsg(string recMsg);

    /**
    *   \brief Creates a Message out of the id and zero to four arguments.
    *           Static function: can be called without an object.
    */
    static string newMsg(string id, string arg1 = "", string arg2 = "", string arg3 = "", string arg4 = "");

    string id_ = "";
    string arg1_ = "";
    string arg2_ = "";
    string arg3_ = "";
    string arg4_ = "";
};




/**
*   \brief creates a Message out of a 6 character id and 0 to 4 arguments in the format: id(arg1;arg2;arg3;arg4)
*/
string writeMessage(string id, string arg1 = "", string arg2 = "", string arg3 = "", string arg4 = "");

/**
*   \brief Struct saving the messages id and all arguments.
*/
typedef struct{
    string id = "";
    string arg1 = "";
    string arg2 = "";
    string arg3 = "";
    string arg4 = "";
} msgStruct;

/**
*   \brief Seperates the message id and its arguments and saves them into a messagStruct
*/
msgStruct readMessage(string message);


/**
*   \class pwdCheckerClient
*   \brief The pwdCheckerClient class inherits from the TCPclient.
*/
class pwdCheckerClient : public TCPclient{
public:
    /**
    * \brief Constructor of the pwdCheckerClient class. Inherits from the TCPclient.
    */
    pwdCheckerClient() : TCPclient(){;};

    /**
    *   \brief recursive fuction to generate all different passwords with the given password length and available characters.
    *   \param *password Pointer to the Password.
    *   \param Chars number of available characters for the Password
    */
    void createPwdRec(string& password, int index, int chars, int& counterOfTrials, bool& passwordHacked);


};



#endif // CLIENT_H_INCLUDED
