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

    pwdCheckerClient client;
    string host = "localhost";
    string msg = {};

    /*
    int connectionTrys = 0;
    while(1){
        if(client.conn(host,PORT)==true){
            break;
        } else{
            if(connectionTrys<10){
                sleep(1);
                connectionTrys++;
            } else {
                cout << "Connection Failed. Closing Client" << endl;
                return 0;
            }
        }
    }*/
    cout << "Connected to the Server" << endl;


    for(int CharsAvailable = 1; CharsAvailable <= MAX_CHAR_COUNT; CharsAvailable++){
        for(int pwdLenght = 1; pwdLenght <= MAX_PWD_LENGHT; pwdLenght++){

            for(int i = 1; i < 11; i++){

                //Send Create-New-Password Command to the Server
                cout << "Sent: " << writeMessage("NewPassword", to_string(pwdLenght), to_string(CharsAvailable)) << endl;
                //client.sendData(writeMessage("NewPassword", to_string(pwdLenght), to_string(CharsAvailable)));

                //Wait for the server to confirm, that a new password was created
                //msg = client.receive(MESSAGE_SIZE);
                msg = "ServerStatus(Password Created)";
                if(msg == "ServerStatus(Password Created)"){
                    break;
                } else if (i == 10){
                    cout << "Server can not create the password. Client stopped." << endl;
                    return 0;
                }
                sleep(1);
            }

            //Create the initial Password
            string password(pwdLenght, TASK1::SYMBOLS.at(0));

            //recursive call of Function createPwdRec to create every possible password with the given length and available characters
            int CounterOfTrials = 0;
            bool passwordHacked = false;
            client.createPwdRec(password, pwdLenght-1, CharsAvailable, CounterOfTrials, passwordHacked);

            cout << "Counter of Trials: " << CounterOfTrials << "\tFinally hacked? " << passwordHacked << endl << endl;


        }
    }

}

void pwdCheckerClient::createPwdRec(string& password, int index, int Chars, int& CounterOfTrials, bool& passwordHacked){
    //Replace the character on the current index with every available character
    for(int i = 0; i < Chars; i++){
        password.at(index) = TASK1::SYMBOLS.at(i);
        //If the current index is not the first index of the password, call this function again and manipulate the character on the index one lower
        if(index != 0){
            pwdCheckerClient::createPwdRec(password, index-1, Chars, CounterOfTrials, passwordHacked);
            if(passwordHacked==true){
            return;
            }
        } else{
        //If the current index is the first index, it is one of the individual passwords which should be sended to the server.
            CounterOfTrials++;
            cout << "Sent: " << writeMessage("CheckPassword", password) << endl;
            //client.sendData(writeMessage("CheckPassword", password));
            //string msg = client.receive(MESSAGE_SIZE);
            string msg = "Access(Denied)";

            msgStruct accessMessage = readMessage(msg);

            if(accessMessage.arg1 == "Allowed"){
                passwordHacked = true;
            }
        }
    }
    return;
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


