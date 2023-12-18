#include <cstdio> // standard input and output library
#include <cstdlib> // this includes functions regarding memory allocation
#include <cstring> // contains string functions
#include <cerrno> //It defines macros for reporting and retrieving error conditions through error codes
#include <ctime> //contains various functions for manipulating date and time
#include <string>
#include <sstream>
#include <iostream>


#include <unistd.h> //contains various constants
#include <sys/types.h> //contains a number of basic derived types that should be used whenever appropriate
#include <arpa/inet.h> // defines in_addr structure
#include <sys/socket.h> // for socket creation
#include <netinet/in.h> //contains constants and structures needed for internet domain addresses


#include "SIMPLESOCKET.H"
#include "server.H"
#include "client.H"
#include "TASK1.H"




using namespace std;

int main(){
	srand(time(nullptr));
	pwdCheckerServer srv(PORT,MESSAGE_SIZE);
	srv.run();
}

string pwdCheckerServer::myResponse(string input){
    pwdMsg message; //erstellt Objekt der Klasse pwdMsg mit leeren Strings
    message.readMsg(input); //Nachricht vom Server wird in id_ und arg_ aufgeteilt

    int arg[4];
    //Umwandlung der Argumente von string zu int
    istringstream(message.arg1_) >> arg[0];
    istringstream(message.arg2_) >> arg[1];
    istringstream(message.arg3_) >> arg[2];
    istringstream(message.arg4_) >> arg[3];

    if (message.id_ == "NewPassword"){
        if (ptrPwdBox_ != NULL){
            delete ptrPwdBox_;
        }
        ptrPwdBox_ = new TASK1::BlackBoxSafe(arg[0], arg[1]);
        return "ServerStatus(Password Created)";
    }
    else if (message.id_ == "CheckPassword"){
        cout << "message arg1: " << message.arg1_ << endl;

        message.arg1_ = sha256(message.arg1_);
        int comp = message.arg1_.compare(ptrPwdBox_->pwd_);

        cout << "PwdBox_: " << ptrPwdBox_->pwd_ << endl;
        cout << "message: " << message.arg1_ << endl;

        if(comp == 0){
            return "Access(Allowed)";
        }
        else{
            return "Access(Denied)";
        }

    }
}
