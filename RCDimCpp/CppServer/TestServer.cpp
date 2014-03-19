/*
 * TestServer.cpp
 *
 *  Created on: Mar 19, 2014
 *      Author: nlurkin
 */

#include "TestServer.h"

TestServer::TestServer(string name):
NA62DimServer(name),
fFrequency(0.),
fSourceID(0),
fUselessString(""),
fUselessInt(0),
fParam(0)
{
	//Replace the default FileContent command with  the TestFileContent one.
	initCommands(new TestCommand(getDimServerName(), this), new TestFileContent(getDimServerName(), this), NULL);
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

void TestServer::setUselessString(const string& uselessString) {
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

const string& TestServer::getUselessString() const {
	return fUselessString;
}

int TestServer::getParam() const {
	return fParam;
}

void TestServer::mainLoop()
{
}

void TestServer::generateConfig(stringstream& ss) {
	ss << "uselessInt=" << fUselessInt << endl;
	ss << "param=" << fParam << endl;
	ss << "sourceID=0x" << hex << fSourceID << endl;
	ss << "frequency=" << fFrequency << endl;
	ss << "uselessString=" << fUselessString << endl;
}

void TestServer::setParam(int param) {
	fParam = param;
}

void TestFileContent::decodeFile(string fileContent) {
	fDecoder.parseFile(fileContent);

	p->setParam(fDecoder.param2);
	p->setSourceId(fDecoder.param3);
	p->setUselessInt(fDecoder.param1);
	p->setFrequency(fDecoder.param4);
	p->setUselessString(fDecoder.param5);
}

void TestCommand::doEndRun(vector<string> tok) {
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

void TestCommand::doResetState(vector<string> tok) {
	p->println("Reset requested");
	p->setState(kIDLE);
}
