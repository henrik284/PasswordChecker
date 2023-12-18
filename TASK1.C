/*
 * TASK1.C
 *
 *  Created on: 10.09.2019
 *      Author: aml
 */

#include <cstdlib>
#include <cstring>
#include <iostream>

#include "TASK1.H"
#include "SHA256.H"

namespace TASK1{

//BlackboxUNSAFE

BlackBoxUnsafe::BlackBoxUnsafe(int pwdLength, int symbSetSize){
	if((symbSetSize < 1) || (symbSetSize > SYMBOLS.length()) ){
		lengthSymbArray_ = SYMBOLS.length();
	}else{
		lengthSymbArray_ = symbSetSize;
	}
	// extract and save the subset of valid symbols
	charSymbArray_ = new char [lengthSymbArray_ + 1];
	strncpy(charSymbArray_, SYMBOLS.c_str(), lengthSymbArray_);
	// create and save random password
	pwd_ = this->randomPwd(pwdLength);
	return;
}

string BlackBoxUnsafe::getSymbols(){
	return string(charSymbArray_);
};

string BlackBoxUnsafe::input(string strPwd){
	if(strPwd.compare(pwd_) == 0){
		return string("ACCESS ACCEPTED");
	}
	return string("ACCESS DENIED");
}

string BlackBoxUnsafe::randomPwd(int l){
	int symbolIdx;
	if(l < MINIMAL_PWD_LENGTH){ l = MINIMAL_PWD_LENGTH;};
	pwd_ = string("");
	for(int i=0; i < l; i++){
		symbolIdx = rand() % lengthSymbArray_;
		pwd_ += charSymbArray_[symbolIdx];
	}
	return pwd_;
}


//BlackboxSAFE

BlackBoxSafe::BlackBoxSafe(int pwdLength, int symbSetSize) : BlackBoxUnsafe(pwdLength, symbSetSize){
    pwd_ = sha256(pwd_);                                            //Store the Checksum calculated by the sha256 algorithm in the pwd_ member
    return;
}

string BlackBoxSafe::input(string strPwd){
    if(strPwd.compare(sha256(pwd_)) == 0){
        return string("ACCESS ACCEPTED");
    } else {
        return string("ACCESS DENIED");
    }
}


//Demonstrations

void demoTASK1_00(){
	string pwd("meinpassword");
	cout << "pwd   : " << pwd << endl;
	cout << "SHA256: " << sha256(pwd) << endl;
	pwd = ("einpassword");
	cout << "pwd   : " << pwd << endl;
	cout << "SHA256: " << sha256(pwd) << endl;
	return;
}

void demoTASK1_01(){
	TASK1::BlackBoxUnsafe bs(4,4);
	cout << bs.pwd_ << endl;

	cout << bs.input(bs.pwd_) << endl;

	return;
}

void demoTASK1_02(){
    TASK1::BlackBoxUnsafe bb(10, 0);
    cout << "BlackBoxSafe Objekt mit zufaelligem Passwort erstellt. Passwort: " << bb.pwd_ << endl;
    cout << "Geben Sie dies zum Test ein: ";
    string inputPwd;
    cin >> inputPwd;
    cout << bb.input(inputPwd) << endl;

}

}

pwdMsg::pwdMsg(){
    id_ = "";
    arg1_ = "";
    arg2_ = "";
    arg3_ = "";
    arg4_ = "";
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
