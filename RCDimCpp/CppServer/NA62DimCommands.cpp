/*
 * NA62DimCommands.cpp
 *
 *  Created on: 30 Jan 2014
 *      Author: ncl
 */

#include "NA62DimCommands.h"
#include "NA62DimServer.h"
#include <iostream>
#include <vector>
#include <sstream>

void Command::commandHandler(){
	p->print("Command port receiving: ");
	p->println(getString());

	vector<string> tok = tokenize(getString());

	string commandName = tok[0];
	tok.erase(tok.begin());
	selectCommand(commandName, tok);
}

void Command::selectCommand(string commandName, vector<string> tok){
	if(commandName.compare("initialize")==0) doInitialize(tok);
	if(commandName.compare("startrun")==0) doStartRun(tok);
	if(commandName.compare("endrun")==0) doEndRun(tok);
	if(commandName.compare("resetstate")==0) doResetState(tok);
}

void Command::doInitialize(vector<string> tok){
	if(p->getState()!=kREADY){
		p->println("Initializing... Waiting for configuration file");
		p->setNextState(kINITIALIZED);
	}
}
void Command::doStartRun(vector<string> tok){
	if(p->getState()==kINITIALIZED){
		p->println("Starting run number " + tok[0] + "... Waiting for configuration file");
		p->setRunNumber(atoi(tok[0].c_str()));
		p->setNextState(kREADY);
	}
	else{
		p->println("Device is not in INITIALIZED state. Cannot start a run.");
		p->setState(kWRONGSTATE);
	}
}
void Command::doEndRun(vector<string> tok){
	if(p->getState()==kREADY){
		p->print("Stopping current run (");
		p->print(p->getRunNumber());
		p->println(") ... Waiting for configuration file");
		p->setNextState(kINITIALIZED);
	}
	else{
		p->println("Device is not in READY state. Cannot stop a run.");
		p->setState(kWRONGSTATE);
	}
}
void Command::doResetState(vector<string> tok){
	p->println("Reset requested ... Waiting for configuration file");
	p->setNextState(kIDLE);
}

void FileContent::commandHandler(){
	p->print("FileContent port receiving: ");
	p->println(getString());

	decodeFile(getString());

	p->println("Finished processing config file... Moving to next state.");
	p->setState(p->getNextState());
	p->setNextState(-1);
}

void RequestConfig::commandHandler(){
	p->print("RequestConfig port receiving: ");
	p->println(getInt());

	if(getInt()==1){
		//Generate final configuration file
		p->publishConfig();
	}
	else{
		p->println("Unexpected value received from RequestConfig port.");
		p->setState(kUNKNOWN);
	}
}

const vector<string> tokenize(string s, const char delim) {
	vector<string> tokens;
	stringstream ss(s);
	string item;

	while(getline(ss, item, delim)){
		tokens.push_back(item);
	}
	return tokens;
}
