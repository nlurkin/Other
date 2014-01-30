/*
 * TestDimClient.cpp
 *
 *  Created on: 30 Jan 2014
 *      Author: ncl
 */

#include "TestDimClient.h"
#include "dic.hxx"
#include <iostream>

using namespace std;

TestDimClient::TestDimClient(string name) {
	dimServerName = name;
	infoState = new DimInfo((name + "/State").c_str(), -1, this);
	infoInfo = new DimInfo((name + "/Info").c_str(), "", this);
	infoLogging = new DimInfo((name + "/Logging").c_str(), "", this);
	infoWaiting = new DimInfo((name + "/Waiting").c_str(), -1, this);
	infoConfig = new DimInfo((name + "/Config").c_str(), "", this);

	files.push_back("config1");
	files.push_back("config2");
	files.push_back("config3");
}

TestDimClient::~TestDimClient() {
	// TODO Auto-generated destructor stub
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
}

void TestDimClient::handleInfo(string s) {
	cout << "Info: " <<s << endl;
}

void TestDimClient::handleLogging(string s) {
	cout << "Logging: " <<s << endl;
}

void TestDimClient::handleWaiting(int i) {
}

void TestDimClient::initialize() {
}

void TestDimClient::startrun() {
	sendCommand((dimServerName + "/Command").c_str(), "startrun 1");
}

void TestDimClient::endrun() {
}

void TestDimClient::reset() {
}

void TestDimClient::handleConfig(string s) {
}

void TestDimClient::sendFiles() {
}
