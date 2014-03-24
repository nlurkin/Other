/*
 * commands.c
 *
 *  Created on: 31 Jan 2014
 *      Author: Nicolas Lurkin
 */

#include "helper.h"
#include "commands.h"
#include "server.h"
#include "dis.h"
#include <stdlib.h>
#include <string.h>

/**
 * @file commands.c
 * Contains functions implementing the commands described in the note NA62-14-02
 */

/**
 * Standard Command handler.
 *
 * This function is called when a command is received on dimServerName/Command.
 * The string is tokenized (blank space as separator).
 * The first token is considered as the command and all following tokens are
 * considered as parameters. The parameters are placed in an array.
 * The command and parameters are passed to the selectCommand function (to be implemented by user).
 * @param tag
 * @param cmnd_buffer
 * @param size
 */
void stdDimCommand(long* tag, char* cmnd_buffer, int*size){
	print("Command port receiving: ");
	println(cmnd_buffer);

	char args[5][STRING_MAX_LENGTH];

	if(tokenize(args, cmnd_buffer, " ")>0){
		selectCommand(args[0], args);
	}
}

/**
 * Decide which command we received on the dimServerName/Command command port.
 * @param commandName: command
 * @param tok: array of parameters (entry 0 is the command name)
 * @return 0 if the command was recognized, else -1.
 */
int stdSelectCommand(char commandName[STRING_MAX_LENGTH], char tok[5][STRING_MAX_LENGTH]){
	if(strcmp(commandName, "initialize")==0) doStdInitialize(tok);
	else if(strcmp(commandName, "startrun")==0) doStdStartRun(tok);
	else if(strcmp(commandName, "endrun")==0) doStdEndRun(tok);
	else if(strcmp(commandName, "resetstate")==0) doStdResetState(tok);
	else return -1;

	return 0;
}

/**
 * Standard handler for the initialize command.
 *
 * If the current state is kIDLE or kINITIALIZED, wait for a configuration file
 * and set the next expected state to kINITIALIZED.
 * Else set the state to kWRONGSTATE
 * @param tok: list of parameters (entry 0 is the command name)
 */
void doStdInitialize(char tok[5][STRING_MAX_LENGTH]){
	if(fState==kIDLE || fState==kINITIALIZED){
		//If in one off the right state, wait for a config file
		println("Initializing");
		waitConfigurationFile(kINITIALIZED);
	}
	else{
		//Go to error
		println("Device is not in IDLE or INITIALIZED state. Cannot initialize.");
		setState(kWRONGSTATE);
	}
}

/**
 * Standard handler for the startrun command.
 *
 * If the current state is kINITIALIZED, wait for a configuration file
 * and set the next expected state to kREADY.
 * Else set the state to kWRONGSTATE
 * @param tok: list of parameters (entry 0 is the command name)
 */
void doStdStartRun(char tok[5][STRING_MAX_LENGTH]){
	if(fState==kINITIALIZED){
		//If in one off the right state, wait for a config file
		print("Starting run number ");
		println(tok[0]);
		setRunNumber(atoi(tok[0]));
		waitConfigurationFile(kREADY);
	}
	else{
		//Go to error
		println("Device is not in INITIALIZED state. Cannot start a run.");
		setState(kWRONGSTATE);
	}

}

/**
 * Standard handler for the endrun command.
 *
 * If the current state is kREADY, wait for a configuration file
 * and set the next expected state to kINITIALIZED.
 * Else set the state to kWRONGSTATE
 * @param tok: list of parameters (entry 0 is the command name)
 */
void doStdEndRun(char tok[5][STRING_MAX_LENGTH]){
	//If in one off the right state, wait for a config file
	if(getState()==kREADY){
		print("Stopping current run (");
		printi(fRunNumber);
		println(")");
		waitConfigurationFile(kINITIALIZED);
	}
	else{
		//Go to error
		println("Device is not in READY state. Cannot stop a run.");
		setState(kWRONGSTATE);
	}
}

/**
 * Standard handler for the resetstate command.
 *
 * Wait for a configuration file and set the next expected state to kIDLE.
 * @param tok: list of parameters (entry 0 is the command name)
 */
void doStdResetState(char tok[5][STRING_MAX_LENGTH]){
	println("Reset requested");
	waitConfigurationFile(kIDLE);
}

/**
 * Standard FileContent handler.
 *
 * Calls the parseFile method (to be implemented by user). If successful (parseFile returns 0),
 * tries to apply the configuration by calling applyConfiguration (to be implemented by user)
 * If both are successful, move to the next expected state.
 * If one of them fails, move to kCONFIGERROR.
 * @param tag
 * @param cmnd_buffer
 * @param size
 */
void stdDimFileContent(long* tag, char* cmnd_buffer, int*size){
	int success;

	print("FileContent port receiving: ");
	println(cmnd_buffer);

	//Try to decode the file. If successful, try to apply the configuration to the parent server
	if(parseFile(cmnd_buffer, fConfigStruct)==0) success = applyConfiguration();
	else success = -1;

	if(success==0){
		//Successful, move to next expected state
		println("Finished processing config file... Moving to next state.");
		moveToExpectedState();
	}
	else {
		//Move to error
		println("Error while applying configuration.");
		setState(kCONFIGERROR);
	}
}

/**
 * RequestConfig handler.
 *
 * Calls the publishConfig() function.
 * @param tag
 * @param cmnd_buffer
 * @param size
 */
void stdDimRequestConfig(long* tag, char* cmnd_buffer, int*size){
	print("RequestConfig port receiving: ");
	printlni(*cmnd_buffer);

	if(*cmnd_buffer==1){
		publishConfig();
	}
	else{
		println("Unexpected value received from FileContent port.");
		setState(kUNKNOWN);
	}
}
