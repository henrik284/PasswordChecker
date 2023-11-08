#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED

#include "TASK1.H"

using namespace std;

/**
*   \class pwdChecker
*   \brief The pwdChecker class generates the password and checks, if the password given by the client is correct
*/
class pwdChecker: public TCPserver{
 public:
    /**
    *   \brief Constructor: Calls the Constructor of the parent TCPserver class and creates an new BlackBoxSafe object on the heap and saves the adress in the ptrPwdBox pointer
    */
    pwdChecker(int port, int msgSize) : TCPserver(port,msgSize){
        ptrPwdBox_ = new TASK1::BlackBoxSafe(10,20);
    };

 protected:

    /**
    *   \brief This class checks the password.
    *   \param  input password string given by the client
    *   \return returns "ACCES ALLOWED" if password correct or "ACCESS DENIED" if password is not correct.
    */
    string myResponse(string input);

    /**
    *   \brief ptr to a BlackBoxSafe Object. This is creates a random password and is used to check if the password given by the client is correct.
    */
    TASK1::BlackBoxSafe* ptrPwdBox_;
};

#endif // SERVER_H_INCLUDED
