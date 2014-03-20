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

/**
 * Command handler.
 *
 * This method is called when a string is received on dimServerName/Command.
 * The string is tokenized (blank space as separator).
 * The first token is considered as the command and all following tokens are
 * considered as parameters. The parameters are placed in a std::vector of
 * string. The command and parameters are passed to the selectCommand method.
 */
void Command::commandHandler(){
	p->print("Command port receiving: ");
	p->println(getString());

	//Tokenize received string
	std::vector<std::string> tok = tokenize(getString());

	//First token is the command name, all others are parameters.
	//Keep only parameters in the vector.
	std::string commandName = tok[0];
	tok.erase(tok.begin());

	//Call select command to decide which command we received.
	selectCommand(commandName, tok);
}

/**
 * Decide which command we received on the dimServerName/Command command port.
 * @param commandName: command
 * @param tok: vector of (string) parameters
 * @return true if the command was recognized, else false.
 */
bool Command::selectCommand(std::string commandName, std::vector<std::string> tok){
	if(commandName.compare("initialize")==0) doInitialize(tok);
	else if(commandName.compare("startrun")==0) doStartRun(tok);
	else if(commandName.compare("endrun")==0) doEndRun(tok);
	else if(commandName.compare("resetstate")==0) doResetState(tok);
	else return false;	//If none of the above, return false
	return true;
}

/**
 * Handler for the initialize command.
 *
 * If the current state is kIDLE or kINITIALIZED, wait for a configuration file
 * and set the next expected state to kINITIALIZED.
 * Else set the state to kWRONGSTATE
 * @param tok: list of (string) parameters
 */
void Command::doInitialize(std::vector<std::string> tok){

	if(p->getState()==kIDLE || p->getState()==kINITIALIZED){
		//If in one off the right state, wait for a config file
		p->println("Initializing");
		p->waitConfigurationFile(kINITIALIZED);
	}
	else{
		//Go to error
		p->println("Device is not in IDLE or INITIALIZED state. Cannot initialize.");
		p->setState(kWRONGSTATE);
	}
}

/**
 * Handler for the startrun command.
 *
 * If the current state is kINITIALIZED, wait for a configuration file
 * and set the next expected state to kREADY.
 * Else set the state to kWRONGSTATE
 * @param tok: list of (string) parameters
 */
void Command::doStartRun(std::vector<std::string> tok){
	if(p->getState()==kINITIALIZED){
		//If in one off the right state, wait for a config file
		p->println("Starting run number " + tok[0]);
		p->setRunNumber(atoi(tok[0].c_str()));
		p->waitConfigurationFile(kREADY);
	}
	else{
		//Go to error
		p->println("Device is not in INITIALIZED state. Cannot start a run.");
		p->setState(kWRONGSTATE);
	}
}

/**
 * Handler for the endrun command.
 *
 * If the current state is kREADY, wait for a configuration file
 * and set the next expected state to kINITIALIZED.
 * Else set the state to kWRONGSTATE
 * @param tok: list of (string) parameters
 */
void Command::doEndRun(std::vector<std::string> tok){
	//If in one off the right state, wait for a config file
	if(p->getState()==kREADY){
		p->print("Stopping current run (");
		p->print(p->getRunNumber());
		p->println(")");
		p->waitConfigurationFile(kINITIALIZED);
	}
	else{
		//Go to error
		p->println("Device is not in READY state. Cannot stop a run.");
		p->setState(kWRONGSTATE);
	}
}

/**
 * Handler for the resetstate command.
 *
 * Wait for a configuration file and set the next expected state to kIDLE.
 * @param tok: list of (string) parameters
 */
void Command::doResetState(std::vector<std::string> tok){
	p->println("Reset requested");
	p->waitConfigurationFile(kIDLE);
}


/**
 * FileContent handler.
 *
 * Calls the decodeFile method. If successful (decodeFile returns true),
 * tries to apply the configuration by calling the virtual applyConfiguration
 * method of the parent server.
 * If both are successful, move to the next expected state.
 * If one of them fails, move to kCONFIGERROR.
 */
//TODO implement the applyConfiguration mechanism
void FileContent::commandHandler(){
	bool success;
	p->print("FileContent port receiving: ");
	p->println(getString());

	//Try to decode the file. If successful, try to apply the configuration to the parent server
	if(decodeFile(getString())) success = p->applyConfig(NULL);
	else success = false;

	if(success){
		//Successful, move to next expected state
		p->println("Finished processing config file... Moving to next state.");
		p->moveToExpectedState();
	}
	else {
		//Move to error
		p->println("Error while applying configuration.");
		p->setState(kCONFIGERROR);
	}
}

/**
 * RequestConfig handler.
 *
 * Calls the publishConfig() method of the parent server.
 */
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

/**
 * Tokenize a string according to the specified delimiter
 * @param s: string to tokenize
 * @param delim: delimiter (default=' ')
 * @return std::vector of string containing all the tokens.
 */
const std::vector<std::string> tokenize(std::string s, const char delim) {
	std::vector<std::string> tokens;
	std::stringstream ss(s);
	std::string item;

	while(getline(ss, item, delim)){
		tokens.push_back(item);
	}
	return tokens;
}
