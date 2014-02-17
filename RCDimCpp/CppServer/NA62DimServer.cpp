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


NA62DimServer::NA62DimServer(string name){
	dimServerName = name;
	state = 0;
	nextState = -1;

	info = new char[STRING_MAX_LENGTH+1];
	logging = new char[STRING_MAX_LENGTH+1];
	config = new char[CONFIG_MAX_LENGTH+1];

	infoIndex = 0;

	dimState = new DimService((dimServerName + "/State").c_str(), state);
	dimInfo = new DimService((dimServerName + "/Info").c_str(), info);
	dimLogging = new DimService((dimServerName + "/Logging").c_str(), logging);
	dimConfig = new DimService((dimServerName + "/Config").c_str(), config);

	dimCommand = new Command(dimServerName, this);
	dimFileContent = new FileContent(dimServerName, this);
	dimRequestConfig = new RequestConfig(dimServerName, this);

	runNumber = 0;
	frequency = 0.;
	sourceID = 0;
	uselessInt = 0;
	uselessString = "";
	param = 0;
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
	info[infoIndex+strlen(s)] = '\n';
	info[infoIndex+strlen(s)+1] = '\0';
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
	dimState->updateService();
}

void NA62DimServer::setFrequency(double frequency) {
	this->frequency = frequency;
}

void NA62DimServer::setSourceId(int sourceId) {
	sourceID = sourceId;
}

void NA62DimServer::setUselessInt(int uselessInt) {
	this->uselessInt = uselessInt;
}

void NA62DimServer::setUselessString(const string& uselessString) {
	this->uselessString = uselessString;
}

double NA62DimServer::getFrequency() const {
	return frequency;
}

int NA62DimServer::getSourceId() const {
	return sourceID;
}

int NA62DimServer::getUselessInt() const {
	return uselessInt;
}

const string& NA62DimServer::getUselessString() const {
	return uselessString;
}

int NA62DimServer::getParam() const {
	return param;
}

void NA62DimServer::setParam(int param) {
	this->param = param;
}

void NA62DimServer::setConfig(string config) {
	strcpy(this->config, config.c_str());
	dimConfig->updateService();
}

int NA62DimServer::getNextState() const {
	return nextState;
}

void NA62DimServer::setNextState(int nextState) {
	this->nextState = nextState;
}
