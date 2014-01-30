/*
 * NA62DimServerx.cpp
 *
 *  Created on: 30 Jan 2014
 *      Author: ncl
 */

#include "NA62DimServer.h"
#include <iostream>
#include <sstream>

using namespace std;

#define STRING_MAX_LENGTH 100
#define CONFIG_MAX_LENGTH 1000


NA62DimServer::NA62DimServer(string name){
	dimServerName = name;
	state = 0;
	waiting = 0;
	info = new char[STRING_MAX_LENGTH+1];
	logging = new char[STRING_MAX_LENGTH+1];
	config = new char[CONFIG_MAX_LENGTH+1];

	infoIndex = 0;

	dimState = new DimService((dimServerName + "/State").c_str(), state);
	dimInfo = new DimService((dimServerName + "/Info").c_str(), info);
	dimLogging = new DimService((dimServerName + "/Logging").c_str(), logging);
	dimWaiting = new DimService((dimServerName + "/Waiting").c_str(), waiting);
	dimConfig = new DimService((dimServerName + "/Config").c_str(), config);

	dimCommand = new Command(dimServerName, this);
	dimFileContent = new FileContent(dimServerName, this);
	dimEndTransfer = new EndTransfer(dimServerName, this);
}
void NA62DimServer::start(){
	DimServer::start(dimServerName.c_str());

	println(dimServerName + " is starting.");

}
void NA62DimServer::print(const char * s){
	cout << s;
	strcpy(info+infoIndex, s);
	infoIndex+=strlen(s);
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
	strcpy(info+infoIndex, s);
	info[strlen(s)] = '\n';
	infoIndex = 0;
	dimInfo->updateService();
}
void NA62DimServer::println(string s){
	println(s.c_str());
}
void NA62DimServer::println(int s){
	ostringstream ss;
	ss << s;
	println(ss.str());
}
void NA62DimServer::mainLoop(){

}

int NA62DimServer::getRunNumber() const {
	return runNumber;
}

void NA62DimServer::setRunNumber(int runNumber) {
	this->runNumber = runNumber;
}

int NA62DimServer::getState() const {
	return state;
}

void NA62DimServer::setState(int state) {
	this->state = state;
}

int NA62DimServer::getWaiting() const {
	return waiting;
}

void NA62DimServer::setWaiting(int waiting) {
	this->waiting = waiting;
}
