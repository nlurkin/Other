/*
 * TestDimClient.cpp
 *
 *  Created on: 30 Jan 2014
 *      Author: ncl
 */

#include "TestDimClient.h"
#include "dic.hxx"
#include <iostream>
#include <fstream>

#define NONE "\x1b[0m"
#define YELLOW "\x1b[43m"
#define BLUE "\x1b[44m"
#define GREEN "\x1b[42m"
#define RED "\x1b[41m"


using namespace std;

TestDimClient::TestDimClient(string name) {
	dimServerName = name;
	infoState = new DimInfo((name + "/State").c_str(), -1, this);
	infoInfo = new DimInfo((name + "/Info").c_str(), (char*)"", this);
	infoLogging = new DimInfo((name + "/Logging").c_str(), (char*)"", this);
	infoWaiting = new DimInfo((name + "/Waiting").c_str(), -1, this);
	infoConfig = new DimInfo((name + "/Config").c_str(), (char*)"", this);

	currentFile = -1;
	deviceState = -1;
	transferStatus = kENDED;

	files.push_back("config1");
	files.push_back("config2");
	files.push_back("config3");
}

TestDimClient::~TestDimClient() {
	// TODO Auto-generated destructor stub
	cout << NONE << endl;
}

void TestDimClient::infoHandler() {
	DimInfo *curr = getInfo();
	if(curr==infoState){
		handleState(curr->getInt());
	}
	else if(curr==infoInfo){
		handleInfo(curr->getString());
	}
	else if(curr==infoLogging){
		handleLogging(curr->getString());
	}
	else if(curr==infoWaiting){
		handleWaiting(curr->getInt());
	}
	else if(curr==infoConfig){
		handleConfig(curr->getString());
	}
}

void TestDimClient::handleState(int i) {
	deviceState = i;
	print("");
	if(transferStatus!=kENDED) resetTransfer();
}

void TestDimClient::resetTransfer(){
	modifyTransferStatus(kENDED);
	currentFile = -1;
}

void TestDimClient::handleInfo(string s) {
	print("\tInfo: " + s);
}

void TestDimClient::handleLogging(string s) {
	print("\tLogging: " + s);
}

void TestDimClient::handleWaiting(int i) {
	cout << "Waiting has moved to " << i << "(status: " << transferStatus << " " << ((transferStatus==kWAITING) ? "OK" : "NOK") << ")" << endl;
	if(i==1 && transferStatus==kWAITING){
		sendNextFile();
	}
}

void TestDimClient::initialize(){
	sendCommand((dimServerName + "/Command").c_str(), "initialize muons");

	modifyTransferStatus(kSTARTED);
	sendNextFile();
}

void TestDimClient::startrun() {
	sendCommand((dimServerName + "/Command").c_str(), "startrun 1");
}

void TestDimClient::endrun() {
	sendCommand((dimServerName + "/Command").c_str(), "endrun");
}

void TestDimClient::reset() {
	sendCommand((dimServerName + "/Command").c_str(), "resetstate");
}

void TestDimClient::handleConfig(string s) {
	print("Full config received");
	print(s);
}

void TestDimClient::sendNextFile() {
	ifstream fd;
	string content;
	char *c = new char[1000];
	currentFile++;
	if(files.size()>currentFile){

		modifyTransferStatus(kSENDING);
		cout << "Sending file " << files[currentFile] << endl;
		fd.open(files[currentFile].c_str());

		fd.seekg(0,ios::end);
		content.reserve(fd.tellg());
		fd.seekg(0, ios::beg);

		content.assign((istreambuf_iterator<char>(fd)), istreambuf_iterator<char>());

		fd.close();
		modifyTransferStatus(kWAITING);
		strcpy(c, content.c_str());
		sendCommandNB((dimServerName + "/FileContent").c_str(), c);
		delete[] c;
	}
	else{
		endTransfer();
	}
}

void TestDimClient::endTransfer(){
	currentFile=-1;

	sendCommand((dimServerName + "/EndTransfer").c_str(), 1);
	modifyTransferStatus(kENDED);
}

void TestDimClient::modifyTransferStatus(int st) {
	string n;

	if(st==kSTARTED) n = "kSTARTED";
	else if(st==kSENDING) n = "kSENDING";
	else if(st==kWAITING) n = "kWAITING";
	else if(st==kENDED) n = "kENDED";
	cout << "STATUS=" << n << endl;
	transferStatus = st;
}

void TestDimClient::print(string s){
	string color;
	if(deviceState==kIDLE) color = YELLOW;
	else if(deviceState==kINITIALIZED) color = BLUE;
	else if(deviceState==kREADY) color = GREEN;
	else color = RED;

	cout << color << s << endl;
}

int TestDimClient::getDeviceState() const {
	return deviceState;
}


void TestDimClient::test(){
	sendCommand((dimServerName + "/Command").c_str(), "initialize muons");
	sendCommand((dimServerName + "/FileContent").c_str(), "xxx");
	sendCommand((dimServerName + "/FileContent").c_str(), "xxx");
	sendCommand((dimServerName + "/EndTransfer").c_str(), 1);
	sendCommand((dimServerName + "/FileContent").c_str(), "xxx");
}
