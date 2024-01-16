//============================================================================
// Name        : INF3_Prak.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C, Ansi-style
//============================================================================

#include <string>
#include <iostream>

#include <cstdio>      /* printf, NULL */
#include <cstdlib>     /* srand, rand */
#include <ctime>       /* time */

#include <unistd.h>

#include "SHA256.H"
#include "TASK1.H"
#include "SIMPLESOCKET.H"

using namespace std;


int main(){
	srand(time(nullptr));
    TCPserver serv(2000,50);
    TCPclient client;

    cout << "The main.c is not used" << endl;

    return 0;
}





