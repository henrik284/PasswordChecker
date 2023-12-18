#ifndef CLIENT_H_INCLUDED
#define CLIENT_H_INCLUDED

#include "SIMPLESOCKET.H"
#include "SHA256.H"



using namespace std;

/**
*   \class pwdCheckerClient
*   \brief The pwdCheckerClient class inherits from the TCPclient and specifies the client for the Password Checking Programm.
*/
class pwdCheckerClient : public TCPclient{
public:
    /**
    * \brief Constructor of the pwdCheckerClient class. Inherits from the TCPclient class. (No additional commands)
    */
    pwdCheckerClient() : TCPclient(){;};

    /**
    *   \brief Recursive fuction to generate all different passwords with the given password length and available characters.
    *   \param password Reference to the Password.
    *   \param index Index of the character wich is manipulated in this function call.
    *   \param chars Number of available characters.
    *   \param counterOfTrials Reference to a variable wich counts the Trials of guessing the Password.
    *   \param passwordHacked Reference to a variable wich will be set to true if the password is guess. This stopps the password guessing process.
    */
    void createPwdRec(string& password, int index, int chars, int& counterOfTrials, bool& passwordHacked, pwdCheckerClient* ptrClient);


};



#endif // CLIENT_H_INCLUDED
