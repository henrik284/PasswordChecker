#ifndef CLIENT_H_INCLUDED
#define CLIENT_H_INCLUDED

#include "SIMPLESOCKET.H"
#include "SHA256.H"



using namespace std;

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
    void createPwdRec(string& password, int index, int Chars, int& CounterOfTrials, bool& passwordHacked);


};



#endif // CLIENT_H_INCLUDED
