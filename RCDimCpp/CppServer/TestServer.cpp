/*
 * TestServer.cpp
 *
 *  Created on: Mar 19, 2014
 *      Author: nlurkin
 */

#include "TestServer.h"
#include <iostream>

TestServer::TestServer(std::string name, int sourceID):
NA62DimServer(name, sourceID),
fFrequency(0.),
fSourceID(0),
fUselessString(""),
fUselessInt(0),
fParam(0)
{
	//Replace the default FileContent command with  the TestFileContent one.
	initCommands(new TestCommand(getDimServerName(), this), new TestFileContent(getDimServerName(), this), NULL);

	centralizedLog(0, "Starting server", 1);
	NA62DimServer::fConfigStruct = new TestNode;
}

TestServer::~TestServer() {
}

void TestServer::setFrequency(double frequency) {
	fFrequency = frequency;
}

void TestServer::setSourceId(int sourceId) {
	fSourceID = sourceId;
}

void TestServer::setUselessInt(int uselessInt) {
	fUselessInt = uselessInt;
}

void TestServer::setUselessString(const std::string& uselessString) {
	fUselessString = uselessString;
}

double TestServer::getFrequency() const {
	return fFrequency;
}

int TestServer::getSourceId() const {
	return fSourceID;
}

int TestServer::getUselessInt() const {
	return fUselessInt;
}

const std::string& TestServer::getUselessString() const {
	return fUselessString;
}

int TestServer::getParam() const {
	return fParam;
}

void TestServer::mainLoop()
{
}

void TestServer::generateConfig(std::stringstream& ss) {
	//Generate the current configuration stream using the same format as the input file.
	ss << "uselessInt=" << fUselessInt << std::endl;
	ss << "param=" << fParam << std::endl;
	ss << "sourceID=0x" << std::hex << fSourceID << std::endl;
	ss << "frequency=" << fFrequency << std::endl;
	ss << "uselessString=" << fUselessString << std::endl;
}

void TestServer::setParam(int param) {
	fParam = param;
}

bool TestFileContent::decodeFile(std::string fileContent, void* structPtr) {
	fDecoder.parseFile(fileContent, (TestNode*)structPtr);
	return true;
}

bool TestServer::applyConfiguration(){
	TestNode *s = (TestNode*)fConfigStruct;
	setParam(s->param);
	setSourceId(s->sourceID);
	setUselessInt(s->uselessInt);
	setFrequency(s->frequency);
	setUselessString(s->uselessString);

	return true;
}

void TestCommand::doEndRun(std::vector<std::string> tok) {
	if(p->getState()==kREADY){
		p->print("Stopping current run (");
		p->print(p->getRunNumber());
		p->println(")");
		p->setState(kINITIALIZED);
	}
	else{
		p->println("Device is not in READY state. Cannot stop a run.");
		p->setState(kWRONGSTATE);
	}
}

void TestCommand::doResetState(std::vector<std::string> tok) {
	p->println("Reset requested");
	p->setState(kIDLE);
}
