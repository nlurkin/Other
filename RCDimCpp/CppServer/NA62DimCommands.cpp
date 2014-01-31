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

Command::Command(string dimServerName, NA62DimServer *parent):
DimCommand((dimServerName + "/Command").c_str(), "C")
{
	p = parent;
}

void Command::commandHandler(){
	p->print("Command port receiving: ");
	p->println(getString());

	vector<string> tok = ConfigDecoder::tokenize(getString());

	if(tok[0].compare("initialize")==0) doInitialize(tok);
	if(tok[0].compare("startrun")==0) doStartRun(tok);
	if(tok[0].compare("endrun")==0) doEndRun(tok);
	if(tok[0].compare("resetstate")==0) doResetState(tok);
}
void Command::doInitialize(vector<string> tok){
	if(p->getState()!=kREADY){
		p->println("Initializing... Waiting for possible configuration file");
		p->setWaiting(1);
	}
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
	if(p->getWaiting()==1){
		p->print("FileContent port receiving: ");
		p->println(getString());

		decoder.parseFile(getString());
		//Applying parameters
		p->setSourceId(decoder.param3);
		p->setFrequency(decoder.param4);
		p->setUselessInt(decoder.param1);
		p->setUselessString(decoder.param5);
		p->setParam(decoder.param2);

		p->println("Finished processing config file... Waiting for next one.");
		p->setWaiting(1);
	}
	else{
		p->setState(p->getState());
	}
}

EndTransfer::EndTransfer(string dimServerName, NA62DimServer *parent):
				DimCommand((dimServerName + "/EndTransfer").c_str(), "I")
{
	p = parent;
}
void EndTransfer::commandHandler(){
	if(p->getWaiting()==1){
		p->print("EndTransfer port receiving: ");
		p->println(getInt());

		if(getInt()==1){
			//Generate final configuration file
			stringstream ss;
			ss << "uselessInt=" << p->getUselessInt() << endl;
			ss << "param=" << p->getParam() << endl;
			ss << "sourceID=0x" << hex << p->getSourceId() << endl;
			ss << "frequency=" << p->getFrequency() << endl;
			ss << "uselessString=" << p->getUselessString() << endl;

			p->setWaiting(3);
			p->setConfig(ss.str());
			p->setState(kINITIALIZED);
		}
		else{
			p->println("Unexpected value received from FileContent port.");
			p->setState(kUNKNOWN);
		}
	}
}
