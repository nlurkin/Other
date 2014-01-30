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
#include <stdlib.h>

Command::Command(string dimServerName, NA62DimServer *parent):
		DimCommand((dimServerName + "/Command").c_str(), "C")
{
	p = parent;
}

void Command::commandHandler(){
	p->print("Received: ");
	p->println(getString());

	char *t = strtok(getString(), " ");
	vector<string> tok;

	while(t){
		tok.push_back(t);
		t = strtok(NULL, " ");
	}

	if(tok[0].compare("initialize")==0) doInitialize(tok);
	if(tok[0].compare("startrun")==0) doStartRun(tok);
	if(tok[0].compare("endrun")==0) doEndRun(tok);
	if(tok[0].compare("resetstate")==0) doResetState(tok);
}
void Command::doInitialize(vector<string> tok){
	p->println("Initializing... Waiting for possible configuration file");
	p->setWaiting(true);
}
void Command::doStartRun(vector<string> tok){
	if(p->getState()==kINITIALIZED){
		p->println("Starting run number " + tok[1]);
		p->setRunNumber(atoi(tok[1].c_str()));
		p->setState(kREADY);
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

FileContent::FileContent(string dimServerName, NA62DimServer *parent):
		DimCommand((dimServerName + "/FileContent").c_str(), "C")
{
	p = parent;
}
void FileContent::commandHandler(){
	cout << "Received:" << getString() << endl;
}

EndTransfer::EndTransfer(string dimServerName, NA62DimServer *parent):
		DimCommand((dimServerName + "/EndTransfer").c_str(), "I")
{
	p = parent;
}
void EndTransfer::commandHandler(){
	cout << "Received:" << getInt() << endl;
}
