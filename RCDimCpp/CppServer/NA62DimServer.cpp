/*
 * NA62DimServerx.cpp
 *
 *  Created on: 30 Jan 2014
 *      Author: ncl
 */

#include <iostream>
#include <sstream>
#include "NA62DimServer.h"

using namespace std;

#define STRING_MAX_LENGTH 100
#define CONFIG_MAX_LENGTH 1000


NA62DimServer::NA62DimServer(string name):
	fDimServerName(name),
	fState(0),
	fNextState(-1),
	fInfo(new char[STRING_MAX_LENGTH+1]),
	fLogging(new char[STRING_MAX_LENGTH+1]),
	fConfig(new char[CONFIG_MAX_LENGTH+1]),
	fInfoIndex(0),
	fDimState(new DimService((fDimServerName + "/State").c_str(), fState)),
	fDimInfo(new DimService((fDimServerName + "/Info").c_str(), fInfo)),
	fDimLogging(new DimService((fDimServerName + "/Logging").c_str(), fLogging)),
	fDimConfig(new DimService((fDimServerName + "/Config").c_str(), fConfig)),
	fDimCommand(new Command(fDimServerName, this)),
	fDimFileContent(new FileContent(fDimServerName, this)),
	fDimRequestConfig(new RequestConfig(fDimServerName, this)),
	fRunNumber(0),
	fRunType("")
{
}

void NA62DimServer::start(){
	DimServer::start(fDimServerName.c_str());

	println(fDimServerName + " is starting.");
}


void NA62DimServer::print(const char * s){
	cout << s;
	strcpy(fInfo+fInfoIndex, s);
	fInfoIndex+=strlen(s);
}
void NA62DimServer::print(string s){
	print(s.c_str());
}
void NA62DimServer::print(int s){
	ostringstream ss;
	ss << s;
	print(ss.str());
}
void NA62DimServer::println(const char *s){
	cout << s << endl;
	strcpy(fInfo+fInfoIndex, s);
	fInfo[fInfoIndex+strlen(s)] = '\n';
	fInfo[fInfoIndex+strlen(s)+1] = '\0';
	fInfoIndex = 0;
	fDimInfo->updateService();
}
void NA62DimServer::println(string s){
	println(s.c_str());
}
void NA62DimServer::println(int s){
	ostringstream ss;
	ss << s;
	println(ss.str());
}

int NA62DimServer::getRunNumber() const {
	return fRunNumber;
}

void NA62DimServer::setRunNumber(int runNumber) {
	fRunNumber = runNumber;
}

int NA62DimServer::getState() const {
	return fState;
}

void NA62DimServer::setState(int state) {
	fState = state;
	fDimState->updateService();
}

int NA62DimServer::getNextState() const {
	return fNextState;
}

void NA62DimServer::setNextState(int nextState) {
	fNextState = nextState;
}
