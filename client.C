/*
 * client.C
 *
 *  Created on: 11.09.2019
 *      Author: aml
 */

#include <string>
#include <iostream>
#include <unistd.h> //contains various constants

#include "client.H"
#include <string>
#include <vector>
#include "TASK1.H"


#define PORT 2022
#define MAX_PWD_LENGHT 3
#define MAX_CHAR_COUNT 3
#define MESSAGE_SIZE 64

using namespace std;


int main(){
    srand(time(NULL));

    pwdCheckerClient client;
    string host = "localhost";
    string msg = {};

    int connectionTrys = 0;
    while(1){
        if(client.conn(host,PORT)==true){
            break;
        } else{

        //Has to be removed when server is implemented
        break;

            if(connectionTrys<10){
                sleep(1);
                connectionTrys++;
            } else {
                cout << "Connection Failed. Closing Client" << endl;
                return 0;
            }
        }
    }

    cout << "Connected to the Server" << endl << endl;

    //Loop over every Number of available characters
    for(int charsAvailable = 1; charsAvailable <= MAX_CHAR_COUNT; charsAvailable++){

        //Loop over all lenghts of passwords
        for(int pwdLenght = 1; pwdLenght <= MAX_PWD_LENGHT; pwdLenght++){

            //Loop over max 10 trys, in which the client trys to request a new password. Server has to answer with "ServerStatus(Password created)" to continue.
            for(int i = 0; i < 10; i++){

                //Send "Create-New-Password"-Command to the Server
                //client.sendData(pwdMsg::newMsg("NewPassword", to_string(pwdLenght), to_string(charsAvailable)));
                cout << "Sent: " << pwdMsg::newMsg("NewPassword", to_string(pwdLenght), to_string(charsAvailable)) << endl;


                //Wait for the server to confirm, that a new password was created
                //pwdMsg recmsg(client.receive(MESSAGE_SIZE);
                pwdMsg recmsg("ServerStatus(Password Created)");

                //If Sever has created the password, end this loop.
                if((recmsg.id_ == "ServerStatus") && (recmsg.arg1_ == "Password Created")){
                    break;
                //If not, try again after 1 second, if it isn't already the 10th try. Then stop programm
                } else if (i == 9){
                    cout << "Server can not create the password. Client stopped." << endl;
                    return 0;
                }
                sleep(1);
            }

            //Create the initial Password
            string password(pwdLenght, TASK1::SYMBOLS.at(0));

            //Variable to store the number of trys until the password is hacked.
            int counterOfTrials = 0;

            //Variable which is set to true if the password is hacked.
            bool passwordHacked = false;

            //Creating all possible Password with a recusive call of the createPwdRec function.
            client.createPwdRec(password, pwdLenght-1, charsAvailable, counterOfTrials, passwordHacked);

            cout << "Counter of Trials: " << counterOfTrials << "\tFinally hacked? " << passwordHacked << endl << endl;


        }
    }

}

void pwdCheckerClient::createPwdRec(string& password, int index, int chars, int& counterOfTrials, bool& passwordHacked){
    //Loop for replace the character on the current index with every available character
    for(int i = 0; i < chars; i++){
        password.at(index) = TASK1::SYMBOLS.at(i);

        if(index != 0){
            //If the current index is not the first index (0) of the password, call this function again and manipulate the character on the index one lower
            pwdCheckerClient::createPwdRec(password, index-1, chars, counterOfTrials, passwordHacked);

            //If the password was hacked in the function call before, end the recusive function calls of guessing the password.
            if(passwordHacked==true){
                return;
            }
        } else{
            //If the current index is the first index, the current password is one of the passwords which should be sended to the server. Increase the counter of Trial with 1.
            counterOfTrials++;

            //client.sendData(pwdMsg::newMsg("CheckPassword", password));
            cout << "Sent: " << pwdMsg::newMsg("CheckPassword", password) << endl;

            //Receive an answer of the server.
            //pwdMsg recmsg(client.receive(MESSAGE_SIZE));

                //Simulation of Server answer.
                pwdMsg recmsg;
                int coincidence = rand()%10;
                if(coincidence == 1){
                    recmsg.readMsg("Access(Allowed)");
                    cout << "correct password: " << password << endl;
                } else{
                    recmsg.readMsg("Access(Denied)");
                }

            if((recmsg.id_ == "Access") && (recmsg.arg1_ == "Allowed")){
                //Set passwordHacked to true to stop previous recursive calls
                passwordHacked = true;
                //exit this Loop
                break;
            }
        }
    }
    return;
}

void pwdMsg::readMsg(string recMsg){
    //Check if received message has valid format.
    if((recMsg.find("(") == string::npos) || (recMsg.find(")") == string::npos)){
        id_ = "ERROR_";
        arg1_ = "Message does not contain \"(\" and \")\"";
        return;
    }

    //Save the id
    int found = recMsg.find("(");
    id_ = recMsg.substr(0,found);
    recMsg.erase(0,found+1);

    //Save argument 1
    found = recMsg.find(";");
    if(found == string::npos){          //If message has no or only one argument
        arg1_ = recMsg.substr(0, recMsg.size()-1);
        return;
    } else {                            //If message has more than on argument (seperated by ;)
        arg1_ = recMsg.substr(0,found);
        recMsg.erase(0,found+1);
    }

    //Save argument 2
    found = recMsg.find(";");
    if(found==string::npos){            //If message had only two arguments
        arg2_ = recMsg.substr(0, recMsg.size()-1);
        return;
    } else{                             //If message had more than two arguments
        arg2_ = recMsg.substr(0, found);
        recMsg.erase(0,found+1);
    }

    //Save argument 3 and 4
    found = recMsg.find(";");
    if(found==string::npos){            //If message had three arguments
        arg3_ = recMsg.substr(0, recMsg.size()-1);
    } else{                             //If message had four arguments
        arg3_ = recMsg.substr(0, found);
        arg4_ = recMsg.substr(found+1, recMsg.size()-found-1-1);
    }
    return;
}

pwdMsg::pwdMsg(string recMsg){
    id_ = "";
    arg1_ = "";
    arg2_ = "";
    arg3_ = "";
    arg4_ = "";

    readMsg(recMsg);

    return;
}

pwdMsg::pwdMsg(){
    id_ = "";
    arg1_ = "";
    arg2_ = "";
    arg3_ = "";
    arg4_ = "";
    return;
}

string pwdMsg::newMsg(string id, string arg1, string arg2, string arg3, string arg4){
    string newMessage = {};
    newMessage += id;
    newMessage += "(";
    newMessage += arg1;
    if(arg2 != ""){
        newMessage += ";";
        newMessage += arg2;
        if(arg3 != ""){
            newMessage += ";";
            newMessage += arg3;
            if(arg4 != ""){
                newMessage += ";";
                newMessage += arg4;
            }
        }
    }
    newMessage += ")";

    if(newMessage.size()>MESSAGE_SIZE){
        return string("ERROR_(Message to long)");
    }

    return newMessage;
}

string writeMessage(string id, string arg1, string arg2, string arg3, string arg4){
    string newMessage = {};
    newMessage += id;
    newMessage += "(";
    newMessage += arg1;
    if(arg2 != ""){
        newMessage += ";";
        newMessage += arg2;
        if(arg3 != ""){
            newMessage += ";";
            newMessage += arg3;
            if(arg4 != ""){
                newMessage += ";";
                newMessage += arg4;
            }
        }
    }
    newMessage += ")";

    if(newMessage.size()>MESSAGE_SIZE){
        return string("ERROR_(Message to long)");
    }

return newMessage;
}



msgStruct readMessage(string message){
    msgStruct msgstruct;
    if((message.find("(") == string::npos) || (message.find(")") == string::npos)){
        msgstruct.id = "ERROR_";
        msgstruct.arg1 = "Message does not contain ( and )";
        return msgstruct;
    }
    int found = message.find("(");
    msgstruct.id = message.substr(0,found);
    message.erase(0,found+1);

    found = message.find(";");
    if(found==string::npos){
        msgstruct.arg1 = message.substr(0, message.size()-1);
        return msgstruct;
    } else {
        msgstruct.arg1 = message.substr(0,found);
        message.erase(0,found+1);
    }

    found = message.find(";");
    if(found==string::npos){
        msgstruct.arg2 = message.substr(0, message.size()-1);
        return msgstruct;
    } else{
        msgstruct.arg2 = message.substr(0, found);
        message.erase(0,found+1);
    }

    found = message.find(";");
    if(found==string::npos){
        msgstruct.arg3 = message.substr(0, message.size()-1);
    } else{
        msgstruct.arg3 = message.substr(0, found);
        msgstruct.arg4 = message.substr(found+1, message.size()-found-1-1);
    }
    return msgstruct;
}




/* First Demonstration
int main() {
	srand(time(NULL));
	TCPclient c;
	string host = "localhost";
	string msg;

	//connect to host
	c.conn(host , 2022);

    while(1){
        cout << "Please enter Password: " << endl;
        string pwdInput;
        cin >> pwdInput;

        //Send the Checksum of the Password to the Server
        pwdInput = sha256(pwdInput);
        cout << "Client sends: " << pwdInput << endl;
        c.sendData(pwdInput);

        //Receive the answer of the Server
        msg = c.receive(32);
        cout << "Client got response: " << msg << endl;

        //If password is correct, close server and client
        if(msg=="ACCESS ACCEPTED"){
        cout << "Password hacked. Closing Client and Server." << endl;
        c.sendData(string("BYEBYE"));
        return 0;
        }
    }
}

*/

/* Original Main

srand(time(NULL));
	TCPclient c;
	string host = "localhost";
	string msg;

	//connect to host
	c.conn(host , 2022);

	int i=0;
	bool goOn=1;
	while(goOn){ // send and receive data
		if((rand() % 20) < i++){
			msg = string("BYEBYE");
			goOn = 0;
		}else{
			msg = string("client wants this");
		}
		cout << "client sends:" << msg << endl;
		c.sendData(msg);
		msg = c.receive(32);
		cout << "got response:" << msg << endl;
		sleep(1);

	}

*/


