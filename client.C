#include <string>
#include <iostream>
#include <unistd.h> //contains various constants

#include "client.H"
#include <string>
#include "TASK1.H"
#include <fstream>

using namespace std;


int main(){
    srand(time(NULL));

    pwdCheckerClient client;
    string host = "localhost";
    string msg = {};

    //Connect with Server. Try 10 times before closing the process
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
    }

    cout << "Connected to the Server" << endl << endl;

    //maxPwdLength & maxCharAmount abfragen
    int maxPwdLength;
    cout << "Please enter the maximum length of the password: ";
    cin >> maxPwdLength;

    int maxCharAmount;
    cout << "Please enter the maximum amount of available characters for the password: ";
    cin >> maxCharAmount;

    //maxCharAmount should not be bigger actual number of available characters, which are stored in the TASK1::SYMBOLS
    if(maxCharAmount>TASK1::SYMBOLS.size()){
        maxCharAmount = TASK1::SYMBOLS.size();
    }

    int repetitions;
    cout << "How often should the analysis be repeated (max. 10)? ";
    cin >> repetitions;
    cout << endl;

    //Limit number of repetitions to ten
    if(repetitions>10){
        repetitions = 10;
    }

    //Create Stream to .csv file
    ofstream resultsStream("Results/Results.csv");
    if(!resultsStream){
        cout << "Output file could not be created. Pleas create a \"Results\" folder before starting the client." << endl;

        //close the server
        client.sendData("BYEBYE");

        //end the client
        return 0;

    } else{
        cout << "Output file created" << endl << endl;
    }

    //write the header line in the results file
    resultsStream << "repetition; password length; available characters; trys; time" << endl;

    //Repeat the analysis as often as specified with the repetitions input
    for(int analysisNr = 0; analysisNr < repetitions; analysisNr++){

        cout << "ANALYSIS NUMBER: " << analysisNr << endl << endl;

        //Loop over every Number of available characters
        for(int charsAvailable = 1; charsAvailable <= maxCharAmount; charsAvailable++){

            //Loop over all lenghts of passwords
            for(int pwdLenght = 1; pwdLenght <= maxPwdLength; pwdLenght++){

                //Loop over max 10 trys, in which the client trys to request a new password. Server has to answer with "ServerStatus(Password created)" to continue.
                for(int i = 0; i < 10; i++){

                    //Send "Create-New-Password"-Command to the Server
                    client.sendData(pwdMsg::newMsg("NewPassword", to_string(pwdLenght), to_string(charsAvailable)));
                    cout << "Sent message to server:\t" << pwdMsg::newMsg("NewPassword", to_string(pwdLenght), to_string(charsAvailable)) << endl;

                    //Wait for the server to confirm, that a new password was created
                    pwdMsg recmsg(client.receive(MESSAGE_SIZE));
                    cout << "Answer from Server:\t" << recmsg.newMsg(recmsg.id_, recmsg.arg1_, recmsg.arg2_, recmsg.arg3_, recmsg.arg4_) << endl << endl;

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

                //Set Start time point.
                time_t timebuffer = time(NULL);

                //Creating all possible Password with a call of the recursive createPwdRec function.
                client.createPwdRec(password, pwdLenght-1, charsAvailable, counterOfTrials, passwordHacked, &client);

                cout << "Counter of Trials: " << counterOfTrials << "\tFinally hacked? " << passwordHacked << endl << endl << endl;

                //Save pwdLength, charsAvailable, CounterOfTrials and used time in .csv file
                resultsStream << (analysisNr+1) << ";" << pwdLenght << ";" << charsAvailable << ";" << counterOfTrials << ";" << (int)(time(NULL)-timebuffer) << endl;

            }
        }
    }

    cout << "The Results of this analysis have been saved in the Results.csv file in the Results folder." << endl << endl;

    //end Filestream
    resultsStream.close();

    //close the server
    client.sendData("BYEBYE");

    return 0;
}

void pwdCheckerClient::createPwdRec(string& password, int index, int chars, int& counterOfTrials, bool& passwordHacked, pwdCheckerClient* ptrClient){
    //Loop for replacing the character on the current index with every available character
    for(int i = 0; i < chars; i++){
        password.at(index) = TASK1::SYMBOLS.at(i);

        if(index != 0){
            //If the current index is not the first index (0) of the password, call this function again and manipulate the character on the index one lower
            pwdCheckerClient::createPwdRec(password, index-1, chars, counterOfTrials, passwordHacked, ptrClient);

            //If the password was hacked in the function call before, end the recusive function calls of guessing the password.
            if(passwordHacked==true){
                return;
            }
        } else{
            //If the current index is the first index, the current password is one of the passwords which should be sended to the server. Increase the counter of Trial with 1.
            counterOfTrials++;

            ptrClient->sendData(pwdMsg::newMsg("CheckPassword", password));
            cout << "Sent message to server:\t" << pwdMsg::newMsg("CheckPassword", password) << endl;

            //Receive an answer of the server.
            pwdMsg recmsg(ptrClient->receive(MESSAGE_SIZE));
            //cout << "Answer from Server:\t" << recmsg.newMsg(recmsg.id_, recmsg.arg1_, recmsg.arg2_, recmsg.arg3_, recmsg.arg4_) << endl << endl;

                        //Simulation of Server answer (for testing the client without the server)
                        /*
                        pwdMsg recmsg;
                        int coincidence = rand()%25;
                        if(coincidence == 1){
                            cout << "Simulated Answer:\tAccess(Allowed)" << endl;
                            recmsg.readMsg("Access(Allowed)");
                            cout << "correct password: " << password << endl;
                        } else{
                            recmsg.readMsg("Access(Denied)");
                            cout << "Simulated Answer:\tAccess(Denied)" << endl;
                        }*/

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
