#include "TestDimClient.h"
#include <iosfwd>
/*
 * mainClient.cpp
 *
 *  Created on: 30 Jan 2014
 *      Author: ncl
 */

#include <iostream>
using namespace std;

TestDimClient *client;

void sighandler(int signum){
	delete client;

	exit(signum);
}

int main(int argc, char** argv){
	signal(SIGINT, sighandler);

	std::string serverName = "dimExample";
	if(argc>1) serverName = argv[1];
	client = new TestDimClient(serverName);

	string input;
	while(true){
		cin >> input;

		if(input.compare("init")==0) client->initialize();
		if(input.compare("start")==0) client->startrun();
		if(input.compare("end")==0) client->endrun();
		if(input.compare("reset")==0) client->reset();
		if(input.compare("request")==0) client->requestConfig();
		if(input.compare("t")==0) client->test();
	}
}
