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
	fDimCommand(NULL),
	fDimFileContent(NULL),
	fDimRequestConfig(NULL),
	fRunNumber(0),
	fRunType("")
{
}

NA62DimServer::~NA62DimServer(){
	delete[] fInfo;
	delete[] fLogging;
	delete[] fConfig;
	delete fDimState;
	delete fDimInfo;
	delete fDimLogging;
	delete fDimConfig;

	if(fDimCommand) delete fDimCommand;
	if(fDimFileContent) delete fDimFileContent;
	if(fDimRequestConfig) delete fDimRequestConfig;
}

void NA62DimServer::initCommands(Command *cmdCommand, FileContent *fileContentCommand, RequestConfig *requestConfigCommand)
{
	if(!cmdCommand) fDimCommand = new Command(fDimServerName, this);
	else fDimCommand = cmdCommand;
	if(!fileContentCommand) fDimFileContent = new FileContent(fDimServerName, this);
	else fDimFileContent = fileContentCommand;
	if(!fDimRequestConfig) fDimRequestConfig = new RequestConfig(fDimServerName, this);
	else fDimRequestConfig = requestConfigCommand;

}
void NA62DimServer::start(){
	if(!fDimCommand || !fDimFileContent || !fDimRequestConfig) initCommands(fDimCommand, fDimFileContent, fDimRequestConfig);

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
	fNextState = -1;
	fDimState->updateService();
}

void NA62DimServer::moveToExpectedState() {
	setState(fNextState);
}

void NA62DimServer::setNextState(int nextState) {
	fNextState = nextState;
}

void NA62DimServer::publishConfig(){
	stringstream ss;
	generateConfig(ss);
	strcpy(fConfig, ss.str().c_str());
	fDimConfig->updateService();
}

void NA62DimServer::waitConfigurationFile(int expectedState) {
	println("... Waiting for configuration file");
	setNextState(expectedState);
}
