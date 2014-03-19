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
#include "ConfigDecoder.h"
#include <sstream>

void Command::commandHandler(){
	p->print("Command port receiving: ");
	p->println(getString());

	vector<string> tok = ConfigDecoder::tokenize(getString());

	string commandName = tok[0];
	tok.erase(tok.begin());
	selectCommand(commandName, tok);
}

void Command::selectCommand(string commandName, vector<string> tok){
	if(tok[0].compare("initialize")==0) doInitialize(tok);
	if(tok[0].compare("startrun")==0) doStartRun(tok);
	if(tok[0].compare("endrun")==0) doEndRun(tok);
	if(tok[0].compare("resetstate")==0) doResetState(tok);
}

void Command::doInitialize(vector<string> tok){
	if(p->getState()!=kREADY){
		p->println("Initializing... Waiting for configuration file");
		p->setNextState(kINITIALIZED);
	}
}
void Command::doStartRun(vector<string> tok){
	if(p->getState()==kINITIALIZED){
		p->println("Starting run number " + tok[1] + "... Waiting for configuration file");
		p->setRunNumber(atoi(tok[1].c_str()));
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
		p->println(")");
		p->setState(kINITIALIZED);
	}
	else{
		p->println("Device is not in READY state. Cannot stop a run.");
		p->setState(kWRONGSTATE);
	}
}
void Command::doResetState(vector<string> tok){
	p->println("Reset requested");
	p->setState(kIDLE);
}

void FileContent::commandHandler(){
	p->print("FileContent port receiving: ");
	p->println(getString());

	decoder.parseFile(getString());

	p->println("Finished processing config file... Moving to next state.");
	p->setState(p->getNextState());
	p->setNextState(-1);
}

void RequestConfig::commandHandler(){
	p->print("RequestConfig port receiving: ");
	p->println(getInt());

	if(getInt()==1){
		//Generate final configuration file
		p->generateConfig();
	}
	else{
		p->println("Unexpected value received from RequestConfig port.");
		p->setState(kUNKNOWN);
	}
}
