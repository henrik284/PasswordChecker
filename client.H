#ifndef CLIENT_H_INCLUDED
#define CLIENT_H_INCLUDED

#include "SIMPLESOCKET.H"
#include "SHA256.H"



using namespace std;

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
