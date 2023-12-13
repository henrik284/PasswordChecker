#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED

#include "TASK1.H"

using namespace std;

/**
*   \class pwdCheckerServer
*   \brief The pwdCheckerServer class generates the password and checks, if the password given by the client is correct
*/
class pwdCheckerServer: public TCPserver{
 public:
    /**
    *   \brief Constructor: Calls the Constructor of the parent TCPserver class and creates an new BlackBoxSafe object on the heap and saves the adress in the ptrPwdBox pointer
    */
    pwdCheckerServer(int port, int msgSize) : TCPserver(port,msgSize){
        ptrPwdBox_ = NULL;
    };

 protected:

    /**
    *   \brief This class checks the password.
    *   \param  Message sent by the client
    *   \return Answer to the message sent by the client
    */
    string myResponse(string input);

    /**
    *   \brief ptr to a BlackBoxSafe Object. This is creates a random password and is used to check if the password given by the client is correct.
    */
    TASK1::BlackBoxSafe* ptrPwdBox_;
};

#endif // SERVER_H_INCLUDED
